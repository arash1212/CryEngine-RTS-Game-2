#include "StdAfx.h"
#include "Selectable.h"
#include "GamePlugin.h"

#include <CryAISystem/Components/IEntityNavigationComponent.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterSelectableComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CSelectableComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterSelectableComponent);
}

/******************************************************************************************************************************************************************************/
void CSelectableComponent::Initialize()
{
	//DecalComponent Initialization
	m_pDecalComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CDecalComponent>();
	m_pDecalComponent->SetMaterialFileName("decals/selectable/selectable_decal_mat.mtl");
	m_pDecalComponent->SetDepth(5.f);

	//RenderNode Initialization
	m_pRenderNode = m_pEntity->GetRenderNode();
}

/******************************************************************************************************************************************************************************/
Cry::Entity::EventFlags CSelectableComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::Initialize |
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

/******************************************************************************************************************************************************************************/
void CSelectableComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {
		DeSelect();

	}break;
	case Cry::Entity::EEvent::Update: {

	}break;
	case Cry::Entity::EEvent::Reset: {
		DeSelect();

	}break;
	default:
		break;
	}
}

/******************************************************************************************************************************************************************************/
void CSelectableComponent::Select()
{
	m_pDecalComponent->Spawn();
	HighLightGreen();
}

/******************************************************************************************************************************************************************************/
void CSelectableComponent::DeSelect()
{
	m_pDecalComponent->Remove();
	HighLightBlack();
}

/******************************************************************************************************************************************************************************/
void CSelectableComponent::HighLightGreen()
{
	if (!m_pRenderNode) {
		m_pRenderNode = m_pEntity->GetRenderNode();
		return;
	}
	m_pRenderNode->m_nHUDSilhouettesParam = m_greenColor;
}

/******************************************************************************************************************************************************************************/
void CSelectableComponent::HighLightRed()
{
	if (!m_pRenderNode) {
		m_pRenderNode = m_pEntity->GetRenderNode();
		return;
	}
	m_pRenderNode->m_nHUDSilhouettesParam = m_redColor;
}

/******************************************************************************************************************************************************************************/
void CSelectableComponent::HighLightBlack()
{
	if (!m_pRenderNode) {
		m_pRenderNode = m_pEntity->GetRenderNode();
		return;
	}
	m_pRenderNode->m_nHUDSilhouettesParam = m_blackColor;
}