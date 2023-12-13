#pragma once

#include <Actions/IBaseAction.h>

class CAIControllerComponent;
class CAttackerComponent;
class CUnitStateManagerComponent;
class CHealthComponent;

class UnitAttackAction : public IBaseAction {

public:
	UnitAttackAction(IEntity* entity, IEntity* target);

private:
	IEntity* m_pTarget = nullptr;
	CAIControllerComponent* m_pAiControllerComponent = nullptr;
	CAttackerComponent* m_pAttackerComponent = nullptr;
	CUnitStateManagerComponent* m_pStateManagerComponent = nullptr;
	CHealthComponent* m_pTargetHealthComponent = nullptr;

private:
	bool m_IsTargetSet = false;

public:
	virtual void Execute() override;
	virtual void Cancel() override;
	virtual bool IsDone() override;
	virtual bool CanBeSkipped() override;
	virtual f32 GetProgressAmount() override;
	virtual f32 GetMaxProgressAmount() override;
};