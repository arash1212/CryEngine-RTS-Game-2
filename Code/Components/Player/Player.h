#pragma once

class COwnerInfoComponent;

class CPlayerComponent : public IEntityComponent {

public:
	CPlayerComponent() = default;
	virtual ~CPlayerComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CPlayerComponent>& desc)
	{
		desc.SetGUID("{44A2C91D-F649-4FE0-B16A-6A7B166A4E22}"_cry_guid);
		desc.SetEditorCategory("Player");
	}

private:
	COwnerInfoComponent* m_pOwnerInfoComponent = nullptr;

private:
	bool m_isAI = true;
	DynArray<IEntity*> m_ownedEntities;

private:
	void CommandUnitsToTakeCover();

public:
	bool IsAI();
	void SetIsAI(bool isAI);

	void AddOwnedEntity(IEntity* entity);
	void RemoveOwnedEntity(IEntity* entity);

	DynArray<IEntity*> GetOwnedEntities();
};
