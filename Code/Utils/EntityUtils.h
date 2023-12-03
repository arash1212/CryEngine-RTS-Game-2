#pragma once

class EntityUtils {

public:
	EntityUtils() = default;
	~EntityUtils() = default;
private:

public:
	IEntity* SpawnEntity(Vec3 position, Quat rotation);
};

static EntityUtils* g_EntityUtils = new EntityUtils();