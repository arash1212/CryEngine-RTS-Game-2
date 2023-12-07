#include "StdAfx.h"
#include "CoverUtils.h"
#include "GamePlugin.h"

#include <CryAISystem/ICoverSystem.h>
#include <CryAISystem/Components/IEntityCoverUserComponent.h>


#include <Components/Controller/AIController.h>
#include <Components/Player/Player.h>
#include <Components/Cover/CoverPosition.h>
#include <Utils/PhysicsUtils.h>

#include <CryEntitySystem/IEntitySystem.h>
#include <CryPhysics/physinterface.h>
#include <CryGame/IGameFramework.h>


DynArray<CCoverPosition*> CoverUtils::FindCoverPointsAroundPosition(Vec3 position, f32 maxDistance, int32 maxPoints)
{
	//DynArray<Vec3> result;
	DynArray<CCoverPosition*> result;

	IEntityItPtr entityItPtr = gEnv->pEntitySystem->GetEntityIterator();
	entityItPtr->MoveFirst();
	while (!entityItPtr->IsEnd() && result.size() < maxPoints)
	{
		IEntity* pEntity = entityItPtr->Next();
		if (!pEntity) {
			continue;
		}
		if (pEntity->GetComponent<CPlayerComponent>() || pEntity->GetComponent<CAIControllerComponent>()) {
			continue;
		}
		f32 distanceToEntity = position.GetDistance(pEntity->GetWorldPos());
		if (distanceToEntity > maxDistance) {
			continue;
		}

		AABB aabb;
		pEntity->GetWorldBounds(aabb);

		Vec3 min = aabb.min;
		Vec3 max = aabb.max;
		f32 diff = 1.f;
		f32 pointDistanceToCover = 0.5f;

		IPersistantDebug* pd = gEnv->pGameFramework->GetIPersistantDebug();
		if (pd) {
			pd->Begin("coverPointsTest", true);
			f32 width = crymath::abs(max.x - min.x);
			f32 height = crymath::abs(max.y - min.y);
			width -= 0.2f;
			height -= 0.2f;

			f32 currX = min.x + 0.2f;
			while (currX <= min.x + width)
			{
				if (result.size() >= maxPoints) {
					break;
				}

				Vec3 vec1(currX, min.y - pointDistanceToCover, min.z);
				IEntity* vec1Entity = g_PhysicsUtils->RaycastGetEntnity(vec1, position);
				if (!vec1Entity || vec1Entity != pEntity) {
					pd->AddSphere(vec1, 0.2f, ColorF(0, 1, 0), 0.5f);
					result.append(new CCoverPosition(pEntity, vec1));
				}

				Vec3 vec2(currX, max.y + pointDistanceToCover, min.z);
				IEntity* vec2Entity = g_PhysicsUtils->RaycastGetEntnity(vec2, position);
				if (!vec2Entity || vec2Entity != pEntity) {
					pd->AddSphere(vec2, 0.2f, ColorF(0, 1, 0), 0.5f);
					result.append(new CCoverPosition(pEntity, vec2));
				}

				currX += diff;
			}

			f32 currY = min.y + 0.2f;
			while (currY <= min.y + height)
			{
				if (result.size() >= maxPoints) {
					break;
				}

				Vec3 vec1(min.x - pointDistanceToCover, currY, min.z);
				IEntity* vec1Entity = g_PhysicsUtils->RaycastGetEntnity(vec1, position);
				if (!vec1Entity || vec1Entity != pEntity) {
					pd->AddSphere(vec1, 0.2f, ColorF(0, 1, 0), 0.5f);
					result.append(new CCoverPosition(pEntity, vec1));
				}

				Vec3 vec2(max.x + pointDistanceToCover, currY, min.z);
				IEntity* vec2Entity = g_PhysicsUtils->RaycastGetEntnity(vec2, position);
				if (!vec2Entity || vec2Entity != pEntity) {
					pd->AddSphere(vec2, 0.2f, ColorF(0, 1, 0), 0.5f);
					result.append(new CCoverPosition(pEntity, vec2));
				}

				currY += diff;
			}

		}
	}

	return result;
}
