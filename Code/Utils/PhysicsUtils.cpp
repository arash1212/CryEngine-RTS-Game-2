#include "StdAfx.h"
#include "PhysicsUtils.h"
#include "GamePlugin.h"

#include <CryEntitySystem/IEntitySystem.h>
#include <CryPhysics/physinterface.h>
#include <CryGame/IGameFramework.h>

/******************************************************************************************************************************************************************************/
IEntity* PhysicsUtils::RaycastGetEntity(Vec3 from, Vec3 to)
{
	Vec3 origin = from;
	Vec3 dir = to - from;
	dir = dir.normalized();

	int flags = rwi_colltype_any | rwi_stop_at_pierceable;
	ray_hit hit;
	//IPersistantDebug* pd = gEnv->pGameFramework->GetIPersistantDebug();
	//pd->Begin("PhysicsUtils_RaycastGetEntity", false);
	if (gEnv->pPhysicalWorld->RayWorldIntersection(origin, dir * gEnv->p3DEngine->GetMaxViewDistance(), ent_all, flags, &hit, 1)) {
		if (hit.pCollider) {
			IEntity* pEntity = gEnv->pEntitySystem->GetEntityFromPhysics(hit.pCollider);

			//pd->AddSphere(hit.pt, 0.3f, ColorF(1, 0, 0), 1.f);
			return pEntity;
		}
	}
	return nullptr;
}

/******************************************************************************************************************************************************************************/
Vec3 PhysicsUtils::RaycastGetHitNormal(Vec3 from, Vec3 to)
{
	Vec3 origin = from;
	Vec3 dir = to - from;
	dir = dir.normalized();

	int flags = rwi_colltype_any | rwi_stop_at_pierceable;
	ray_hit hit;
	if (gEnv->pPhysicalWorld->RayWorldIntersection(origin, dir * gEnv->p3DEngine->GetMaxViewDistance(), ent_all, flags, &hit, 1)) {
		if (hit.pCollider) {
			return hit.n;
		}
	}
	return ZERO;
}

/******************************************************************************************************************************************************************************/
Vec3 PhysicsUtils::RaycastGetHitPoint(Vec3 from, Vec3 to)
{
	Vec3 origin = from;
	Vec3 dir = to - from;
	dir = dir.normalized();

	int flags = rwi_colltype_any | rwi_stop_at_pierceable;
	ray_hit hit;
	//IPersistantDebug* pd = gEnv->pGameFramework->GetIPersistantDebug();
	if (gEnv->pPhysicalWorld->RayWorldIntersection(origin, dir * gEnv->p3DEngine->GetMaxViewDistance(), ent_all, flags, &hit, 1)) {
		return hit.pt;
	}
	return ZERO;
}

/******************************************************************************************************************************************************************************/
