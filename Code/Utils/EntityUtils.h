#pragma once

class EntityUtils {

public:
	EntityUtils() = default;
	~EntityUtils() = default;
private:

public:
	IEntity* SpawnEntity(Vec3 position, Quat rotation);
	Vec3 GetClosetPointOnMeshBorder(Vec3 from, IEntity* entity);
	IEntity* GetClosestEntity(DynArray<IEntity*> entities, Vec3 to);
	f32 GetDistance(Vec3 from, Vec3 to, IEntity* toEntity);
	f32 GetDistance(Vec3 from, Vec3 to);
};

static EntityUtils* g_EntityUtils = new EntityUtils();