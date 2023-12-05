#include "StdAfx.h"
#include "BoxSelectionUI.h"
#include "GamePlugin.h"

#include "FlashUI/FlashUI.h"
#include "Utils/EntityUtils.h"
#include "Components/Selectables/Selectable.h"

#include "CryEntitySystem/IEntitySystem.h"

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterBoxSelectionUIComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CBoxSelectionUIComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterBoxSelectionUIComponent);
}

/******************************************************************************************************************************************************************************/
void CBoxSelectionUIComponent::Initialize()
{
	//Initialize SelectionBoxUIElement and set it visible
	m_pBoxSelectionUIElement = gEnv->pFlashUI->GetUIElement("box-selection");
	if (m_pBoxSelectionUIElement) {
		m_pBoxSelectionUIElement->SetVisible(true);
	}
}

/******************************************************************************************************************************************************************************/
Cry::Entity::EventFlags CBoxSelectionUIComponent::GetEventMask() const
{
	return 
		Cry::Entity::EEvent::Initialize |
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

/******************************************************************************************************************************************************************************/
void CBoxSelectionUIComponent::ProcessEvent(const SEntityEvent& event)
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
void CBoxSelectionUIComponent::SetBoxStartPos(Vec2 startPos)
{
	this->m_boxStartPos = startPos;
}

/******************************************************************************************************************************************************************************/
DynArray<IEntity*> CBoxSelectionUIComponent::GetEntitiesInsideSelectionBox(Vec2 boxCurrentPos)
{
	DynArray<IEntity*> result;

	f32 width = crymath::abs(m_boxStartPos.x - boxCurrentPos.x);
	f32 height = crymath::abs(m_boxStartPos.y - boxCurrentPos.y);

	IEntityItPtr entityItPtr = gEnv->pEntitySystem->GetEntityIterator();
	entityItPtr->MoveFirst();
	while (!entityItPtr->IsEnd())
	{
		IEntity* entity = entityItPtr->Next();
		CSelectableComponent* pSelectableComponent = entity->GetComponent<CSelectableComponent>();
		if (!pSelectableComponent) {
			continue;
		}

		AABB aabb;
		entity->GetWorldBounds(aabb);

		Vec3 p1;
		Vec3 p2;
		Vec3 p3;
		Vec3 p4;
		Vec3 p5;
		Vec3 p6;
		Vec3 p7;
		Vec3 p8;
		Vec3 p9;

		IPersistantDebug* pd = gEnv->pGameFramework->GetIPersistantDebug();
		pd->Begin("CUIBoxSelectionComponent_GetEntitiesInsideSelectionBox", true);

		f32 entityWidth = crymath::abs(aabb.max.x - aabb.min.x);
		f32 entityHeight = crymath::abs(aabb.max.y - aabb.min.y);

		gEnv->pSystem->GetViewCamera().Project(aabb.min, p1);
		gEnv->pSystem->GetViewCamera().Project(aabb.max, p2);
		gEnv->pSystem->GetViewCamera().Project(aabb.GetCenter(), p3);
		gEnv->pSystem->GetViewCamera().Project(Vec3(aabb.min.x, aabb.min.y + entityHeight, aabb.min.z), p4);
		gEnv->pSystem->GetViewCamera().Project(Vec3(aabb.max.x - entityWidth, aabb.max.y, aabb.max.z), p5);
		gEnv->pSystem->GetViewCamera().Project(Vec3(aabb.min.x + entityWidth, aabb.min.y, aabb.min.z), p6);
		gEnv->pSystem->GetViewCamera().Project(Vec3(aabb.max.x, aabb.max.y - entityHeight, aabb.max.z), p7);
		gEnv->pSystem->GetViewCamera().Project(Vec3(aabb.min.x + entityWidth, aabb.min.y + entityHeight, aabb.min.z), p8);
		gEnv->pSystem->GetViewCamera().Project(Vec3(aabb.max.x - entityWidth, aabb.max.y - entityHeight, aabb.max.z), p9);

		pd->AddSphere(aabb.min, 0.1f, ColorF(0, 1, 0), 5);
		pd->AddSphere(aabb.max, 0.1f, ColorF(0, 1, 0), 5);
		pd->AddSphere(aabb.GetCenter(), 0.1f, ColorF(0, 1, 0), 5);
		pd->AddSphere(Vec3(aabb.min.x, aabb.min.y + entityHeight, aabb.min.z), 0.1f, ColorF(0, 0, 1), 5);
		pd->AddSphere(Vec3(aabb.max.x - entityWidth, aabb.max.y, aabb.max.z), 0.1f, ColorF(0, 0, 1), 5);
		pd->AddSphere(Vec3(aabb.min.x + entityWidth, aabb.min.y, aabb.min.z), 0.1f, ColorF(0, 0, 1), 5);
		pd->AddSphere(Vec3(aabb.max.x, aabb.max.y - entityHeight, aabb.max.z), 0.1f, ColorF(0, 0, 1), 5);
		pd->AddSphere(Vec3(aabb.min.x + entityWidth, aabb.min.y + entityHeight, aabb.min.z), 0.1f, ColorF(0, 0, 1), 5);
		pd->AddSphere(Vec3(aabb.max.x - entityWidth, aabb.max.y - entityHeight, aabb.max.z), 0.1f, ColorF(0, 0, 1), 5);

		f32 minX = m_boxStartPos.x < boxCurrentPos.x ? m_boxStartPos.x : boxCurrentPos.x;
		f32 maxX = minX + width;
		f32 minY = m_boxStartPos.y < boxCurrentPos.y ? m_boxStartPos.y : boxCurrentPos.y;
		f32 maxY = minY + height;

		if (p1.x >= minX && p1.x <= maxX && p1.y >= minY && p1.y <= maxY ||
			p2.x >= minX && p2.x <= maxX && p2.y >= minY && p2.y <= maxY ||
			p3.x >= minX && p3.x <= maxX && p3.y >= minY && p3.y <= maxY ||
			p4.x >= minX && p4.x <= maxX && p4.y >= minY && p4.y <= maxY ||
			p5.x >= minX && p5.x <= maxX && p5.y >= minY && p5.y <= maxY ||
			p6.x >= minX && p6.x <= maxX && p6.y >= minY && p6.y <= maxY || 
			p7.x >= minX && p7.x <= maxX && p7.y >= minY && p7.y <= maxY ||
			p8.x >= minX && p8.x <= maxX && p8.y >= minY && p8.y <= maxY ||
			p9.x >= minX && p9.x <= maxX && p9.y >= minY && p9.y <= maxY) {
			CryLog("Entity Found");
			result.append(entity);
		}
	}
	return result;
}

/******************************************************************************************************************************************************************************/
void CBoxSelectionUIComponent::SetEventListener(IUIElementEventListener* listener)
{
	this->m_pBoxSelectionUIElement->AddEventListener(listener, "BOX_SELECTION_COMPONENT_UI_EVENT_LISTENER");
}

/******************************************************************************************************************************************************************************/
/*
void CUIBoxSelectionComponent::SetCameraComponent(Cry::DefaultComponents::CCameraComponent* cameraComponent)
{
	this->m_pCameraComponent = cameraComponent;
}
*/

/******************************************************************************************************************************************************************************/
