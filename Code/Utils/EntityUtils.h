#pragma once

#include <DefaultComponents/Cameras/CameraComponent.h>

class EntityUtils {

public:
	EntityUtils() = default;
	~EntityUtils() = default;
private:

public:
	IEntity* SpawnEntity(Vec3 position, Quat rotation, IEntity* ownerEntity);
	Vec3 GetClosetPointOnMeshBorder(Vec3 from, IEntity* entity);
	IEntity* GetClosestEntity(DynArray<IEntity*> entities, Vec3 to);
	f32 GetDistance(Vec3 from, Vec3 to, IEntity* toEntity);
	f32 GetDistance(Vec3 from, Vec3 to);
	DynArray<IEntity*> FindHostilePlayers(IEntity* toEntity);
	void SortEntitiesByDistance(DynArray<IEntity*>& entities, Vec3 position, int32 size);
	bool IsEntityInsideViewPort(Cry::DefaultComponents::CCameraComponent* camera, IEntity* entity);
	void RemoveEntity(IEntity* entity);
};

static EntityUtils* g_EntityUtils = new EntityUtils();