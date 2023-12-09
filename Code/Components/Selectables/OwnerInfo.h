#pragma once

class CPlayerComponent;

enum class EPlayer {
	PLAYER1,
	PLAYER2,
	PLAYER3,
	PLAYER4,
	PLAYER5,
	PLAYER6,
};

enum class EPlayerTeam {
	TEAM_1,
	TEAM_2,
	TEAM_3,
	TEAM_4,
	TEAM_5,
	TEAM_6,
	TEAM_7
};

enum class EPlayerFaction {
	FACTION_1
};

struct SOwnerInfo {
	EPlayer m_pPlayer;
	EPlayerTeam m_pPlayerTeam;
	EPlayerFaction m_pPlayerFaction;
	CPlayerComponent* m_pPlayerComponent;

	void SetPlayerComponent(CPlayerComponent* playerComponent) {
		this->m_pPlayerComponent = playerComponent;
	}
};

class COwnerInfoComponent final : public IEntityComponent
{
public:
	COwnerInfoComponent() = default;
	virtual ~COwnerInfoComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<COwnerInfoComponent>& desc)
	{
		desc.SetGUID("{0E64A5A0-5E6D-485D-84DE-5C2C37644D35}"_cry_guid);
		desc.SetEditorCategory("Selectables");
		desc.SetDescription("Owner Info Component");
	}


private:
	SOwnerInfo m_pOwnerInfo;
	bool m_canBeTarget = true;

public:
	SOwnerInfo GetOwnerInfo();
	void SetOwnerInfo(SOwnerInfo ownerInfo);
	
	void SetCanBeTarget(bool canBeTarget);
	bool CanBeTarget();
};
