#include "StdAfx.h"
#include "ChangeStanceUIItem.h"
#include "GamePlugin.h"

#include <Components/Managers/UnitStateManager.h>

ChangeStanceUIItem::ChangeStanceUIItem(IEntity* entity)
{
	this->m_pEntity = entity;
}

void ChangeStanceUIItem::Execute()
{
	if (!m_pEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UICancelAction : (Execute) m_pEntity is null !");
		return;
	}
	CUnitStateManagerComponent* pUnitStateManagerComponent = m_pEntity->GetComponent<CUnitStateManagerComponent>();
	if (pUnitStateManagerComponent) {
		EUnitStance stance = pUnitStateManagerComponent->GetStance();
		pUnitStateManagerComponent->SetStance(static_cast<EUnitStance>((static_cast<int>(stance) + 1) % (static_cast<int>(EUnitStance::PRONE) + 1)));
	}
}

string ChangeStanceUIItem::GetImagePath()
{
	return "ui_stance_Icon.png";
}
