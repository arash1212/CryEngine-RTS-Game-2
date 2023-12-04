#pragma once

class CSelectableComponent;
class CUnitAnimationComponent;
class CAIControllerComponent;

class CBaseUnitComponent : public IEntityComponent
{
public:
	CBaseUnitComponent() = default;
	virtual ~CBaseUnitComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CBaseUnitComponent>& desc)
	{
		desc.SetGUID("{5528592D-64D4-491F-9111-9A35CA9DD048}"_cry_guid);
		desc.SetEditorCategory("Units");
		desc.SetDescription("Base Unit Component");
	}

private:
	CSelectableComponent* m_pSelectableComponent = nullptr;
	CUnitAnimationComponent* m_pUnitAnimationComponent = nullptr;
	CAIControllerComponent* m_pAIControllerComponent = nullptr;

private:
};
