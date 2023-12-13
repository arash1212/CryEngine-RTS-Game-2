#include "StdAfx.h"
#include "EntityCoverUser.h"
#include "GamePlugin.h"

#include <Utils/EntityUtils.h>
#include <Utils/PhysicsUtils.h>
#include <Components/Controller/AIController.h>
#include <CryAISystem/ICoverSystem.h>
#include <Utils/PhysicsUtils.h>
#include <Components/Cover/CoverPosition.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Units/Attacker.h>
#include <Components/Cover/CoverPosition.h>

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

	//AttackerComponent Initialization
	m_pAttackerComponent = m_pEntity->GetOrCreateComponent<CAttackerComponent>();
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
		f32 DeltaTime = event.fParam[0];

		//TODO : CLEAN
		if (m_pCurrentCoverPosition) {
			if (!IsInCover()) {
				MoveToCover();
				m_isMovingToCover = true;
			}
			else {
				m_pEntity->SetPos(m_pCurrentCoverPosition->GetCoverPosition());

				if (m_pCurrentCoverPosition->GetCoverHeight() < GetHeight()) {
					//
					if (m_pAttackerComponent->IsAttacking() && m_changeStanceTimePassed >= m_timeBetweenChangingStance) {
						m_pUnitStateManagerComponent->SetStance(EUnitStance::CROUCH);
						m_changeStanceTimePassed = 0;
					}
					else if (m_pAttackerComponent->IsAttacking() && m_changeStanceTimePassed < m_timeBetweenChangingStance) {
						m_changeStanceTimePassed += 0.5f * DeltaTime;
					}

					//
					if (!m_pAttackerComponent->IsAttacking() && m_changeStanceTimePassed >= m_timeBetweenChangingStance)
					{
						m_pUnitStateManagerComponent->SetStance(EUnitStance::COVER);
						m_changeStanceTimePassed = 0;
					}
					else if (!m_pAttackerComponent->IsAttacking() && m_changeStanceTimePassed < m_timeBetweenChangingStance) {
						m_changeStanceTimePassed += 0.5f * DeltaTime;
					}
				}

				if (!m_isStanceSet) {
					Vec3 normal = GetCurrentCoverNormal();
					m_pAIControllerComponent->LookAt(normal);
					m_isStanceSet = true;
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
	this->m_pAIControllerComponent->LookAtMovePosition();
}

/******************************************************************************************************************************************************************************/
bool CEntityCoverUserComponent::IsInCover() const
{
	return m_pCurrentCoverPosition&& g_EntityUtils->GetDistance(m_pCurrentCoverPosition->GetCoverPosition(), m_pEntity->GetWorldPos()) < 0.3f;
}

/******************************************************************************************************************************************************************************/
bool CEntityCoverUserComponent::IsMovingToCover() const
{
	return m_isMovingToCover;
}

/******************************************************************************************************************************************************************************/
bool CEntityCoverUserComponent::IsCoverCompromised() const
{
	//TODO
	return false;
}

/******************************************************************************************************************************************************************************/
void CEntityCoverUserComponent::SetCurrentCoverPosition(CCoverPosition* coverPosition)
{
	this->m_pCurrentCoverPosition = coverPosition;
	this->m_isMovingToCover = false;
	this->m_isStanceSet = false;
}

/******************************************************************************************************************************************************************************/
CCoverPosition* CEntityCoverUserComponent::GetCurrentCoverPosition() const
{
	return m_pCurrentCoverPosition;
}

/******************************************************************************************************************************************************************************/
Vec3 CEntityCoverUserComponent::GetCurrentCoverNormal() const
{
	return  m_pCurrentCoverPosition ? g_EntityUtils->GetClosetPointOnMeshBorder(m_pCurrentCoverPosition->GetCoverPosition(), m_pCurrentCoverPosition->GetCoverObject()) : ZERO;
}

/******************************************************************************************************************************************************************************/
f32 CEntityCoverUserComponent::GetHeight()
{
	AABB aabb;
	m_pEntity->GetWorldBounds(aabb);

	return crymath::abs(aabb.max.z - aabb.min.z);
}

/******************************************************************************************************************************************************************************/
