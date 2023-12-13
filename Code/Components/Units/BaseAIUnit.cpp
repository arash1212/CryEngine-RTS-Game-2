#include "StdAfx.h"
#include "BaseAIUnit.h"
#include "GamePlugin.h"

#include <Components/Selectables/OwnerInfo.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Controller/AIController.h>
#include <Components/Weapons/BaseWeapon.h>
#include <Components/Units/UnitAnimation.h>
#include <Components/Managers/ActionManager.h>
#include <Actions/IBaseAction.h>
#include <Components/Player/Player.h>
#include <Components/Player/PlayerController.h>
#include <Components/Cover/EntityCoverUser.h>

#include <Utils/MathUtils.h>
#include <Utils/EntityUtils.h>
#include <Utils/CoverUtils.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


namespace
{
	static void RegisterBaseAIUnitComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CBaseAIUnitComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterBaseAIUnitComponent);
}

/******************************************************************************************************************************************************************************/
void CBaseAIUnitComponent::Initialize()
{
	//EntityCoverUserComponent Initialization
	m_pEntityCoverUserComponent = m_pEntity->GetOrCreateComponent<CEntityCoverUserComponent>();
}

/******************************************************************************************************************************************************************************/
Cry::Entity::EventFlags CBaseAIUnitComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

/******************************************************************************************************************************************************************************/
void CBaseAIUnitComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {
		

	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

/******************************************************************************************************************************************************************************/
bool CBaseAIUnitComponent::LookForCover()
{
	if (!m_pEntityCoverUserComponent->GetCurrentCoverPosition()) {
		m_pEntityCoverUserComponent->SetCurrentCoverPosition(g_CoverUtils->FindCoverPointsAroundPosition(m_pEntity->GetWorldPos(), m_pEntity, 30, 1)[0]);
		CryLog("cover set ");
		return false;
	}
	else {
		return true;
	}
	return false;
}

/******************************************************************************************************************************************************************************/
void CBaseAIUnitComponent::MoveToCover(CCoverPosition* coverPosition)
{
	this->m_pEntityCoverUserComponent->SetCurrentCoverPosition(coverPosition);
}

/******************************************************************************************************************************************************************************/
