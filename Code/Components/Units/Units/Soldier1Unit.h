#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <Components/Units/BaseUnit.h>

class CUIItemProviderComponent;
class CBaseWeaponComponent;
class CAttackerComponent;

class CSoldier1UnitComponent final : public CBaseUnitComponent
{
public:
	CSoldier1UnitComponent() = default;
	virtual ~CSoldier1UnitComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CSoldier1UnitComponent>& desc)
	{
		desc.SetGUID("{BF1ABE42-7B40-441D-A9C4-FAF12904D9F2}"_cry_guid);
		desc.SetEditorCategory("Units");
		desc.SetDescription("Unit Soldier1 Component");
		desc.AddBase<CBaseUnitComponent>();
	}


private:
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	CUIItemProviderComponent* m_pUIItemProviderComponent = nullptr;
	CBaseWeaponComponent* m_pBaseWeaponComponent = nullptr;
	CAttackerComponent* m_pAttackerComponent = nullptr;

private:
};
