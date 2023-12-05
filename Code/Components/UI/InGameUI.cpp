#include "StdAfx.h"
#include "InGameUI.h"
#include "GamePlugin.h"

#include "FlashUI/FlashUI.h"
#include "Utils/EntityUtils.h"

#include "CryEntitySystem/IEntitySystem.h"

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterInGameUIComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CInGameUIComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterInGameUIComponent);
}

/******************************************************************************************************************************************************************************/
void CInGameUIComponent::Initialize()
{
	//Initialize InGameUIElement and set it visible
	m_pInGameUIElement = gEnv->pFlashUI->GetUIElement("in-game");
	if (m_pInGameUIElement) {
		m_pInGameUIElement->SetVisible(true);
	}
}

/******************************************************************************************************************************************************************************/
Cry::Entity::EventFlags CInGameUIComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::Initialize |
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

/******************************************************************************************************************************************************************************/
void CInGameUIComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

/******************************************************************************************************************************************************************************/
void CInGameUIComponent::AddActionbarItem(string imagePath)
{
	SUIArguments args;
	args.AddArgument(imagePath);

	m_pInGameUIElement->CallFunction("AddActionbarItem", args);

}
/******************************************************************************************************************************************************************************/
void CInGameUIComponent::ClearActionbarItems()
{
	m_pInGameUIElement->CallFunction("ClearActionbar");
}

/******************************************************************************************************************************************************************************/