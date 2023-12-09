#include "StdAfx.h"
#include "EnemySpawnPoint.h"
#include "GamePlugin.h"

#include <Utils/EntityUtils.h>
#include <Components/Player/Player.h>
#include <Components/Selectables/OwnerInfo.h>
#include <Components/Units/Units/Soldier1Unit.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterEnemySpawnPointComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CEnemySpawnPointComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterEnemySpawnPointComponent);
}

/******************************************************************************************************************************************************************************/
void CEnemySpawnPointComponent::Initialize()
{
}

/******************************************************************************************************************************************************************************/
Cry::Entity::EventFlags CEnemySpawnPointComponent::GetEventMask() const
{
	return Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

/******************************************************************************************************************************************************************************/
void CEnemySpawnPointComponent::ProcessEvent(const SEntityEvent& event)
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
void CEnemySpawnPointComponent::SpawnPlayer()
{
	Vec3 vSpawnPosition = m_pEntity->GetWorldPos();
	Quat qSpawnRotation = m_pEntity->GetRotation();
	m_pEnemyPlayerEntity = g_EntityUtils->SpawnEntity(vSpawnPosition, qSpawnRotation, nullptr);
	m_pEnemyPlayerEntity->GetOrCreateComponent<CPlayerComponent>();

	SpawnUnits(m_pEnemyPlayerEntity);
}

/******************************************************************************************************************************************************************************/
void CEnemySpawnPointComponent::SpawnUnits(IEntity* ownerPlayer)
{
	for (int32 i = 0; i < 4; i++) {
		Vec3 pos = m_pEntity->GetWorldPos();
		pos.x += i * (1);
		IEntity* pSpawnedUnitEntity = g_EntityUtils->SpawnEntity(pos, m_pEntity->GetRotation(), ownerPlayer);
		pSpawnedUnitEntity->GetOrCreateComponent<CSoldier1UnitComponent>();
	}
}

/******************************************************************************************************************************************************************************/
