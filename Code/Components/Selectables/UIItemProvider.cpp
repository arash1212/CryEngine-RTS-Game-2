#include "StdAfx.h"
#include "UIItemProvider.h"
#include "GamePlugin.h"

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterUIItemProviderComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CUIItemProviderComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterUIItemProviderComponent);
}

/******************************************************************************************************************************************************************************/
void CUIItemProviderComponent::Initialize()
{

}

/******************************************************************************************************************************************************************************/
Cry::Entity::EventFlags CUIItemProviderComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::Initialize |
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

/******************************************************************************************************************************************************************************/
void CUIItemProviderComponent::ProcessEvent(const SEntityEvent& event)
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
void CUIItemProviderComponent::AddGeneralUIItem(IBaseUIItem* item)
{
	this->m_generalUIItems.append(item);
}

/******************************************************************************************************************************************************************************/
void CUIItemProviderComponent::AddUniqueUIItem(IBaseUIItem* item)
{
	this->m_uniqueUIItems.append(item);
}

/******************************************************************************************************************************************************************************/
DynArray<IBaseUIItem*> CUIItemProviderComponent::GetGeneralUIItems()
{
	return m_generalUIItems;
}

/******************************************************************************************************************************************************************************/
DynArray<IBaseUIItem*> CUIItemProviderComponent::GetUniqueUIItems()
{
	return m_uniqueUIItems;
}

/******************************************************************************************************************************************************************************/
DynArray<IBaseUIItem*> CUIItemProviderComponent::GetAllUIItems()
{
	return m_generalUIItems + m_uniqueUIItems;
}

/******************************************************************************************************************************************************************************/