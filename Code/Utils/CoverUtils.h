#pragma once

class CCoverPosition;
class CEntityCoverUserComponent;

class CoverUtils {
public:
	CoverUtils() = default;
	~CoverUtils() = default;

private:
	int32 m_lastEntitiesSizeCheck = -1;
	DynArray<IEntity*> m_coverUsers;

private:

public:
	DynArray<CCoverPosition*> FindCoverPointsAroundPosition(Vec3 position, IEntity* entity, f32 maxDistance, int32 maxPoints);
	void SortPointsByDistance(DynArray<CCoverPosition*>& locations, Vec3 position, int32 size);

	void FindCoverUsers();
	bool IsCoverPointValid(IEntity* entity, CCoverPosition* point);
};

static CoverUtils* g_CoverUtils = new CoverUtils();