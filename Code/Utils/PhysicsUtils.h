#pragma once

class PhysicsUtils {
public:
	PhysicsUtils() = default;
	~PhysicsUtils() = default;

public:
	IEntity* RaycastGetEntnity(Vec3 from, Vec3 to);
	Vec3 RaycastGetHitNormal(Vec3 from, Vec3 to);
	Vec3 RaycastGetHitPoint(Vec3 from, Vec3 to);
};

static PhysicsUtils* g_PhysicsUtils = new PhysicsUtils();
