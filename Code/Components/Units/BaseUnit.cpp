#include "StdAfx.h"
#include "BaseUnit.h"
#include "GamePlugin.h"

#include <Components/Units/UnitAnimation.h>
#include <Components/Selectables/Selectable.h>
#include <Components/Controller/AIController.h>
#include <Components/Managers/ActionManager.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Units/UnitAnimation.h>
#include <Components/Selectables/UIItemProvider.h>
#include <Components/Cover/EntityCoverUser.h>
#include <Components/Selectables/OwnerInfo.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterBaseUnitComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CBaseUnitComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterBaseUnitComponent);
}

/******************************************************************************************************************************************************************************/
void CBaseUnitComponent::Initialize()
{
	//SelectableComponent Initialization
	m_pSelectableComponent = m_pEntity->GetOrCreateComponent<CSelectableComponent>();

	//AIControllerComponent Initialization
	m_pAIControllerComponent = m_pEntity->GetOrCreateComponent<CAIControllerComponent>();

	//ActionManagerComponent Initialization
	m_pActionManagerComponent = m_pEntity->GetOrCreateComponent<CActionManagerComponent>();

	//UnitStateManagerComponent Initialization
	m_pUnitStateManagerComponent = m_pEntity->GetOrCreateComponent<CUnitStateManagerComponent>();

	//UnitAnimationComponent Initialization
	m_pUnitAnimationComponent = m_pEntity->GetOrCreateComponent<CUnitAnimationComponent>();

	//UnitAnimationComponent Initialization
	m_pUnitAnimationComponent = m_pEntity->GetOrCreateComponent<CUnitAnimationComponent>();

	//UIItemProviderComponent Initialization
	m_pUIItemProviderComponent = m_pEntity->GetOrCreateComponent<CUIItemProviderComponent>();

	//EntityCoverUserComponent Initialization
	m_pEntityCoverUserComponent = m_pEntity->GetOrCreateComponent<CEntityCoverUserComponent>();

	//OwnerInfoComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetOrCreateComponent<COwnerInfoComponent>();
}

/******************************************************************************************************************************************************************************/
Cry::Entity::EventFlags CBaseUnitComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::Initialize |
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

/******************************************************************************************************************************************************************************/
void CBaseUnitComponent::ProcessEvent(const SEntityEvent& event)
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