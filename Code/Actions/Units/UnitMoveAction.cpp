#include "StdAfx.h"
#include "UnitMoveAction.h"
#include "GamePlugin.h"

#include <Components/Controller/AIController.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Cover/CoverPosition.h>
#include <Components/Cover/EntityCoverUser.h>

UnitMoveAction::UnitMoveAction(IEntity* entity, Vec3 movePosition, bool isRunning, CCoverPosition* coverPosition)
{
	this->m_pEntity = entity;
	this->m_movePosition = movePosition;
	this->m_isRunning = isRunning;
	this->m_pCoverPosition = coverPosition;

	this->m_pUnitStateManagerComponent = m_pEntity->GetComponent<CUnitStateManagerComponent>();
	this->pAIControllerComponent = m_pEntity->GetComponent<CAIControllerComponent>();
	this->m_pEntityCoverUserComponent = m_pEntity->GetComponent<CEntityCoverUserComponent>();

	//IsRunning
	if (m_isRunning) {
		m_pUnitStateManagerComponent->SetStance(EUnitStance::RUNNING);
	}
	else if (!m_isRunning && m_pUnitStateManagerComponent->GetStance() == EUnitStance::RUNNING) {
		m_pUnitStateManagerComponent->SetStance(EUnitStance::STANDING);
	}

	m_pEntityCoverUserComponent->SetCurrentCoverPosition(m_pCoverPosition);
}

void UnitMoveAction::Execute()
{
	if (!pAIControllerComponent) {
		CryWarning(EValidatorModule::VALIDATOR_MODULE_GAME, EValidatorSeverity::VALIDATOR_WARNING, "UnitMoveAction : (Execute) AIControllerComponent cannot be null.Action cancelled.");
		Cancel();
	}
	if (!m_pUnitStateManagerComponent) {
		CryWarning(EValidatorModule::VALIDATOR_MODULE_GAME, EValidatorSeverity::VALIDATOR_WARNING, "UnitMoveAction : (Execute) UnitStateManagerComponent cannot be null.Action cancelled.");
		Cancel();
	}

	f32 distanceToPosition = m_pEntity->GetWorldPos().GetDistance(m_movePosition);
	if (distanceToPosition <= 0.1f) {
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
	this->m_isDone = true;
	this->pAIControllerComponent->StopMoving();
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
