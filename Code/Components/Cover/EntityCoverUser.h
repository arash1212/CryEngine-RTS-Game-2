#pragma once

class CAIControllerComponent;
class CCoverPosition;
class CUnitStateManagerComponent;
class CAttackerComponent;

class CEntityCoverUserComponent final : public IEntityComponent{
public:
	CEntityCoverUserComponent() = default;
	virtual ~CEntityCoverUserComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent & event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CEntityCoverUserComponent>&desc)
	{
		desc.SetGUID("{085AD725-30EA-4442-907D-C1491B212FC5}"_cry_guid);
		desc.SetEditorCategory("CoverSystem");
		desc.SetDescription("Entity CoverUser Component");
	}
private:
	CAIControllerComponent* m_pAIControllerComponent = nullptr;
	CUnitStateManagerComponent* m_pUnitStateManagerComponent = nullptr;
	CAttackerComponent* m_pAttackerComponent = nullptr;

private:
	CCoverPosition* m_pCurrentCoverPosition = nullptr;
	bool m_isMovingToCover = false;
	bool m_isStanceSet = false;

	f32 m_timeBetweenChangingStance = 0.5f;
	f32 m_changeStanceTimePassed = 0.f;
public:
	void MoveToCover();

	bool IsInCover() const;
	bool IsMovingToCover() const;
	bool IsCoverCompromised() const;

	void SetCurrentCoverPosition(CCoverPosition* coverPosition);
	CCoverPosition* GetCurrentCoverPosition() const;
	Vec3 GetCurrentCoverNormal() const;
	f32 GetHeight();
};