#pragma once

struct IUIElement;

class CInGameUIComponent final : public IEntityComponent
{
public:
	CInGameUIComponent() = default;
	virtual ~CInGameUIComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CInGameUIComponent>& desc)
	{
		desc.SetGUID("{8045EB7F-9456-4D7B-85E3-15D57DBDA745}"_cry_guid);
		desc.SetEditorCategory("UI");
		desc.SetDescription("In Game UI Component");
	}


private:
	IUIElement* m_pInGameUIElement = nullptr;

public:
	void AddActionbarItem(string imagePath);
};