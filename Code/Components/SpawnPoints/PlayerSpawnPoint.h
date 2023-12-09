#pragma once

class CPlayerSpawnPointComponent final : public IEntityComponent
{
public:
	CPlayerSpawnPointComponent() = default;
	virtual ~CPlayerSpawnPointComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CPlayerSpawnPointComponent>& desc)
	{
		desc.SetGUID("{847739A6-3F19-462E-BEFE-40EF7B08E95F}"_cry_guid);
		desc.SetEditorCategory("SpawnPoints");
		desc.SetDescription("Player Spawn Point Component");
	}

private:
	IEntity* m_pPlayerEntity = nullptr;

private:
	void SpawnPlayer();
};
