#include "StdAfx.h"
#include "PhysicsUtils.h"
#include "GamePlugin.h"

#include <CryEntitySystem/IEntitySystem.h>
#include <CryPhysics/physinterface.h>
#include <CryGame/IGameFramework.h>

/******************************************************************************************************************************************************************************/
IEntity* PhysicsUtils::RaycastGetEntnity(Vec3 from, Vec3 to)
{
	Vec3 origin = from;
	Vec3 dir = to - from;
	dir = dir.normalized();

	int flags = rwi_colltype_any | rwi_stop_at_pierceable;
	ray_hit hit;
	if (gEnv->pPhysicalWorld->RayWorldIntersection(origin, dir * gEnv->p3DEngine->GetMaxViewDistance(), ent_all, flags, &hit, 1)) {
		if (hit.pCollider) {
			IEntity* pEntity = gEnv->pEntitySystem->GetEntityFromPhysics(hit.pCollider);
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
			hit.n;
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
	if (gEnv->pPhysicalWorld->RayWorldIntersection(origin, dir * gEnv->p3DEngine->GetMaxViewDistance(), ent_all, flags, &hit, 1)) {
		hit.pt;
	}
	return ZERO;
}

/******************************************************************************************************************************************************************************/
