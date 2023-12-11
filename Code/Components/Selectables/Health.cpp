#include "StdAfx.h"
#include "Health.h"
#include "GamePlugin.h"

#include <Components/Selectables/OwnerInfo.h>
#include <Components/Managers/ActionManager.h>
#include <Actions/IBaseAction.h>

#include <Components/Player/PlayerController.h>

#include "FlashUI/FlashUI.h"
#include "FlashUI/FlashUIElement.h"

#include <Listeners/UIElementEventListener.h>
#include <Components/Selectables/Selectable.h>
#include <Utils/EntityUtils.h>

#include <CryEntitySystem/IEntitySystem.h>
#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

#include <DefaultComponents/Cameras/CameraComponent.h>

namespace
{
	static void RegisterHealthComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CHealthComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterHealthComponent);
}

/******************************************************************************************************************************************************************************/
void CHealthComponent::Initialize()
{
	//Initialize m_pHealthbarUIElement and set it visible
	m_pHealthbarUIElement = gEnv->pFlashUI->GetUIElement("health-bar");
	if (m_pHealthbarUIElement) {
		m_pHealthbarUIElement->SetVisible(true);
	}
	else {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "HealthComponent : (Initialize) m_pHealthbarUIElement is null !");
	}

	//OwnerInfoComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetComponent<COwnerInfoComponent>();
}

/******************************************************************************************************************************************************************************/
Cry::Entity::EventFlags CHealthComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

