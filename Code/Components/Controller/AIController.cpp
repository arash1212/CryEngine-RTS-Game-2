#include "StdAfx.h"
#include "AIController.h"
#include "GamePlugin.h"

#include <CryAISystem/Components/IEntityNavigationComponent.h>
#include <Components/Managers/UnitStateManager.h>
#include <CryAISystem/Components/IEntityCoverUserComponent.h>
#include <Components/Cover/EntityCoverUser.h>
#include <CryAISystem/IAISystem.h>
#include <CryAISystem/INavigationSystem.h>
#include <CryAISystem/NavigationSystem/INavMeshQueryFilter.h>

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
	IEntityNavigationComponent::SMovementProperties pMovementProps;
	pMovementProps.normalSpeed = 1.f;
	pMovementProps.minSpeed = 2;
	pMovementProps.maxSpeed = 3;
	pMovementProps.lookAheadDistance = 1.f;
	pMovementProps.bStopAtEnd = true;
	m_pNavigationComponent->SetMovementProperties(pMovementProps);

	//Collision avoidance
	IEntityNavigationComponent::SCollisionAvoidanceProperties pCollisionAvoidanceProperties;
	pCollisionAvoidanceProperties.radius = 0.3f;
	pCollisionAvoidanceProperties.type = IEntityNavigationComponent::SCollisionAvoidanceProperties::EType::Active;
	m_pNavigationComponent->SetCollisionAvoidanceProperties(pCollisionAvoidanceProperties);

	//UnitStateManagerComponen Initialization
	m_pUnitStateManagerComponent = m_pEntity->GetOrCreateComponent<CUnitStateManagerComponent>();

	//EntityCoverUserComponent Initialization
	m_pEntityCoverUserComponent = m_pEntity->GetComponent<CEntityCoverUserComponent>();
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

	Vec3 pos = SnapToNavmesh(position);
	m_pNavigationComponent->NavigateTo(pos);
	IPersistantDebug* pd = gEnv->pGameFramework->GetIPersistantDebug();
	pd->Begin("CommandSelectedUnitsToMoveTo_MovePoints123", false);
	pd->AddSphere(pos, 0.3f, ColorF(0, 1, 1), 3);
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
	dir.z = 0;
	m_pEntity->SetRotation(Quat::CreateRotationVDir(dir));
}

/******************************************************************************************************************************************************************************/
Vec3 CAIControllerComponent::SnapToNavmesh(Vec3 point)
{
	NavigationAgentTypeID agentTypeId = NavigationAgentTypeID::TNavigationID(1);
	NavigationMeshID navMeshId = gEnv->pAISystem->GetNavigationSystem()->FindEnclosingMeshID(agentTypeId, point);
	MNM::SOrderedSnappingMetrics snappingMetrics;
	snappingMetrics.CreateDefault();
	MNM::SSnappingMetric metric;
	metric.type = MNM::SSnappingMetric::EType::Box;
	metric.horizontalRange = 2000.f;
	metric.verticalUpRange = 2000.f;
	metric.verticalDownRange = 2000.f;
	snappingMetrics.AddMetric(metric);
	SAcceptAllQueryTrianglesFilter filter;
	MNM::SPointOnNavMesh pointOnNavMesh = gEnv->pAISystem->GetNavigationSystem()->SnapToNavMesh(agentTypeId, point, snappingMetrics, &filter, &navMeshId);
	return pointOnNavMesh.GetWorldPosition();
}

/******************************************************************************************************************************************************************************/
bool CAIControllerComponent::IsDestinationReachable(Vec3 destination)
{
	return m_pNavigationComponent->IsDestinationReachable(destination);
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