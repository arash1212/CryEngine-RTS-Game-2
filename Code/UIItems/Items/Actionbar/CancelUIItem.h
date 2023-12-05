#pragma once

#include <UIItems/IBaseUIItem.h>

class CancelUIItem : public IBaseUIItem {
public:
	CancelUIItem(IEntity* entity);

protected :
	IEntity* m_pEntity = nullptr;

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
};