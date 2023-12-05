#pragma once

#include <DefaultComponents/Effects/DecalComponent.h>

class IBaseUIItem;

class CUIItemProviderComponent final : public IEntityComponent
{
public:
	CUIItemProviderComponent() = default;
	virtual ~CUIItemProviderComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CUIItemProviderComponent>& desc)
	{
		desc.SetGUID("{525A5CB6-E71A-4FC8-8E13-A245D7E0B85D}"_cry_guid);
		desc.SetEditorCategory("Selectables");
		desc.SetDescription("UI Item Provider Component");
	}

private:
	DynArray<IBaseUIItem*> m_generalUIItems;
	DynArray<IBaseUIItem*> m_uniqueUIItems;

private:


public:
	void AddGeneralUIItem(IBaseUIItem* item);
	void AddUniqueUIItem(IBaseUIItem* item);

	DynArray<IBaseUIItem*> GetGeneralUIItems();
	DynArray<IBaseUIItem*> GetUniqueUIItems();
	DynArray<IBaseUIItem*> GetAllUIItems();
};
