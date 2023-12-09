#pragma once

class COwnerInfoComponent;

class CEnemySpawnPointComponent final : public IEntityComponent
{
public:
	CEnemySpawnPointComponent() = default;
	virtual ~CEnemySpawnPointComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CEnemySpawnPointComponent>& desc)
	{
		desc.SetGUID("{71ABE7BC-0F97-4DB4-8083-FB0F49C73A8C}"_cry_guid);
		desc.SetEditorCategory("SpawnPoints");
		desc.SetDescription("Enemy Spawn Point Component");
	}

private:
	IEntity* m_pPlayerEntity = nullptr;

private:
	void SpawnPlayer();

	void SpawnUnits(IEntity* ownerPlayer);

};
