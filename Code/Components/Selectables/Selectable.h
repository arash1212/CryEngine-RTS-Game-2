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

public:
	void Select();
	void DeSelect();
};
