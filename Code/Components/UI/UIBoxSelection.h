#pragma once

struct IUIElement;

class CUIBoxSelectionComponent final : public IEntityComponent
{
public:
	CUIBoxSelectionComponent() = default;
	virtual ~CUIBoxSelectionComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CUIBoxSelectionComponent>& desc)
	{
		desc.SetGUID("{2A7725A9-2447-4888-9E37-04C9786A9BF9}"_cry_guid);
		desc.SetEditorCategory("UI");
		desc.SetDescription("UI Box Selection Component");
	}

private:
	IUIElement* m_pBoxSelectionUIElement = nullptr;

private:
};
