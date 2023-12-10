#pragma once

class CEntityCoverUserComponent;
class CCoverPosition;

class CBaseAIUnitComponent final : public IEntityComponent
{

public:
	CBaseAIUnitComponent() = default;
	virtual ~CBaseAIUnitComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CBaseAIUnitComponent>& desc)
	{
		desc.SetGUID("{5B398761-25DB-4D27-9657-C770228B5855}"_cry_guid);
		desc.SetEditorCategory("Units");
	}

private:
	CEntityCoverUserComponent* m_pEntityCoverUserComponent = nullptr;

public:
	bool LookForCover();
	void MoveToCover(CCoverPosition* coverPosition);
};