#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>

class CUnitAnimationComponent final : public IEntityComponent
{
public:
	CUnitAnimationComponent() = default;
	virtual ~CUnitAnimationComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CUnitAnimationComponent>& desc)
	{
		desc.SetGUID("{9495C3F2-724D-4F99-B7E9-26E9EFBD1103}"_cry_guid);
		desc.SetEditorCategory("Units");
		desc.SetDescription("Unit Animation Component");
	}

private:
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;

private:
};
