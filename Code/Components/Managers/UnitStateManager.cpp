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
		UpdateCurrentHeight();

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
			m_pUnitStance = EUnitStance::STANDING;
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
void CUnitStateManagerComponent::UpdateCurrentHeight()
{
	if (m_pUnitStance == EUnitStance::PRONE) {
		m_currentHeight = m_proneHeight;
	}
	else if (m_pUnitStance == EUnitStance::CROUCH) {
		m_currentHeight = m_crouchHeight;
	}
	else if (m_pUnitStance == EUnitStance::STANDING) {
		m_currentHeight = m_standingHeight;
	}
	else if (m_pUnitStance == EUnitStance::RUNNING) {
		m_currentHeight = m_standingHeight;
	}
}

/******************************************************************************************************************************************************************************/
EUnitState CUnitStateManagerComponent::GetState()
{
	return this->m_pUnitState;
}

/******************************************************************************************************************************************************************************/
void CUnitStateManagerComponent::SetStance(EUnitStance newStance)
{
	this->m_pUnitStance = newStance;
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
f32 CUnitStateManagerComponent::GetCurrentHeight()
{
	return m_currentHeight;
}

/******************************************************************************************************************************************************************************/