/******************************************************************************************************************************************************************************/
void CHealthComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {
		//Objects/effects/bloodsplatter/smoke_particle_1.pfx
	}break;
	case Cry::Entity::EEvent::Update: {

		if (!m_pSelectableComponent) {
			//SelectableComponent Initialization
			m_pSelectableComponent = m_pEntity->GetComponent<CSelectableComponent>();
		}

		f32 DeltaTime = event.fParam[0];
		if (fEntityRemoveTimePassed < fTimeBetweenRemoveingEntity) {
			fEntityRemoveTimePassed += 0.5f * DeltaTime;
		}
		if (fHealthbarHidingTimePassed < fTimeBetweenHidingHealthbar) {
			fHealthbarHidingTimePassed += 0.5f * DeltaTime;
		}
		if (fBloodParticleRestartTimePassed < fTimeBetweenRestartingBloodParticle) {
			fBloodParticleRestartTimePassed += 0.5f * DeltaTime;
		}
		else {
			/*
			if (m_pBloodParticleComponent) {
				m_pBloodParticleComponent->Activate(false);
			}
			*/
		}

		UpdateProgressAmount();
		Die();

		//TODO: player hamishe yeki hast ?
		IEntity* player = gEnv->pEntitySystem->FindEntityByName(PLAYER_NAME);
		if (!player) {
			return;
		}
		Cry::DefaultComponents::CCameraComponent* camera = player->GetComponent<Cry::DefaultComponents::CCameraComponent>();
		if (!camera) {
			return;
		}
		if (!m_pOwnerInfoComponent) {
			return;
		}
		COwnerInfoComponent* pPlayerOwnerInfo = player->GetComponent<COwnerInfoComponent>();
		if (!pPlayerOwnerInfo) {
			return;
		}

		if (g_EntityUtils->IsEntityInsideViewPort(camera, m_pEntity) && m_currentHealth > 0 && (m_pSelectableComponent && m_pSelectableComponent->IsSelected() || fHealthbarHidingTimePassed < fTimeBetweenHidingHealthbar || m_lastProgressbarUpdateAmount > 0)) {
			ShowHealthBar();
			fEntityRemoveTimePassed = 0;
		}
		else if (!g_EntityUtils->IsEntityInsideViewPort(camera, m_pEntity)|| m_currentHealth <= 0 || (m_pSelectableComponent && !m_pSelectableComponent->IsSelected()) || fHealthbarHidingTimePassed >= fTimeBetweenHidingHealthbar && m_lastProgressbarUpdateAmount <= 0) {
			HideHealthBar();
			return;
		}

		if (m_pHealthbarUIElement) {
			bool bIsRed = pPlayerOwnerInfo->GetOwnerInfo().m_pPlayerTeam != m_pOwnerInfoComponent->GetOwnerInfo().m_pPlayerTeam;
			AddHealthBar(bIsRed);

			Vec3 flashPos;
			Vec2 borders;
			float scale = 0;
			Vec3 pos = m_pEntity->GetWorldPos();
			m_pHealthbarUIElement->WorldToFlash(camera->GetCamera().GetMatrix(), pos, flashPos, borders, scale);

			if (m_lastHealthUpdateAmount != m_currentHealth) {
				SetHealthAmount((int32)m_currentHealth, bIsRed);
			}
			SetHealthbarPosition(flashPos.x , flashPos.y - 15);
		}

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

/******************************************************************************************************************************************************************************/
void CHealthComponent::UpdateProgressAmount()
{
	CActionManagerComponent* pActionManagerComponent = m_pEntity->GetComponent<CActionManagerComponent>();
	if (!pActionManagerComponent) {
		return;
	}
	if (!pActionManagerComponent->GetCurrentAction()) {
		if (m_lastProgressbarUpdateAmount != 0) {
			SetProgressAmount(0, 100);
			m_lastProgressbarUpdateAmount = 0;
		}
		return;
	}

	if (m_lastProgressbarUpdateAmount == pActionManagerComponent->GetCurrentAction()->GetProgressAmount()) {
		return;
	}

	SetProgressAmount(pActionManagerComponent->GetCurrentAction()->GetProgressAmount(), pActionManagerComponent->GetCurrentAction()->GetMaxProgressAmount());
	m_lastProgressbarUpdateAmount = pActionManagerComponent->GetCurrentAction()->GetProgressAmount();
}

/******************************************************************************************************************************************************************************/
void CHealthComponent::ApplyDamage(f32 damage)
{
	/*
	if (m_pBloodParticleComponent) {
		this->m_pBloodParticleComponent->Activate(true);
	}
	*/
	this->m_currentHealth = CLAMP(m_currentHealth - damage, 0, m_maxHealth);
	this->fHealthbarHidingTimePassed = 0;
	fBloodParticleRestartTimePassed = 0;
}

/******************************************************************************************************************************************************************************/
void CHealthComponent::Die()
{
	if (m_currentHealth <= 0 && fEntityRemoveTimePassed >= fTimeBetweenRemoveingEntity) {
		HideHealthBar();
		g_EntityUtils->RemoveEntity(m_pEntity);
	}
}

/******************************************************************************************************************************************************************************/
void CHealthComponent::SetMaxHealth(f32 maxHealth)
{
	this->m_maxHealth = maxHealth;
	this->m_currentHealth = maxHealth;
}

/******************************************************************************************************************************************************************************/
f32 CHealthComponent::GetCurrentHealth()
{
	return m_currentHealth;
}

/******************************************************************************************************************************************************************************/
f32 CHealthComponent::GetMaxHealth()
{
	return m_maxHealth;
}

/******************************************************************************************************************************************************************************/
bool CHealthComponent::IsConsumesFood()
{
	return bIsConsumesFood;
}

/******************************************************************************************************************************************************************************/
void CHealthComponent::SetConsumesFood(bool consumesFood)
{
	this->bIsConsumesFood = consumesFood;
}

/******************************************************************************************************************************************************************************/
void CHealthComponent::AddHealthBar(bool isRed)
{
	if (bHealthBarAdded) {
		return;
	}

	//Input arguments
	SUIArguments args;
	args.AddArgument(0);
	args.AddArgument(0);
	args.AddArgument(isRed);

	//Returns index
	TUIData data;

	m_pHealthbarUIElement->CallFunction("AddHealthbar", args, &data);

	//Set Index
	int index = -1;
	data.GetValueWithConversion(index);
	m_healthbarIndex = index;

	bHealthBarAdded = true;

	HideHealthBar();
}

/******************************************************************************************************************************************************************************/
void CHealthComponent::HideHealthBar()
{
	SUIArguments args;
	args.AddArgument(m_healthbarIndex);

	m_pHealthbarUIElement->CallFunction("HideHealthbar", args);
}

/******************************************************************************************************************************************************************************/
void CHealthComponent::ShowHealthBar()
{
	SUIArguments args;
	args.AddArgument(m_healthbarIndex);

	m_pHealthbarUIElement->CallFunction("ShowHealthbar", args);
}

/******************************************************************************************************************************************************************************/
void CHealthComponent::SetHealthbarPosition(f32 x, f32 y)
{
	SUIArguments args;
	args.AddArgument(m_healthbarIndex);
	args.AddArgument(x);
	args.AddArgument(y);

	m_pHealthbarUIElement->CallFunction("SetPosition", args);
}

/******************************************************************************************************************************************************************************/
void CHealthComponent::SetHealthAmount(int32 health, bool isRed)
{
	SUIArguments args;
	args.AddArgument(m_healthbarIndex);
	args.AddArgument(m_currentHealth / m_maxHealth);
	args.AddArgument(isRed);

	m_lastHealthUpdateAmount = m_currentHealth;
	m_pHealthbarUIElement->CallFunction("SetHealthAmount", args);
}

/******************************************************************************************************************************************************************************/
void CHealthComponent::ClearAllHealthbars()
{
	m_pHealthbarUIElement->CallFunction("ClearHealthbars");
}

/******************************************************************************************************************************************************************************/
void CHealthComponent::AddWorkerSlot(int32 slotNumber, bool isFilled)
{
	SUIArguments args;
	args.AddArgument(m_healthbarIndex);
	args.AddArgument(slotNumber);
	args.AddArgument(isFilled);

	m_pHealthbarUIElement->CallFunction("AddWorkerSlot", args);
}

/******************************************************************************************************************************************************************************/
void CHealthComponent::ClearWorkersSlots()
{
	SUIArguments args;
	args.AddArgument(m_healthbarIndex);

	m_pHealthbarUIElement->CallFunction("ClearWorkerSlots", args);
}

/******************************************************************************************************************************************************************************/
void CHealthComponent::SetProgressAmount(f32 progressAmount, f32 maxProgressAmount)
{
	SUIArguments args;
	args.AddArgument(m_healthbarIndex);
	args.AddArgument(progressAmount / maxProgressAmount);

	m_pHealthbarUIElement->CallFunction("SetProgressAmount", args);
}

/******************************************************************************************************************************************************************************/
void CHealthComponent::HideProgressbar()
{
	SUIArguments args;
	args.AddArgument(m_healthbarIndex);

	m_pHealthbarUIElement->CallFunction("HideProgressbar", args);
}

/******************************************************************************************************************************************************************************/
void CHealthComponent::ShowProgressbar()
{
	SUIArguments args;
	args.AddArgument(m_healthbarIndex);

	m_pHealthbarUIElement->CallFunction("ShowProgressbar", args);
}

/******************************************************************************************************************************************************************************/