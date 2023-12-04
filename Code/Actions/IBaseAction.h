#pragma once

class IBaseAction {

protected:
	IEntity* m_pEntity = nullptr;
	bool m_isDone = false;

public:
	virtual void Execute() = 0;
	virtual void Cancel() = 0;
	virtual bool IsDone() = 0;
	virtual bool CanBeSkipped() = 0;
	virtual f32 GetProgressAmount() = 0;
	virtual f32 GetMaxProgressAmount() = 0;
};