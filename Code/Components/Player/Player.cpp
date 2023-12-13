#include "StdAfx.h"
#include "Player.h"
#include "GamePlugin.h"

#include <Actions/IBaseAction.h>
#include <Components/Selectables/OwnerInfo.h>
#include <Components/Units/BaseAIUnit.h>
#include <Components/Cover/EntityCoverUser.h>
#include <Components/Cover/CoverPosition.h>

#include <Utils/CoverUtils.h>

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
	//OwnerInfoComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetOrCreateComponent<COwnerInfoComponent>();
	m_pOwnerInfoComponent->SetCanBeTarget(false);
	//
	SOwnerInfo pOwnerInfo;
	pOwnerInfo.m_pPlayer = EPlayer::PLAYER2;
	pOwnerInfo.m_pPlayerFaction = EPlayerFaction::FACTION_1;
	pOwnerInfo.m_pPlayerTeam = EPlayerTeam::TEAM_2;
	pOwnerInfo.m_pPlayerComponent = this;
	m_pOwnerInfoComponent->SetOwnerInfo(pOwnerInfo);
}

/******************************************************************************************************************************************************************************/
Cry::Entity::EventFlags CPlayerComponent::GetEventMask() const
{
	return
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
		//f32 DeltaTime = event.fParam[0];
		CommandUnitsToTakeCover();

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

/******************************************************************************************************************************************************************************/
bool CPlayerComponent::IsAI()
{
	return m_isAI;
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::SetIsAI(bool isAI)
{
	this->m_isAI = isAI;
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::AddOwnedEntity(IEntity* entity)
{
	m_ownedEntities.append(entity);
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::RemoveOwnedEntity(IEntity* entity)
{
	DynArray<IEntity*> result;
	for (IEntity* pEntity : m_ownedEntities) {
		if (pEntity == entity) {
			continue;
		}
		result.append(pEntity);
	}

	m_ownedEntities = result;
}

/******************************************************************************************************************************************************************************/
DynArray<IEntity*> CPlayerComponent::GetOwnedEntities()
{
	return m_ownedEntities;
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::CommandUnitsToTakeCover()
{
	if (!m_isAI) {
		return;
	}
	for (int32 i = 0; i < m_ownedEntities.size(); i++) {
		CBaseAIUnitComponent* pAIUnitComponent = m_ownedEntities[i]->GetComponent<CBaseAIUnitComponent>();
		if (!pAIUnitComponent) {
			continue;
		}
		CEntityCoverUserComponent* pEntityCoverUserComponent = m_ownedEntities[i]->GetComponent<CEntityCoverUserComponent>();
		if (!pEntityCoverUserComponent) {
			continue;
		}
		if (pEntityCoverUserComponent->GetCurrentCoverPosition()) {
			continue;
		}

		DynArray<CCoverPosition*> pCoverPositions = g_CoverUtils->FindCoverPointsAroundPosition(m_ownedEntities[i]->GetWorldPos() , m_ownedEntities[i], 30, m_ownedEntities.size());
		pAIUnitComponent->MoveToCover(pCoverPositions[0]);
	}
}

/******************************************************************************************************************************************************************************/