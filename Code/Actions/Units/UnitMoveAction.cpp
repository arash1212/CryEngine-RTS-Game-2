#include "StdAfx.h"
#include "UnitMoveAction.h"
#include "GamePlugin.h"

#include "Components/Controller/AIController.h"

UnitMoveAction::UnitMoveAction(IEntity* entity, Vec3 movePosition)
{
	this->m_pEntity = entity;
	this->m_movePosition = movePosition;
}

void UnitMoveAction::Execute()
{
	CAIControllerComponent* pAIControllerComponent = m_pEntity->GetComponent<CAIControllerComponent>();
	if (!pAIControllerComponent) {
		CryWarning(EValidatorModule::VALIDATOR_MODULE_GAME, EValidatorSeverity::VALIDATOR_WARNING, "UnitMoveAction : (Execute) AIControllerComponent cannot be null.Action cancelled.");
		Cancel();
	}

	f32 distanceToPosition = m_pEntity->GetWorldPos().GetDistance(m_movePosition);
	if (distanceToPosition <= 1) {
		m_isDone = true;
		return;
	}

	if (!m_navigationRequestDone) {
		pAIControllerComponent->MoveTo(m_movePosition);
		m_navigationRequestDone = true;
	}
	pAIControllerComponent->LookAtMovePosition();
}

void UnitMoveAction::Cancel()
{
	m_isDone = true;
}

bool UnitMoveAction::IsDone()
{
	return false;
}

bool UnitMoveAction::CanBeSkipped()
{
	return false;
}

f32 UnitMoveAction::GetProgressAmount()
{
	return 0;
}

f32 UnitMoveAction::GetMaxProgressAmount()
{
	return 1;
}
