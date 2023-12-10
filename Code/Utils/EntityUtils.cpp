#include "StdAfx.h"
#include "EntityUtils.h"
#include "GamePlugin.h"

#include <Components/Selectables/OwnerInfo.h>
#include <Components/Player/Player.h>

#include <CryEntitySystem/IEntitySystem.h>
#include <CryGame/IGameFramework.h>

/******************************************************************************************************************************************************************************/
IEntity* EntityUtils::SpawnEntity(Vec3 position, Quat rotation, IEntity* ownerEntity)
{
	SEntitySpawnParams pEntitySpawnParams;
	pEntitySpawnParams.vPosition = position;
	pEntitySpawnParams.qRotation = rotation;
	IEntity* pSpawnedEntity = gEnv->pEntitySystem->SpawnEntity(pEntitySpawnParams, true);

	if (ownerEntity) {
		COwnerInfoComponent* pOwnerInfoComponent = ownerEntity->GetComponent<COwnerInfoComponent>();

		COwnerInfoComponent* pSpawnedEntityOwnerInfoComponent = pSpawnedEntity->GetOrCreateComponent<COwnerInfoComponent>();
		pSpawnedEntityOwnerInfoComponent->SetOwnerInfo(pOwnerInfoComponent->GetOwnerInfo());
	}

	CryLog("EntityUtils : (SpawnEntity) New Entity Spawned");
	return pSpawnedEntity;
}

/******************************************************************************************************************************************************************************/
Vec3 EntityUtils::GetClosetPointOnMeshBorder(Vec3 from, IEntity* entity)
{
	if (entity) {
		AABB aabb;
		entity->GetWorldBounds(aabb);
		Vec3 to = entity->GetWorldPos();

		f32 diffX = aabb.max.x - aabb.GetCenter().x;
		f32 diffY = aabb.max.y - aabb.GetCenter().y;

		Vec3 pos = to;
		//X
		if (from.x > pos.x + diffX) {
			pos.x += diffX;
		}
		else if (from.x < pos.x - diffX) {
			pos.x -= diffX;
		}
		else {
			pos.x = from.x;
		}

		//Y
		if (from.y > pos.y + diffY) {
			pos.y += diffY;
		}
		else if (from.y < pos.y - diffY) {
			pos.y -= diffY;
		}
		else {
			pos.y = from.y;
		}
		return pos;
	}
	else {
		return ZERO;
	}
}

/******************************************************************************************************************************************************************************/
IEntity* EntityUtils::GetClosestEntity(DynArray<IEntity*> entities, Vec3 to)
{
	f32 closest = 10000;
	IEntity* result = nullptr;
	for (IEntity* entity : entities) {
		f32 distance = to.GetDistance(entity->GetWorldPos());
		if (distance < closest) {
			closest = distance;
			result = entity;
		}
	}
	return result;
}

/******************************************************************************************************************************************************************************/
f32 EntityUtils::GetDistance(Vec3 from, Vec3 to, IEntity* toEntity)
{
	if (toEntity) {
		AABB aabb;
		toEntity->GetWorldBounds(aabb);
		Vec3 pos = GetClosetPointOnMeshBorder(from, toEntity);
		return from.GetDistance(pos);
	}
	else {
		return from.GetDistance(to);
	}
}

/******************************************************************************************************************************************************************************/
f32 EntityUtils::GetDistance(Vec3 from, Vec3 to)
{
	return from.GetDistance(to);
}

/******************************************************************************************************************************************************************************/
DynArray<IEntity*> EntityUtils::FindHostilePlayers(IEntity* toEntity)
{
	COwnerInfoComponent* pOwnerInfoComponent = toEntity->GetComponent<COwnerInfoComponent>();

	DynArray<IEntity*> hostiles;
	IEntityItPtr entityItPtr = gEnv->pEntitySystem->GetEntityIterator();
	entityItPtr->MoveFirst();
	while (!entityItPtr->IsEnd())
	{
		IEntity* pEntity = entityItPtr->Next();
		if (!pEntity) {
			continue;
		}
		CPlayerComponent* pPlayerComponent = pEntity->GetComponent<CPlayerComponent>();
		if (!pPlayerComponent) {
			continue;
		}
		COwnerInfoComponent* pOtherOwnerInfoComponent = pEntity->GetComponent<COwnerInfoComponent>();
		if (!pOtherOwnerInfoComponent) {
			continue;
		}
		if (pOtherOwnerInfoComponent->GetOwnerInfo().m_pPlayerTeam == pOwnerInfoComponent->GetOwnerInfo().m_pPlayerTeam) {
			continue;
		}
		hostiles.append(pEntity);
	}

	return hostiles;
}

/******************************************************************************************************************************************************************************/
void EntityUtils::SortEntitiesByDistance(DynArray<IEntity*>& entities, Vec3 position, int32 size)
{
	for (int32 i = 0; i < size; i++) {
		for (int32 j = i; j < size; j++) {
			f32 distanceToJ = g_EntityUtils->GetDistance(position, entities[j]->GetWorldPos());
			f32 distanceToI = g_EntityUtils->GetDistance(position, entities[i]->GetWorldPos());
			if (distanceToI < distanceToJ) {
				std::swap(entities[i], entities[j]);
			}
		}
	}
}

/******************************************************************************************************************************************************************************/
