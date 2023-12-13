#include "StdAfx.h"
#include "UnitAttackAction.h"
#include "GamePlugin.h"

#include <Utils/EntityUtils.h>
#include <Components/Selectables/Health.h>

#include <Components/Units/Attacker.h>
#include <Components/Controller/AIController.h>
#include <Components/Managers/UnitStateManager.h>

UnitAttackAction::UnitAttackAction(IEntity* entity, IEntity* target)
{
	this->m_pEntity = entity;
	this->m_pTarget = target;
	this->m_pAttackerComponent = m_pEntity->GetComponent<CAttackerComponent>();
	this->m_pAiControllerComponent = m_pEntity->GetComponent<CAIControllerComponent>();
	this->m_pStateManagerComponent = m_pEntity->GetComponent<CUnitStateManagerComponent>();
	this->m_pTargetHealthComponent = target->GetComponent<CHealthComponent>();

	//Set Unit stance to walking if it's running
	if (m_pStateManagerComponent&& m_pStateManagerComponent->GetStance()==EUnitStance::RUNNING) {
		m_pStateManagerComponent->SetStance(EUnitStance::STANDING);
	}
}

void UnitAttackAction::Execute()
{
	if (!m_pAttackerComponent){
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "AttackAction : (Execute) AttackerComponent is null");
		m_isDone = true;
		return;
	}
	if (!m_pAiControllerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "AttackAction : (Execute) AiControllerComponent is null");
		m_isDone = true;
		return;
	}

	//todo ?
	if (!m_IsTargetSet) {
		this->m_pAttackerComponent->SetTargetEntity(m_pTarget);
		this->m_IsTargetSet = true;
	}

	//if (m_pAttackerComponent->CanAttack()) {
		m_pAttackerComponent->Attack(m_pAttackerComponent->GetTargetEntity());
		m_pAttackerComponent->LookAt(m_pTarget->GetWorldPos());
	//}
	//else {
	//	m_pAiControllerComponent->MoveTo(EntityUtils::GetClosetPointOnMeshBorder(m_pEntity->GetWorldPos(), m_pTarget), false);
	//	m_pAiControllerComponent->LookAtMovePosition();
	//}
}

void UnitAttackAction::Cancel()
{
	if (!m_pAttackerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "AttackAction : (Cancel) AttackerComponent is null");
		m_isDone = true;
		return;
	}
	if (!m_pAiControllerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "AttackAction : (Cancel) AiControllerComponent is null");
		m_isDone = true;
		return;
	}

	m_pAttackerComponent->SetTargetEntity(nullptr);
	m_pAiControllerComponent->StopMoving();
	m_isDone = true;
}

bool UnitAttackAction::IsDone()
{
	return !m_pTargetHealthComponent || m_pTargetHealthComponent->GetCurrentHealth() <= 0 || !m_pTarget;
}

bool UnitAttackAction::CanBeSkipped()
{
	return false;
}

f32 UnitAttackAction::GetProgressAmount()
{
	return 0;
}

f32 UnitAttackAction::GetMaxProgressAmount()
{
	return 1;
}