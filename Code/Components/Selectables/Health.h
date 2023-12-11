#pragma once

#include <DefaultComponents/Effects/ParticleComponent.h>

class COwnerInfoComponent;
class CSelectableComponent;

struct IUIElement;

static constexpr f32 DEFAULT_MAX_HEALTH = 100.f;

class CHealthComponent final : public IEntityComponent
{

public:
	CHealthComponent() = default;
	virtual ~CHealthComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CHealthComponent>& desc)
	{
		desc.SetGUID("{33D43C47-486B-4A00-A909-DDE78C90201C}"_cry_guid);
		desc.SetEditorCategory("Selectables");
	}

private:
	Cry::DefaultComponents::CParticleComponent* m_pBloodParticleComponent = nullptr;
	COwnerInfoComponent* m_pOwnerInfoComponent = nullptr;
	CSelectableComponent* m_pSelectableComponent = nullptr;

private:
	f32 m_maxHealth = DEFAULT_MAX_HEALTH;
	f32 m_currentHealth = m_maxHealth;
	f32 m_lastHealthUpdateAmount = 0;

	bool bIsConsumesFood = false;
	bool bHealthBarAdded = false;

	int32 m_healthbarIndex = -1;

	IUIElement* m_pHealthbarUIElement = nullptr;

	//Progressbar
	f32 m_lastProgressbarUpdateAmount = -100;

	//Timers
	f32 fTimeBetweenHidingHealthbar = 1.f;
	f32 fHealthbarHidingTimePassed = 0.f;
	f32 fTimeBetweenRemoveingEntity = 0.1f;
	f32 fEntityRemoveTimePassed = 0.0f;

	f32 fTimeBetweenRestartingBloodParticle = 0.05f;
	f32 fBloodParticleRestartTimePassed = 0.0f;
private:
	void UpdateProgressAmount();

public:
	void ApplyDamage(f32 damage);
	void Die();

	void SetMaxHealth(f32 maxHealth);
	f32 GetCurrentHealth();
	f32 GetMaxHealth();

	bool IsConsumesFood();
	void SetConsumesFood(bool consumesFood);

	void AddHealthBar(bool isRed);
	void HideHealthBar();
	void ShowHealthBar();
	void SetHealthbarPosition(f32 x, f32 y);
	void SetHealthAmount(int32 health, bool isRed);
	void ClearAllHealthbars();

	void AddWorkerSlot(int32 slotNumber, bool isFilled);
	void ClearWorkersSlots();

	void SetProgressAmount(f32 progressAmount, f32 maxProgressAmount);

	void HideProgressbar();
	void ShowProgressbar();
};
