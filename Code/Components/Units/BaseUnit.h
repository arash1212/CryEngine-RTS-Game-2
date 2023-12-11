#pragma once

class CSelectableComponent;
class CUnitAnimationComponent;
class CAIControllerComponent;
class CActionManagerComponent;
class CUnitAnimationComponent;
class CUnitStateManagerComponent;
class CUIItemProviderComponent;
class CEntityCoverUserComponent;
class COwnerInfoComponent;
class CAttackerComponent;
class CHealthComponent;

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

protected:
	CSelectableComponent* m_pSelectableComponent = nullptr;
	CAIControllerComponent* m_pAIControllerComponent = nullptr;
	CActionManagerComponent* m_pActionManagerComponent = nullptr;
	CUnitStateManagerComponent* m_pUnitStateManagerComponent = nullptr;
	CUnitAnimationComponent* m_pUnitAnimationComponent = nullptr;
	CUIItemProviderComponent* m_pUIItemProviderComponent = nullptr;
	CEntityCoverUserComponent* m_pEntityCoverUserComponent = nullptr;
	COwnerInfoComponent* m_pOwnerInfoComponent = nullptr;
	CAttackerComponent* m_pAttackerComponent = nullptr;
	CHealthComponent* m_pHealthComponent = nullptr;

private:
};
