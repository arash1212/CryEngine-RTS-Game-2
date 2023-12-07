#pragma once

class CCoverPosition {
public :
	CCoverPosition(IEntity* coverObject, Vec3 coverPosition);
private:
	IEntity* m_pCoverObject = nullptr;
	Vec3 m_coverPosition = ZERO;

public:
	IEntity* GetCoverObject() const;
	Vec3 GetCoverPosition() const;
	f32 GetCoverHeight();
};