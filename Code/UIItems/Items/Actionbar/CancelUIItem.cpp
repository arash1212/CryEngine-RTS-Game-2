#include "StdAfx.h"
#include "CancelUIItem.h"
#include "GamePlugin.h"

#include <Components/Managers/ActionManager.h>

CancelUIItem::CancelUIItem(IEntity* entity)
{
	this->m_pEntity = entity;
}

void CancelUIItem::Execute()
{
	if (!m_pEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UICancelAction : (Execute) m_pEntity is null !");
		return;
	}
	CActionManagerComponent* pActionManagerComponent = m_pEntity->GetComponent<CActionManagerComponent>();
	if (pActionManagerComponent) {
		pActionManagerComponent->CancelCurrentAction();
	}
}

string CancelUIItem::GetImagePath()
{
	return "cancel.png";
}
