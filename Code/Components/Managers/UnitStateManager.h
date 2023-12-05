#pragma once

class CAIControllerComponent;

enum class EUnitState {
	IDLE,
	WALKING
};

enum class EUnitStance {
	PRONE,
	CROUCH,
	STANDING,
	RUNNING
};

class CUnitStateManagerComponent : public IEntityComponent {

public:
	CUnitStateManagerComponent() = default;
	virtual ~CUnitStateManagerComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CUnitStateManagerComponent>& desc)
	{
		desc.SetGUID("{3B9F0E37-1628-40EB-A9B7-A9C6A56585D2}"_cry_guid);
		desc.SetEditorCategory("Managers");
	}

private:
	CAIControllerComponent* m_pAIControllerComponent = nullptr;

	EUnitState m_pUnitState = EUnitState::IDLE;
	EUnitStance m_pUnitStance = EUnitStance::STANDING;

private:
	f32 m_proneSpeed = 0.5f;
	f32 m_crouchSpeed = 1.f;
	f32 m_walkSpeed = 1.5f;
	f32 m_runSpeed = 2.f;
	f32 m_currentSpeed = 0.f;

private:
	void UpdateState();
	void UpdateSpeed();

public:
	EUnitState GetState();
	EUnitStance GetStance();
	f32 GetCurrentSpeed();

	void SetStanceToRun();
	void SetStanceToWalk();
};