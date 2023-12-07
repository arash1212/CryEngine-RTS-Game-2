#include "StdAfx.h"
#include "EntityCoverUser.h"
#include "GamePlugin.h"

#include <Utils/PhysicsUtils.h>
#include <Components/Controller/AIController.h>
#include <CryAISystem/ICoverSystem.h>
#include <Utils/PhysicsUtils.h>
#include <Components/Cover/CoverPosition.h>
#include <Components/Managers/UnitStateManager.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterEntityCoverUserComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CEntityCoverUserComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterEntityCoverUserComponent);
}

/******************************************************************************************************************************************************************************/
void CEntityCoverUserComponent::Initialize()
{
	//AIControllerComponent Initialization
	m_pAIControllerComponent = m_pEntity->GetComponent<CAIControllerComponent>();

	//UnitStateManagerComponent Initialization
	m_pUnitStateManagerComponent = m_pEntity->GetOrCreateComponent<CUnitStateManagerComponent>();
}

/******************************************************************************************************************************************************************************/
Cry::Entity::EventFlags CEntityCoverUserComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::Initialize |
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

/******************************************************************************************************************************************************************************/
void CEntityCoverUserComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {

		if (m_pCurrentCoverPosition) {
			if (!IsInCover()) {
				MoveToCover();
				m_isMovingToCover = true;
			}
			else {
				if (m_pCurrentCoverPosition->GetCoverHeight() < GetHeight()) {
					m_pUnitStateManagerComponent->SetStance(EUnitStance::CROUCH);
				}
				Vec3 normal = GetCurrentCoverNormal();
				if (normal != ZERO) {
					m_pAIControllerComponent->LookAt(normal);
				}
			}
		}
	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

/******************************************************************************************************************************************************************************/
void CEntityCoverUserComponent::MoveToCover()
{
	this->m_pAIControllerComponent->MoveTo(m_pCurrentCoverPosition->GetCoverPosition());
}

/******************************************************************************************************************************************************************************/
bool CEntityCoverUserComponent::IsInCover() const
{
	return m_pCurrentCoverPosition && m_pCurrentCoverPosition->GetCoverPosition().GetDistance(m_pEntity->GetWorldPos()) < 0.3f;
}

/******************************************************************************************************************************************************************************/
bool CEntityCoverUserComponent::IsMovingToCover() const
{
	return m_isMovingToCover;
}

/******************************************************************************************************************************************************************************/
bool CEntityCoverUserComponent::IsCoverCompromised() const
{
	return false;
}

/******************************************************************************************************************************************************************************/
void CEntityCoverUserComponent::SetCurrentCoverPosition(CCoverPosition* coverPosition)
{
	this->m_pCurrentCoverPosition = coverPosition;
	this->m_isMovingToCover = false;
}

/******************************************************************************************************************************************************************************/
CCoverPosition* CEntityCoverUserComponent::GetCurrentCoverPosition() const
{
	return m_pCurrentCoverPosition;
}

/******************************************************************************************************************************************************************************/
Vec3 CEntityCoverUserComponent::GetCurrentCoverNormal() const
{
	return  m_pCurrentCoverPosition ? g_PhysicsUtils->RaycastGetHitNormal(m_pEntity->GetWorldPos(), m_pCurrentCoverPosition->GetCoverObject()->GetWorldPos()) : ZERO;
}

/******************************************************************************************************************************************************************************/
f32 CEntityCoverUserComponent::GetHeight()
{
	AABB aabb;
	m_pEntity->GetWorldBounds(aabb);

	return crymath::abs(aabb.max.z - aabb.min.z);
}

/******************************************************************************************************************************************************************************/
