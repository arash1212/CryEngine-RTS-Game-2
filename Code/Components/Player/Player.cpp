#include "StdAfx.h"
#include "Player.h"
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

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterPlayerComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPlayerComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPlayerComponent);
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::Initialize()
{
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

	//Inputs Initialization
	InitInputs();
}

/******************************************************************************************************************************************************************************/
Cry::Entity::EventFlags CPlayerComponent::GetEventMask() const
{
	return 
		Cry::Entity::EEvent::Initialize |
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		f32 fDeltaTime = event.fParam[0];

		Move(fDeltaTime);

		if (m_rightClickCountRestartTimePassed < m_timeBetweenRightClickCountRestart) {
			m_rightClickCountRestartTimePassed += 0.5f * fDeltaTime;
		}
		else {
			m_rightClickCount = 0;
		}

		g_CoverUtils->FindCoverPointsAroundPosition(g_MouseUtils->GetPositionUnderCursor(), 2, m_selectedEntities.size());

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

void CPlayerComponent::InitInputs()
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
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::Move(f32 deltaTime)
{
	Vec3 position = m_pEntity->GetWorldPos();
	position.x += m_movementOffset.x * m_movementSpeed * deltaTime;
	position.y += m_movementOffset.y * m_movementSpeed * deltaTime;
	m_pEntity->SetPos(position);
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::MoveForwardPressed(int activationMode, float value)
{
	m_movementOffset.y = value;
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::MoveBackwardPressed(int activationMode, float value)
{
	m_movementOffset.y = -value;
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::MoveRightPressed(int activationMode, float value)
{
	m_movementOffset.x = value;
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::MoveLeftPressed(int activationMode, float value)
{
	m_movementOffset.x = -value;
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::SelectionPressed(int activationMode, float value)
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
void CPlayerComponent::CommandPressed(int activationMode, float value)
{
	Vec3 mousePos = g_MouseUtils->GetPositionUnderCursor();
	if (activationMode == eAAM_OnRelease) {

		m_rightClickCount++;
		m_rightClickCountRestartTimePassed = 0;

		CommandSelectedUnitsToMoveTo(mousePos);
	}
}

/*=============================================================================================================================================
																	Selections
==============================================================================================================================================*/

void CPlayerComponent::SelectSelectables()
{
	for (IEntity* entity : m_selectedEntities) {
		CSelectableComponent* pSelectableComponent = entity->GetComponent<CSelectableComponent>();
		if (!pSelectableComponent) {
			continue;
		}

		pSelectableComponent->Select();
	}

	UpdateActionbarItems();
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::DeSelectSelectables()
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


/*=============================================================================================================================================
																	Commands
==============================================================================================================================================*/

void CPlayerComponent::CommandSelectedUnitsToMoveTo(Vec3 position)
{
	CryLog("size units : %i", m_selectedEntities.size());
	DynArray<Vec3> coverPoints = g_CoverUtils->FindCoverPointsAroundPosition(position, 2, m_selectedEntities.size());

	for (int32 i = 0; i < m_selectedEntities.size(); i++) {
		CActionManagerComponent* pActionManagerComponent = m_selectedEntities[i]->GetComponent<CActionManagerComponent>();
		if (!pActionManagerComponent) {
			continue;
		}

		if (coverPoints.size() > i) {
			pActionManagerComponent->AddAction(new UnitMoveAction(m_selectedEntities[i], coverPoints[i], m_rightClickCount >= 2, true));
		}
		else {
			pActionManagerComponent->AddAction(new UnitMoveAction(m_selectedEntities[i], position, m_rightClickCount >= 2, false));
		}
	}
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::UpdateActionbarItems()
{
	if (m_selectedEntities.size() <= 0) {
		return;
	}

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
void CPlayerComponent::ExecuteActionbarItem(int32 index)
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