#include "StdAfx.h"
#include "PlayerController.h"
#include "GamePlugin.h"

#include <Utils/CoverUtils.h>
#include <Utils/EntityUtils.h>
#include <Utils/MouseUtils.h>
#include <Components/UI/BoxSelectionUI.h>
#include <Components/UI/InGameUI.h>
#include <Components/Selectables/Selectable.h>
#include <Components/Controller/AIController.h>
#include <Components/Managers/ActionManager.h>
#include <Actions/Units/UnitMoveAction.h>
#include <UIItems/IBaseUIItem.h>
#include <Components/Selectables/UIItemProvider.h>
#include <Listeners/UIElementEventListener.h>
#include <Components/Cover/EntityCoverUser.h>
#include <Components/Cover/CoverPosition.h>
#include <Components/Player/Player.h>
#include <Components/Selectables/OwnerInfo.h>
#include <Actions/Units/UnitAttackAction.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterPlayerControllerComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPlayerControllerComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPlayerControllerComponent);
}

/******************************************************************************************************************************************************************************/
void CPlayerControllerComponent::Initialize()
{
	m_defaultPosZ = m_pEntity->GetPos().z;

	//InputComponent Initialization
	m_pInputComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();

	//CameraComponent Initialization
	m_pCameraComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCameraComponent>();
	m_pCameraComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), Quat::CreateRotationX(DEG2RAD(-55)), Vec3(0, 0, 10)));

	//UIElementEventListener Initialization
	m_pUIElementEventListener = new UIElementEventListener();
	m_pUIElementEventListener->SetPlayerComponent(this);

	//UIBoxSelectionComponent Initialization
	m_pBoxSelectionUIComponent = m_pEntity->GetOrCreateComponent<CBoxSelectionUIComponent>();
	m_pBoxSelectionUIComponent->SetEventListener(m_pUIElementEventListener);
	//m_pUIBoxSelectionComponent->SetCameraComponent(m_pCameraComponent);

	//InGameUIComponent Initialization
	m_pInGameUIComponent = m_pEntity->GetOrCreateComponent<CInGameUIComponent>();
	m_pInGameUIComponent->SetEventListener(m_pUIElementEventListener);

	//PlayerComponent Initialization
	m_pPlayerComponent = m_pEntity->GetOrCreateComponent<CPlayerComponent>();
	m_pPlayerComponent->SetIsAI(false);

	//TODO
	//OwnerInfoComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetComponent<COwnerInfoComponent>();
	SOwnerInfo pOwnerInfo;
	pOwnerInfo.m_pPlayer = EPlayer::PLAYER1;
	pOwnerInfo.m_pPlayerFaction = EPlayerFaction::FACTION_1;
	pOwnerInfo.m_pPlayerTeam = EPlayerTeam::TEAM_1;
	pOwnerInfo.m_pPlayerComponent = m_pPlayerComponent;
	m_pOwnerInfoComponent->SetOwnerInfo(pOwnerInfo);

	//AudioListenerComponent initialization
	m_pAudioListenerComp = m_pEntity->GetOrCreateComponent<Cry::Audio::DefaultComponents::CListenerComponent>();

	m_pEntity->SetName(PLAYER_NAME);

	//Inputs Initialization
	InitInputs();
}

/******************************************************************************************************************************************************************************/
Cry::Entity::EventFlags CPlayerControllerComponent::GetEventMask() const
{
	return 
		Cry::Entity::EEvent::Initialize |
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

/******************************************************************************************************************************************************************************/
void CPlayerControllerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		f32 fDeltaTime = event.fParam[0];

		Move(fDeltaTime);

		ValidateSelectables();

		if (m_rightClickCountRestartTimePassed < m_timeBetweenRightClickCountRestart) {
			m_rightClickCountRestartTimePassed += 0.5f * fDeltaTime;
		}
		else {
			m_rightClickCount = 0;
		}

		g_CoverUtils->FindCoverUsers();

		g_CoverUtils->FindCoverPointsAroundPosition(g_MouseUtils->GetPositionUnderCursor(), nullptr, 2, m_selectedEntities.size());

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}


/*=============================================================================================================================================
																	INPUTS
==============================================================================================================================================*/

