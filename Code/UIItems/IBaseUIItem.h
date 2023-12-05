#pragma once

class IBaseUIItem {

protected:
	IEntity* m_pEntity = nullptr;

protected:

public:
	virtual void Execute() = 0;
	virtual string GetImagePath() = 0;
};