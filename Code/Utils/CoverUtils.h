#pragma once

class CoverUtils {
public:
	CoverUtils() = default;
	~CoverUtils() = default;

public:
	DynArray<Vec3> FindCoverPointsAroundPosition(Vec3 position, f32 maxDistance, int32 maxPoints);
	IEntity* Raycast(Vec3 from, Vec3 to);
};

static CoverUtils* g_CoverUtils = new CoverUtils();