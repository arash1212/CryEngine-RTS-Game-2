#pragma once

class CCoverPosition;

class CoverUtils {
public:
	CoverUtils() = default;
	~CoverUtils() = default;

public:
	DynArray<CCoverPosition*> FindCoverPointsAroundPosition(Vec3 position, f32 maxDistance, int32 maxPoints);
};

static CoverUtils* g_CoverUtils = new CoverUtils();