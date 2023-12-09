#include "StdAfx.h"
#include "Player.h"
#include "GamePlugin.h"

#include <Actions/IBaseAction.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterPlayerComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPlayerComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPlayerComponent);
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::Initialize()
{

}

/******************************************************************************************************************************************************************************/
Cry::Entity::EventFlags CPlayerComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::ProcessEvent(const SEntityEvent& event)
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
bool CPlayerComponent::IsAI()
{
	return m_isAI;
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::SetIsAI(bool isAI)
{
	this->m_isAI = isAI;
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::AddOwnedEntity(IEntity* entity)
{
	m_ownedEntities.append(entity);
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::RemoveOwnedEntity(IEntity* entity)
{
	DynArray<IEntity*> result;
	for (IEntity* pEntity : m_ownedEntities) {
		if (pEntity == entity) {
			continue;
		}
		result.append(pEntity);
	}

	m_ownedEntities = result;
}

/******************************************************************************************************************************************************************************/