void CPlayerControllerComponent::InitInputs()
{
	//MoveForward
	m_pInputComponent->RegisterAction("player", "forward", [this](int activationMode, float value) {this->MoveForwardPressed(activationMode, value); });
	m_pInputComponent->BindAction("player", "forward", eAID_KeyboardMouse, eKI_W);

	//MoveBackward
	m_pInputComponent->RegisterAction("player", "backward", [this](int activationMode, float value) {this->MoveBackwardPressed(activationMode, value); });
	m_pInputComponent->BindAction("player", "backward", eAID_KeyboardMouse, eKI_S);

	//MoveRight
	m_pInputComponent->RegisterAction("player", "right", [this](int activationMode, float value) {this->MoveRightPressed(activationMode, value); });
	m_pInputComponent->BindAction("player", "right", eAID_KeyboardMouse, eKI_D);

	//MoveLeft
	m_pInputComponent->RegisterAction("player", "left", [this](int activationMode, float value) {this->MoveLeftPressed(activationMode, value); });
	m_pInputComponent->BindAction("player", "left", eAID_KeyboardMouse, eKI_A);

	//Selection
	m_pInputComponent->RegisterAction("player", "select", [this](int activationMode, float value) {this->SelectionPressed(activationMode, value); });
	m_pInputComponent->BindAction("player", "select", eAID_KeyboardMouse, eKI_Mouse1);

	//Command
	m_pInputComponent->RegisterAction("player", "command", [this](int activationMode, float value) {this->CommandPressed(activationMode, value); });
	m_pInputComponent->BindAction("player", "command", eAID_KeyboardMouse, eKI_Mouse2);

	//Camera ZoomIn
	m_pInputComponent->RegisterAction("player", "zoomIn", [this](int activationMode, float value) {this->ZoonInPressed(activationMode, value); });
	m_pInputComponent->BindAction("player", "zoomIn", eAID_KeyboardMouse, eKI_MouseWheelUp);

	//Camera ZoomOut
	m_pInputComponent->RegisterAction("player", "zoomOut", [this](int activationMode, float value) {this->ZoomOutPressed(activationMode, value); });
	m_pInputComponent->BindAction("player", "zoomOut", eAID_KeyboardMouse, eKI_MouseWheelDown);
}

/******************************************************************************************************************************************************************************/
void CPlayerControllerComponent::Move(f32 deltaTime)
{
	Vec3 position = m_pEntity->GetWorldPos();
	position.x += m_movementOffset.x * m_movementSpeed * deltaTime;
	position.y += m_movementOffset.y * m_movementSpeed * deltaTime;

	//Apply zoomAmount
	position.z = m_defaultPosZ - m_currentZoomAmount;

	m_pEntity->SetPos(position);
}

/******************************************************************************************************************************************************************************/
void CPlayerControllerComponent::MoveForwardPressed(int activationMode, float value)
{
	m_movementOffset.y = value;
}

/******************************************************************************************************************************************************************************/
void CPlayerControllerComponent::MoveBackwardPressed(int activationMode, float value)
{
	m_movementOffset.y = -value;
}

/******************************************************************************************************************************************************************************/
void CPlayerControllerComponent::MoveRightPressed(int activationMode, float value)
{
	m_movementOffset.x = value;
}

/******************************************************************************************************************************************************************************/
void CPlayerControllerComponent::MoveLeftPressed(int activationMode, float value)
{
	m_movementOffset.x = -value;
}

/******************************************************************************************************************************************************************************/
void CPlayerControllerComponent::SelectionPressed(int activationMode, float value)
{
	Vec2 mouseScreenPos = g_MouseUtils->GetCursorScreenPosition();
	if (m_pUIElementEventListener->IsMouseOverUI()) {
		return;
	}

	if (activationMode == eAAM_OnPress) {
		m_pBoxSelectionUIComponent->SetBoxStartPos(mouseScreenPos);

		DeSelectSelectables();
	}

	if (activationMode == eAAM_OnRelease) {
		m_selectedEntities = m_pBoxSelectionUIComponent->GetEntitiesInsideSelectionBox(mouseScreenPos);

		CryLog("size added : %i", m_selectedEntities.size());

		SelectSelectables();
	}
}

