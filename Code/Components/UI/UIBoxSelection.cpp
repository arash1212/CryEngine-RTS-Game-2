#include "StdAfx.h"
#include "UIBoxSelection.h"
#include "GamePlugin.h"

#include "FlashUI/FlashUI.h"
#include "Utils/EntityUtils.h"

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterUIBoxSelectionComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CUIBoxSelectionComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterUIBoxSelectionComponent);
}

/******************************************************************************************************************************************************************************/
void CUIBoxSelectionComponent::Initialize()
{
	//Initialize SelectionBoxUIElement and set it visible
	m_pBoxSelectionUIElement = gEnv->pFlashUI->GetUIElement("box-selection");
	if (m_pBoxSelectionUIElement) {
		m_pBoxSelectionUIElement->SetVisible(true);
	}
}

/******************************************************************************************************************************************************************************/
Cry::Entity::EventFlags CUIBoxSelectionComponent::GetEventMask() const
{
	return 
		Cry::Entity::EEvent::Initialize |
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

/******************************************************************************************************************************************************************************/
void CUIBoxSelectionComponent::ProcessEvent(const SEntityEvent& event)
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
