#include "StdAfx.h"
#include "AIController.h"
#include "GamePlugin.h"

#include <CryAISystem/Components/IEntityNavigationComponent.h>
#include <Components/Managers/UnitStateManager.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterAIControllerComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CAIControllerComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterAIControllerComponent);
}

/******************************************************************************************************************************************************************************/
void CAIControllerComponent::Initialize()
{
	//CharacterControllerComponent Initialization
	m_pCharacterControllerComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
	m_pCharacterControllerComponent->SetTransformMatrix(Matrix34::Create(Vec3(0.01f), IDENTITY, Vec3(0, 0, 1)));

	//NavigationComponent Initialization
	m_pNavigationComponent = m_pEntity->GetOrCreateComponent<IEntityNavigationComponent>();
	m_pNavigationComponent->SetNavigationAgentType("MediumSizedCharacters");
	//MovementProperties
	IEntityNavigationComponent::SMovementProperties m_movementProps;
	m_movementProps.normalSpeed = 3.f;
	m_movementProps.minSpeed = 3.5;
	m_movementProps.maxSpeed = 5;
	m_movementProps.lookAheadDistance = 0.1f;
	m_movementProps.maxDeceleration = 1200;
	m_movementProps.bStopAtEnd = true;
	m_pNavigationComponent->SetMovementProperties(m_movementProps);

	//Collision avoidance
	IEntityNavigationComponent::SCollisionAvoidanceProperties collisionAvoidanceProps;
	collisionAvoidanceProps.radius = 0.01f;
	m_pNavigationComponent->SetCollisionAvoidanceProperties(collisionAvoidanceProps);

	//UnitStateManagerComponen Initialization
	m_pUnitStateManagerComponent = m_pEntity->GetOrCreateComponent<CUnitStateManagerComponent>();

}

/******************************************************************************************************************************************************************************/
Cry::Entity::EventFlags CAIControllerComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::Initialize |
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

/******************************************************************************************************************************************************************************/
void CAIControllerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		Move();

	}break;
	case Cry::Entity::EEvent::Reset: {
		m_pNavigationComponent->SetRequestedVelocity(ZERO);

	}break;
	default:
		break;
	}
}

/******************************************************************************************************************************************************************************/
void CAIControllerComponent::Move()
{
	m_pCharacterControllerComponent->SetVelocity(m_pNavigationComponent->GetRequestedVelocity() * m_pUnitStateManagerComponent->GetCurrentSpeed());
}

/******************************************************************************************************************************************************************************/
void CAIControllerComponent::MoveTo(Vec3 position)
{
	if (!m_pCharacterControllerComponent) {
		CryWarning(EValidatorModule::VALIDATOR_MODULE_GAME, EValidatorSeverity::VALIDATOR_WARNING, "CAIControllerComponent : (MoveTo) m_pCharacterControllerComponent is null.");
		return;
	}
	if (!m_pNavigationComponent) {
		CryWarning(EValidatorModule::VALIDATOR_MODULE_GAME, EValidatorSeverity::VALIDATOR_WARNING, "CAIControllerComponent : (MoveTo) m_pNavigationComponent is null.");
		return;
	}

	m_pNavigationComponent->NavigateTo(position);
}

/******************************************************************************************************************************************************************************/
void CAIControllerComponent::LookAtMovePosition()
{
	Vec3 velocity = m_pNavigationComponent->GetRequestedVelocity();
	if (velocity == ZERO) {
		return;
	}

	m_pEntity->SetRotation(Quat::CreateRotationVDir(velocity));
}

/******************************************************************************************************************************************************************************/
void CAIControllerComponent::StopMoving()
{
	this->m_pNavigationComponent->NavigateTo(m_pEntity->GetWorldPos());
}

/******************************************************************************************************************************************************************************/
void CAIControllerComponent::LookAt(Vec3 position)
{
	Vec3 dir = position - m_pEntity->GetWorldPos();
	m_pEntity->SetRotation(Quat::CreateRotationVDir(dir));
}

/******************************************************************************************************************************************************************************/
bool CAIControllerComponent::IsMoving()
{
	return m_pCharacterControllerComponent->IsWalking();
}

/******************************************************************************************************************************************************************************/
Vec3 CAIControllerComponent::GetVelocity()
{
	return m_pCharacterControllerComponent->GetVelocity();
}

/******************************************************************************************************************************************************************************/