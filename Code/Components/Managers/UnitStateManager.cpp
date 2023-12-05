#include "StdAfx.h"
#include "UnitStateManager.h"
#include "GamePlugin.h"

#include <Actions/IBaseAction.h>
#include <Components/Controller/AIController.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterUnitStateManagerComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CUnitStateManagerComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterUnitStateManagerComponent);
}

/******************************************************************************************************************************************************************************/
void CUnitStateManagerComponent::Initialize()
{
	//IControllerComponent Initialization
	m_pAIControllerComponent = m_pEntity->GetComponent<CAIControllerComponent>();
}

/******************************************************************************************************************************************************************************/
Cry::Entity::EventFlags CUnitStateManagerComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

/******************************************************************************************************************************************************************************/
void CUnitStateManagerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];

		UpdateSpeed();
		UpdateState();

	}break;
	case Cry::Entity::EEvent::Reset: {


	}break;
	default:
		break;
	}
}

/******************************************************************************************************************************************************************************/
void CUnitStateManagerComponent::UpdateState()
{
	if (!m_pAIControllerComponent) {
		m_pAIControllerComponent = m_pEntity->GetComponent<CAIControllerComponent>();
		return;
	}

	if (!m_pAIControllerComponent->IsMoving()) {
		m_pUnitState = EUnitState::IDLE;

		if (m_pUnitStance == EUnitStance::RUNNING) {
			SetStanceToWalk();
		}
	}
	else if (m_pAIControllerComponent->IsMoving() ) {
		m_pUnitState = EUnitState::WALKING;
	}
}

/******************************************************************************************************************************************************************************/
void CUnitStateManagerComponent::UpdateSpeed()
{
	if (m_pUnitStance == EUnitStance::PRONE) {
		m_currentSpeed = m_proneSpeed;
	}
	else if (m_pUnitStance == EUnitStance::CROUCH) {
		m_currentSpeed = m_crouchSpeed;
	}
	else if (m_pUnitStance == EUnitStance::STANDING) {
		m_currentSpeed = m_walkSpeed;
	}
	else if (m_pUnitStance == EUnitStance::RUNNING) {
		m_currentSpeed = m_runSpeed;
	}
}

/******************************************************************************************************************************************************************************/
EUnitState CUnitStateManagerComponent::GetState()
{
	return this->m_pUnitState;
}

/******************************************************************************************************************************************************************************/
EUnitStance CUnitStateManagerComponent::GetStance()
{
	return this->m_pUnitStance;
}

/******************************************************************************************************************************************************************************/
f32 CUnitStateManagerComponent::GetCurrentSpeed()
{
	return this->m_currentSpeed;
}

/******************************************************************************************************************************************************************************/
void CUnitStateManagerComponent::SetStanceToRun()
{
	this->m_pUnitStance = EUnitStance::RUNNING;
}

/******************************************************************************************************************************************************************************/
void CUnitStateManagerComponent::SetStanceToWalk()
{
	this->m_pUnitStance = EUnitStance::STANDING;
}

/******************************************************************************************************************************************************************************/