/******************************************************************************************************************************************************************************/
void CPlayerControllerComponent::CommandPressed(int activationMode, float value)
{
	Vec3 mousePos = g_MouseUtils->GetPositionUnderCursor();
	IEntity* pEntityUnderCursor = g_MouseUtils->GetEntityUnderCursor();
	if (activationMode == eAAM_OnRelease) {

		m_rightClickCount++;
		m_rightClickCountRestartTimePassed = 0;

		if (pEntityUnderCursor) {
			COwnerInfoComponent* pOwnerInfoComponent = pEntityUnderCursor->GetComponent<COwnerInfoComponent>();
			if (pOwnerInfoComponent && pOwnerInfoComponent->GetOwnerInfo().m_pPlayerTeam != m_pOwnerInfoComponent->GetOwnerInfo().m_pPlayerTeam) {
				CommandUnitsToAttack(pEntityUnderCursor);
				return;
			}
		}

		CommandSelectedUnitsToMoveTo(mousePos);
	}
}

/******************************************************************************************************************************************************************************/
void CPlayerControllerComponent::ZoonInPressed(int activationMode, float value)
{
	m_currentZoomAmount = CLAMP(m_currentZoomAmount + (value * gEnv->pTimer->GetFrameTime()), 0, m_cameraMaxZoomAmount);
}

/******************************************************************************************************************************************************************************/
void CPlayerControllerComponent::ZoomOutPressed(int activationMode, float value)
{
	m_currentZoomAmount = CLAMP(m_currentZoomAmount + (value * gEnv->pTimer->GetFrameTime()), 0, m_cameraMaxZoomAmount);
}

/*=============================================================================================================================================
																	Selections
==============================================================================================================================================*/

void CPlayerControllerComponent::SelectSelectables()
{
	DynArray<IEntity*> selectedEntities;
	for (IEntity* entity : m_selectedEntities) {
		CSelectableComponent* pSelectableComponent = entity->GetComponent<CSelectableComponent>();
		if (!pSelectableComponent) {
			continue;
		}
		COwnerInfoComponent* pOwnerInfoComponent = entity->GetComponent<COwnerInfoComponent>();
		if (!pOwnerInfoComponent || pOwnerInfoComponent && pOwnerInfoComponent->GetOwnerInfo().m_pPlayer != m_pOwnerInfoComponent->GetOwnerInfo().m_pPlayer) {
			continue;
		}

		pSelectableComponent->Select();
		selectedEntities.append(entity);
	}
	m_selectedEntities = selectedEntities;
	//UpdateActionbarItems();
}

/******************************************************************************************************************************************************************************/
void CPlayerControllerComponent::DeSelectSelectables()
{
	m_pInGameUIComponent->ClearActionbarItems();
	for (IEntity* entity : m_selectedEntities) {
		CSelectableComponent* pSelectableComponent = entity->GetComponent<CSelectableComponent>();
		if (!pSelectableComponent) {
			continue;
		}

		pSelectableComponent->DeSelect();
	}

	m_selectedEntities.clear();
	m_currentUIItems.clear();
}

/******************************************************************************************************************************************************************************/
void CPlayerControllerComponent::ValidateSelectables()
{
	if (m_selectedEntities.size() != m_lastSelectablesCheckSize) {
		m_lastSelectablesCheckSize = m_selectedEntities.size();
		//m_lastTypesSizeCheck = -1;
		//m_pInGameUIComponent->ClearActionbarItems();
		UpdateActionbarItems();
		SelectSelectables();
	}

	DynArray<IEntity*> result;
	for (IEntity* entity : m_selectedEntities) {
		if (!entity || entity->IsGarbage()) {
			continue;
		}

		result.append(entity);
	}
	m_selectedEntities = result;
}


/*=============================================================================================================================================
																	Commands
==============================================================================================================================================*/

