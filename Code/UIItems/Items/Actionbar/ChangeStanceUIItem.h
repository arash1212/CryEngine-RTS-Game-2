#pragma once

#include <UIItems/IBaseUIItem.h>

class ChangeStanceUIItem : public IBaseUIItem {
public:
	ChangeStanceUIItem(IEntity* entity);

protected:
	IEntity* m_pEntity = nullptr;

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
};