#include "StdAfx.h"
#include "OwnerInfo.h"
#include "GamePlugin.h"

#include <Components/Player/Player.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterOwnerInfoComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(COwnerInfoComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterOwnerInfoComponent);
}

/******************************************************************************************************************************************************************************/
void COwnerInfoComponent::Initialize()
{

}

/******************************************************************************************************************************************************************************/
Cry::Entity::EventFlags COwnerInfoComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::Initialize |
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

/******************************************************************************************************************************************************************************/
void COwnerInfoComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

/******************************************************************************************************************************************************************************/
SOwnerInfo COwnerInfoComponent::GetOwnerInfo()
{
	return m_pOwnerInfo;
}

/******************************************************************************************************************************************************************************/
void COwnerInfoComponent::SetOwnerInfo(SOwnerInfo ownerInfo)
{
	this->m_pOwnerInfo.m_pPlayer = ownerInfo.m_pPlayer;
	this->m_pOwnerInfo.m_pPlayerTeam = ownerInfo.m_pPlayerTeam;
	this->m_pOwnerInfo.m_pPlayerFaction = ownerInfo.m_pPlayerFaction;
	this->m_pOwnerInfo.m_pPlayerComponent = ownerInfo.m_pPlayerComponent;

	this->m_pOwnerInfo.m_pPlayerComponent->AddOwnedEntity(m_pEntity);
}

/******************************************************************************************************************************************************************************/
void COwnerInfoComponent::SetCanBeTarget(bool canBeTarget)
{
	this->m_canBeTarget = canBeTarget;
}

/******************************************************************************************************************************************************************************/
bool COwnerInfoComponent::CanBeTarget()
{
	return m_canBeTarget;
}

/******************************************************************************************************************************************************************************/