#pragma once

#include "Actions/IBaseAction.h"

class CUnitStateManagerComponent;

class UnitMoveAction : public IBaseAction{

public:
	UnitMoveAction(IEntity* entity, Vec3 movePosition, bool isRunning);
	~UnitMoveAction() = default;

protected:
	IEntity* m_pEntity = nullptr;
	CUnitStateManagerComponent* m_pUnitStateManagerComponent = nullptr;

	bool m_isDone = false;
	bool m_isRunning = false;
	bool m_navigationRequestDone = false;

	Vec3 m_movePosition = ZERO;

public:
	virtual void Execute() override;
	virtual void Cancel() override;
	virtual bool IsDone() override;
	virtual bool CanBeSkipped() override;
	virtual f32 GetProgressAmount() override;
	virtual f32 GetMaxProgressAmount() override;
};