#pragma once

//#include <DefaultComponents/Cameras/CameraComponent.h>

struct IUIElement;

class CBoxSelectionUIComponent final : public IEntityComponent
{
public:
	CBoxSelectionUIComponent() = default;
	virtual ~CBoxSelectionUIComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CBoxSelectionUIComponent>& desc)
	{
		desc.SetGUID("{2A7725A9-2447-4888-9E37-04C9786A9BF9}"_cry_guid);
		desc.SetEditorCategory("UI");
		desc.SetDescription("Box Selection UI Component");
	}

private:
	//Cry::DefaultComponents::CCameraComponent* m_pCameraComponent = nullptr;
	IUIElement* m_pBoxSelectionUIElement = nullptr;
	
private:
	Vec2 m_boxStartPos = ZERO;

public:
	void SetBoxStartPos(Vec2 startPos);
	DynArray<IEntity*> GetEntitiesInsideSelectionBox(Vec2 boxCurrentPos);

	//void SetCameraComponent(Cry::DefaultComponents::CCameraComponent* cameraComponent);
};