void CPlayerControllerComponent::CommandSelectedUnitsToMoveTo(Vec3 position)
{
	DynArray<CCoverPosition*> coverPoints = g_CoverUtils->FindCoverPointsAroundPosition(position, nullptr, 2, m_selectedEntities.size());

	int32 row = 0, column = 0;
	int32 unitsCount = m_selectedEntities.size();
	//IPersistantDebug* pd = gEnv->pGameFramework->GetIPersistantDebug();
	//pd->Begin("CommandSelectedUnitsToMoveTo_MovePoints", false);

	for (int32 i = 0; i < m_selectedEntities.size(); i++) {
		IEntity* pEntity = m_selectedEntities[i];
		CActionManagerComponent* pActionManagerComponent = pEntity->GetComponent<CActionManagerComponent>();
		if (!pActionManagerComponent) {
			continue;
		}

		if (coverPoints.size() > i) {
			pActionManagerComponent->AddAction(new UnitMoveAction(pEntity, coverPoints[i]->GetCoverPosition(), m_rightClickCount >= 2, coverPoints[i]));
		}
		else {
			IEntity* closestUnit = g_EntityUtils->GetClosestEntity(m_selectedEntities, position);
			f32 diffX = crymath::abs(position.x - closestUnit->GetWorldPos().x);
			f32 diffY = crymath::abs(position.y - closestUnit->GetWorldPos().y);
			int32 rowSize = unitsCount / 2;
			if (unitsCount < 4) {
				rowSize = 2;
			}

			if (diffX > diffY) {
				if (i != 0) {
					row++;
				}
				if (i != 0 && i % rowSize == 0) {
					column++;
					row = 0;
				}
			}
			else {
				if (i != 0) {
					column++;
				}
				if (i != 0 && i % rowSize == 0) {
					row++;
					column = 0;
				}
			}
			AABB aabb;
			m_selectedEntities[i]->GetWorldBounds(aabb);
			f32 width = aabb.max.x - aabb.min.x;
			f32 height = aabb.max.y - aabb.min.y;
			Vec3 pos = Vec3(position.x + ((column * (width + 1.0f))), position.y - ((row * (height + 1.0f))), position.z);

			//pd->AddSphere(pos, 0.3f, ColorF(0, 1, 1), 3);
			pActionManagerComponent->AddAction(new UnitMoveAction(pEntity, pos, m_rightClickCount >= 2, nullptr));
		}
	}
}

/******************************************************************************************************************************************************************************/
void CPlayerControllerComponent::UpdateActionbarItems()
{
	if (m_selectedEntities.size() <= 0) {
		m_currentUIItems.clear();
		m_pInGameUIComponent->ClearActionbarItems();
		return;
	}

	m_currentUIItems.clear();
	m_pInGameUIComponent->ClearActionbarItems();

	CUIItemProviderComponent* pUIItemProviderComponent = m_selectedEntities[0]->GetComponent<CUIItemProviderComponent>();
	if (m_selectedEntities.size() > 1) {
		m_currentUIItems = pUIItemProviderComponent->GetGeneralUIItems();
	}
	else if (m_selectedEntities.size() == 1) {
		m_currentUIItems = pUIItemProviderComponent->GetAllUIItems();
	}

	for (IBaseUIItem* item : m_currentUIItems) {
		m_pInGameUIComponent->AddActionbarItem(item->GetImagePath());
	}
}

/******************************************************************************************************************************************************************************/
void CPlayerControllerComponent::ExecuteActionbarItem(int32 index)
{
	CryLog("action bar index : %i", index);
	for (IEntity* entity : m_selectedEntities) {
		CUIItemProviderComponent* pUIItemProviderComponent = entity->GetComponent<CUIItemProviderComponent>();
		if (m_selectedEntities.size() > 1) {
			pUIItemProviderComponent->GetGeneralUIItems()[index]->Execute();
		}
		else if (m_selectedEntities.size() == 1) {
			pUIItemProviderComponent->GetAllUIItems()[index]->Execute();
		}
	}
}

/******************************************************************************************************************************************************************************/
void CPlayerControllerComponent::CommandUnitsToAttack(IEntity* target)
{
	for (IEntity* entity : m_selectedEntities) {
		if (!entity) {
			continue;
		}

		CActionManagerComponent* actionManager = entity->GetComponent<CActionManagerComponent>();
		if (actionManager) {
			if (target) {
				actionManager->AddAction(new UnitAttackAction(entity, target));
			}
		}
		else {
			continue;
		}
	}
}

/******************************************************************************************************************************************************************************/