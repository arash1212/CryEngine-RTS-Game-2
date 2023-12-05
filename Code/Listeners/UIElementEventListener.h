#pragma once

#include <FlashUI/FlashUI.h>

class CPlayerComponent;

class UIElementEventListener : public IUIElementEventListener{
protected:
	virtual void OnUIEvent(IUIElement* pSender, const SUIEventDesc& event, const SUIArguments& args) override;

private:
	CPlayerComponent* m_pPlayerComponent = nullptr;

private:
	bool bIsMouseOverUI = false;

public:

	bool IsMouseOverUI();
	void SetPlayerComponent(CPlayerComponent* playerComponent);
};