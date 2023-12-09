#pragma once

#include <FlashUI/FlashUI.h>

class CPlayerControllerComponent;

class UIElementEventListener : public IUIElementEventListener{
protected:
	virtual void OnUIEvent(IUIElement* pSender, const SUIEventDesc& event, const SUIArguments& args) override;

private:
	CPlayerControllerComponent* m_pPlayerControllerComponent = nullptr;

private:
	bool bIsMouseOverUI = false;

public:

	bool IsMouseOverUI();
	void SetPlayerComponent(CPlayerControllerComponent* playerComponent);
};