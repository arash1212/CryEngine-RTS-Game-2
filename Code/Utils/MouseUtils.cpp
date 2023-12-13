#include "StdAfx.h"
#include "MouseUtils.h"
#include "GamePlugin.h"

#include <CryGame/IGameFramework.h>
#include <CryInput/IHardwareMouse.h>
#include <CryPhysics/physinterface.h>

/******************************************************************************************************************************************************************************/
Vec3 MouseUtils::GetPositionUnderCursor()
{
	Vec2 mousePos;
	gEnv->pHardwareMouse->GetHardwareMouseClientPosition(&mousePos.x, &mousePos.y);
	mousePos.y = gEnv->pRenderer->GetHeight() - mousePos.y;

	Vec3 p1;
	gEnv->pRenderer->UnProjectFromScreen(mousePos.x, mousePos.y, 0, &p1.x, &p1.y, &p1.z);

	Vec3 p2;
	gEnv->pRenderer->UnProjectFromScreen(mousePos.x, mousePos.y, 1, &p2.x, &p2.y, &p2.z);

	Vec3 dir = p2 - p1;

	unsigned int flags = rwi_colltype_any | rwi_stop_at_pierceable;
	ray_hit hit;

	IPersistantDebug* pd = gEnv->pGameFramework->GetIPersistantDebug();
	if (gEnv->pPhysicalWorld->RayWorldIntersection(p1, dir * gEnv->p3DEngine->GetMaxViewDistance(), ent_all, flags, &hit, 1)) {
		pd->Begin("MouseUtils_GetPositionUnderCursor_Debug", true);
		pd->AddSphere(hit.pt, 0.1f, ColorF(0, 1, 0), 0.5f);
		return hit.pt;
	}
	return ZERO;
}

/******************************************************************************************************************************************************************************/
Vec2 MouseUtils::GetCursorScreenPosition()
{
	Vec2 result;
	gEnv->pHardwareMouse->GetHardwareMouseClientPosition(&result.x, &result.y);
	return result;
}

/******************************************************************************************************************************************************************************/
IEntity* MouseUtils::GetEntityUnderCursor()
{
	IHardwareMouse* pHardwareMouse = gEnv->pHardwareMouse;

	Vec2 currentPos;
	pHardwareMouse->GetHardwareMouseClientPosition(&currentPos.x, &currentPos.y);
	currentPos.y = gEnv->pRenderer->GetHeight() - currentPos.y;

	Vec3 p1;
	gEnv->pRenderer->UnProjectFromScreen(currentPos.x, currentPos.y, 0, &p1.x, &p1.y, &p1.z);

	Vec3 p2;
	gEnv->pRenderer->UnProjectFromScreen(currentPos.x, currentPos.y, 1, &p2.x, &p2.y, &p2.z);

	Vec3 dir = p2 - p1;

	int objTypes = ent_all;
	int flags = rwi_colltype_any | rwi_stop_at_pierceable;
	ray_hit hit;

	if (gEnv->pPhysicalWorld->RayWorldIntersection(p1, dir, objTypes, flags, &hit, 1)) {
		if (hit.pCollider) {
			IEntity* hitEntity = gEnv->pEntitySystem->GetEntityFromPhysics(hit.pCollider);
			return hitEntity;
		}
	}

	return nullptr;
}

/******************************************************************************************************************************************************************************/