#include "StdAfx.h"
#include "EntityUtils.h"
#include "GamePlugin.h"

#include <CryEntitySystem/IEntitySystem.h>
#include <CryGame/IGameFramework.h>

IEntity* EntityUtils::SpawnEntity(Vec3 position, Quat rotation)
{
	SEntitySpawnParams pEntitySpawnParams;
	pEntitySpawnParams.vPosition = position;
	pEntitySpawnParams.qRotation = rotation;
	IEntity* pSpawnedEntity = gEnv->pEntitySystem->SpawnEntity(pEntitySpawnParams, true);
	CryLog("EntityUtils : (SpawnEntity) New Entity Spawned");
	return pSpawnedEntity;
}

Vec3 EntityUtils::GetClosetPointOnMeshBorder(Vec3 from, IEntity* entity)
{
	if (entity) {
		AABB aabb;
		entity->GetWorldBounds(aabb);
		Vec3 to = entity->GetWorldPos();

		IPersistantDebug* pd = gEnv->pGameFramework->GetIPersistantDebug();
		pd->Begin("tessadadsadt1231", true);

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
		pd->AddSphere(pos, 0.4f, ColorF(0, 1, 0), 0.4f);
		return pos;
	}
	else {
		return ZERO;
	}
}

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

f32 EntityUtils::GetDistance(Vec3 from, Vec3 to)
{
	return from.GetDistance(to);
}
