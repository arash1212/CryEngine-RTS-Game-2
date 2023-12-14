#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>

class COwnerInfoComponent;
class CUnitStateManagerComponent;
class CAIControllerComponent;
class CBaseWeaponComponent;
class CUnitAnimationComponent;
class CActionManagerComponent;

static constexpr f32 DEFAULT_DAMAGE_AMOUNT = 5.f;

enum class EAttackType {
	RANGED,
	MELEE
};

struct SUnitAttackInfo {
public:
	f32 m_maxAttackDistance = 20.f;
	f32 m_detectionDistance = 50.f; //Used to find "RandomAttackTarget"
	bool bIsHumanoid = false;
	bool bIsFollower = true; //Follows "RandomAttackTarget" or not if it's not in this unit's attack range.
	EAttackType m_pAttackType = EAttackType::RANGED;
	f32 m_timeBetweenAttacks = 0.04f;

	int32 m_maxAttackCount = 10;
	int32 m_attackCount = 0;
	f32 m_missChance = 0.7f;
	f32 m_damageAmount = DEFAULT_DAMAGE_AMOUNT;

};

class CAttackerComponent final : public IEntityComponent
{

public:
	CAttackerComponent() = default;
	virtual ~CAttackerComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CAttackerComponent>& desc)
	{
		desc.SetGUID("{DC6D5C54-FA94-4D10-AABD-9DBC40F19DB6}"_cry_guid);
		desc.SetEditorCategory("Units");
		/*
		desc.AddMember(&AttackerComponent::bIsHumanoid, 'ish', "ishumanoid", "Is Humanoid", "Set if is Humanoid", false);
		desc.AddMember(&AttackerComponent::bIsRanged, 'isr', "isranged", "Is Ranged", "Set if is Ranged", false);
		desc.AddMember(&AttackerComponent::bIsFollower, 'isf', "isfollower", "Is Follower", "Set if is Follower (follows random target ?)", false);
		*/
	}


private:
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	CUnitStateManagerComponent* m_pStateManagerComponent = nullptr;
	CAIControllerComponent* m_pAIControllerComponent = nullptr;
	CBaseWeaponComponent* m_pWeaponComponent = nullptr;
	CUnitAnimationComponent* m_pUnitAnimationComponent = nullptr;
	CActionManagerComponent* m_pActionManagerComponent = nullptr;
	COwnerInfoComponent* m_pOwnerInfoComponent = nullptr;

	//Target
	IEntity* m_pAttackTargetEntity = nullptr;
	IEntity* m_pRandomAttackTarget = nullptr;

	//AttackInfo
	SUnitAttackInfo m_pAttackInfo;

private:
	//Timers
	f32 m_attackTimePassed = 0.f;
	f32 m_timeBetweenAttackCountReset = 0.8f;
	f32 m_attackCountResetTimePassed = 0.f;
	f32 m_timeBetweenLookingForRandomTarget = 1.0f;
	f32 m_lookingForRandomTargetTimePassed = 0.f;

	DynArray<IEntity*> m_hostilePlayers;

	bool bIsCheckedForHstilePlayers = false;
private:
	void FindRandomTarget();
	void AttackRandomTarget();
	void ValidateTarget();

	//Attack types
	void PerformMeleeAttack(IEntity* target);
	void PerformRangedAttack(IEntity* target);

	void ApplyDamageToTarget(IEntity* target);

public:
	void Attack(IEntity* target);
	void LookAt(Vec3 position);

	bool IsAttacking();
	bool CanAttack();

	SUnitAttackInfo GetAttackInfo();
	void SetAttackInfo(SUnitAttackInfo attackInfo);

	void SetTargetEntity(IEntity* target);
	IEntity* GetTargetEntity();

	bool IsTargetVisible(IEntity* target);
};