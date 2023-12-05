#include "UIElementEventListener.h"

#include <Components/Player/Player.h>

void UIElementEventListener::OnUIEvent(IUIElement* pSender, const SUIEventDesc& event, const SUIArguments& args)
{
	string eventName = event.sDisplayName;
	if (pSender) {
		if (eventName == "actionbar_button_click") {
			int index = -1;
			args.GetArg(0).GetValueWithConversion(index);
			m_pPlayerComponent->ExecuteActionbarItem(index);
		}
		if (eventName == "mouseOverUI") {
			bIsMouseOverUI = true;
			return;
		}
		else if (eventName == "mouseNotOverUI") {
			bIsMouseOverUI = false;
		}
	}
}

bool UIElementEventListener::IsMouseOverUI()
{
	return bIsMouseOverUI;
}

void UIElementEventListener::SetPlayerComponent(CPlayerComponent* playerComponent)
{
	this->m_pPlayerComponent = playerComponent;
}
