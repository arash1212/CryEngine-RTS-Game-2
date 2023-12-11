#pragma once

#include <DefaultComponents/Effects/DecalComponent.h>

class CSelectableComponent final : public IEntityComponent
{
public:
	CSelectableComponent() = default;
	virtual ~CSelectableComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CSelectableComponent>& desc)
	{
		desc.SetGUID("{6157EE9B-228F-4280-9861-F8B978ECFC72}"_cry_guid);
		desc.SetEditorCategory("Selectables");
		desc.SetDescription("Selectable Component");
	}

private:
	Cry::DefaultComponents::CDecalComponent* m_pDecalComponent = nullptr;
	IRenderNode* m_pRenderNode = nullptr;

	bool m_isSelected = false;
private:

	//Highligh Colors
	uint32  m_redColor = (uint32)(int_round(4 * 255.0f) << 24) | (int_round(1 * 255.0f) << 26) | (int_round(0 * 255.0f) << 8) | (int_round(1 * 255.0f));
	uint32  m_greenColor = (uint32)(int_round(0 * 255.0f) << 24) | (int_round(1 * 255.0f) << 16) | (int_round(0 * 255.0f) << 8) | (int_round(1 * 255.0f));
	uint32  m_blackColor = 0;

public:
	void Select();
	void DeSelect();

	void HighLightGreen();
	void HighLightRed();
	void HighLightBlack();

	bool IsSelected();
};
