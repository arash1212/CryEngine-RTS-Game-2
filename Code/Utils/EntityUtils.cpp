#include "StdAfx.h"
#include "EntityUtils.h"
#include "GamePlugin.h"

#include <CryEntitySystem/IEntitySystem.h>

IEntity* EntityUtils::SpawnEntity(Vec3 position, Quat rotation)
{
	SEntitySpawnParams pEntitySpawnParams;
	pEntitySpawnParams.vPosition = position;
	pEntitySpawnParams.qRotation = rotation;
	IEntity* pSpawnedEntity = gEnv->pEntitySystem->SpawnEntity(pEntitySpawnParams, true);
	CryLog("EntityUtils : (SpawnEntity) New Entity Spawned");
	return pSpawnedEntity;
}
