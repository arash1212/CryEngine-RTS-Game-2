#include "StdAfx.h"
#include "CoverUtils.h"
#include "GamePlugin.h"

#include <CryAISystem/ICoverSystem.h>
#include <CryAISystem/Components/IEntityCoverUserComponent.h>
#include <Components/Cover/EntityCoverUser.h>

#include <Components/Controller/AIController.h>
#include <Components/Player/PlayerController.h>
#include <Components/Cover/CoverPosition.h>
#include <Utils/PhysicsUtils.h>
#include <Utils/EntityUtils.h>

#include <CryEntitySystem/IEntitySystem.h>
#include <CryPhysics/physinterface.h>
#include <CryGame/IGameFramework.h>

/******************************************************************************************************************************************************************************/
void CoverUtils::FindCoverUsers()
{
	if (m_lastEntitiesSizeCheck != gEnv->pEntitySystem->GetNumEntities()) {
		m_coverUsers.clear();
		IEntityItPtr entityItPtr = gEnv->pEntitySystem->GetEntityIterator();
		entityItPtr->MoveFirst();
		while (!entityItPtr->IsEnd())
		{
			IEntity* pEntity = entityItPtr->Next();
			if (!pEntity) {
				continue;
			}
			CEntityCoverUserComponent* pEntityCoverUserComponent = pEntity->GetComponent<CEntityCoverUserComponent>();
			if (!pEntityCoverUserComponent) {
				continue;
			}

			m_coverUsers.append(pEntity);
		}

		m_lastEntitiesSizeCheck = gEnv->pEntitySystem->GetNumEntities();
		CryLog("Cover users Found : %i", m_coverUsers.size());
	}
}

/******************************************************************************************************************************************************************************/
DynArray<CCoverPosition*> CoverUtils::FindCoverPointsAroundPosition(Vec3 position, f32 maxDistance, int32 maxPoints)
{
	//DynArray<Vec3> result;
	DynArray<CCoverPosition*> result;
	DynArray<CCoverPosition*> tempResult;

	IEntityItPtr entityItPtr = gEnv->pEntitySystem->GetEntityIterator();
	entityItPtr->MoveFirst();
	while (!entityItPtr->IsEnd())
	{
		IEntity* pEntity = entityItPtr->Next();
		if (!pEntity) {
			continue;
		}
		if (pEntity->GetComponent<CPlayerControllerComponent>() || pEntity->GetComponent<CAIControllerComponent>()) {
			continue;
		}
		f32 distanceToEntity = g_EntityUtils->GetDistance(position, pEntity->GetWorldPos(), pEntity);
		if (distanceToEntity > maxDistance) {
			continue;
		}

		AABB aabb;
		pEntity->GetWorldBounds(aabb);

		Vec3 min = aabb.min;
		Vec3 max = aabb.max;
		f32 diff = 1.f;
		f32 pointDistanceToCover = 0.5f;

		f32 width = crymath::abs(max.x - min.x);
		f32 height = crymath::abs(max.y - min.y);
		width -= 0.2f;
		height -= 0.2f;

		f32 currX = min.x + 0.5f;
		while (currX <= min.x + width)
		{
			Vec3 vec1(currX, min.y - pointDistanceToCover, min.z + 1.f);
			vec1 = g_PhysicsUtils->RaycastGetHitPoint(vec1, g_EntityUtils->GetClosetPointOnMeshBorder(vec1, pEntity));
			vec1.y -= pointDistanceToCover;
			IEntity* vec1Entity = g_PhysicsUtils->RaycastGetEntity(vec1, position);
			if (!vec1Entity || vec1Entity != pEntity) {
				tempResult.append(new CCoverPosition(pEntity, vec1));
			}

			Vec3 vec2(currX, max.y + pointDistanceToCover, min.z + 1.f);
			vec2 = g_PhysicsUtils->RaycastGetHitPoint(vec2, g_EntityUtils->GetClosetPointOnMeshBorder(vec2, pEntity));
			vec2.y += pointDistanceToCover;
			IEntity* vec2Entity = g_PhysicsUtils->RaycastGetEntity(vec2, position);
			if (!vec2Entity || vec2Entity != pEntity) {
				tempResult.append(new CCoverPosition(pEntity, vec2));
			}

			currX += diff;
		}

		f32 currY = min.y + 0.5f;
		while (currY <= min.y + height)
		{
			Vec3 vec1(min.x - pointDistanceToCover, currY, min.z + 1.f);
			vec1 = g_PhysicsUtils->RaycastGetHitPoint(vec1, g_EntityUtils->GetClosetPointOnMeshBorder(vec1, pEntity));
			vec1.x -= pointDistanceToCover;
			IEntity* vec1Entity = g_PhysicsUtils->RaycastGetEntity(vec1, position);
			if (!vec1Entity || vec1Entity != pEntity) {
				tempResult.append(new CCoverPosition(pEntity, vec1));
			}

			Vec3 vec2(max.x + pointDistanceToCover, currY, min.z + 1.f);
			vec2 = g_PhysicsUtils->RaycastGetHitPoint(vec2, g_EntityUtils->GetClosetPointOnMeshBorder(vec2, pEntity));
			vec2.x += pointDistanceToCover;
			IEntity* vec2Entity = g_PhysicsUtils->RaycastGetEntity(vec2, position);
			if (!vec2Entity || vec2Entity != pEntity) {
				tempResult.append(new CCoverPosition(pEntity, vec2));
			}

			currY += diff;
		}
	}

	if (tempResult.size() > 0) {
		IPersistantDebug* pd = gEnv->pGameFramework->GetIPersistantDebug();
		pd->Begin("FindCoverPointsAroundPosition_CoverPointsFound", true);
		//Sort Points By distance
		int32 size = tempResult.size() >= maxPoints ? maxPoints : tempResult.size();
		this->SortPointsByDistance(tempResult, position, tempResult.size());
		for (int32 i = 0; i < size; i++) {
			if (IsCoverPointValid(tempResult[i])) {
				result.append(tempResult[i]);
				pd->AddSphere(tempResult[i]->GetCoverPosition(), 0.3f, ColorF(0.9f, 0.5f, 0), 5.0f);
			}
		}
	}

	return result;
}

/******************************************************************************************************************************************************************************/
void CoverUtils::SortPointsByDistance(DynArray<CCoverPosition*>& locations, Vec3 position, int32 size)
{
	for (int32 j = 0; j < size; j++) {
		for (int32 i = j; i < size; i++) {
			f32 distanceToJ = g_EntityUtils->GetDistance(position, locations[j]->GetCoverPosition());
			f32 distanceToI = g_EntityUtils->GetDistance(position, locations[i]->GetCoverPosition());
			if (distanceToI < distanceToJ) {
				std::swap(locations[i], locations[j]);
			}
		}
	}
}

/******************************************************************************************************************************************************************************/
bool CoverUtils::IsCoverPointValid(CCoverPosition* point)
{
	for (IEntity* entity : m_coverUsers) {
		CEntityCoverUserComponent* pEntityCoverUserComponent = entity->GetComponent<CEntityCoverUserComponent>();
		CCoverPosition* pCoverPosition = pEntityCoverUserComponent->GetCurrentCoverPosition();
		if (!pCoverPosition) {
			continue;
		}
		f32 distanceToUser = g_EntityUtils->GetDistance(pCoverPosition->GetCoverPosition(), point->GetCoverPosition());
		if (distanceToUser < 1) {
			return false;
		}
	}
	return true;
}

/******************************************************************************************************************************************************************************/