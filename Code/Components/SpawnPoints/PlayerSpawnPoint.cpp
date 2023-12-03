#include "StdAfx.h"
#include "PlayerSpawnPoint.h"
#include "GamePlugin.h"

#include "Utils/EntityUtils.h"
#include "Components/Player/Player.h"

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterPlayerSpawnPointComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPlayerSpawnPointComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPlayerSpawnPointComponent);
}

/******************************************************************************************************************************************************************************/
void CPlayerSpawnPointComponent::Initialize()
{
}

/******************************************************************************************************************************************************************************/
Cry::Entity::EventFlags CPlayerSpawnPointComponent::GetEventMask() const
{
	return Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

/******************************************************************************************************************************************************************************/
void CPlayerSpawnPointComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {
		SpawnPlayer();

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
void CPlayerSpawnPointComponent::SpawnPlayer()
{
	Vec3 vSpawnPosition = m_pEntity->GetWorldPos();
	Quat qSpawnRotation = m_pEntity->GetRotation();
	m_pPlayerEntity = g_EntityUtils->SpawnEntity(vSpawnPosition, qSpawnRotation);
	m_pPlayerEntity->GetOrCreateComponent<CPlayerComponent>();
}

/******************************************************************************************************************************************************************************/
