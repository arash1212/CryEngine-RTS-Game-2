#pragma once

#include <DefaultComponents/Physics/CharacterControllerComponent.h>

struct IEntityNavigationComponent;

class CUnitStateManagerComponent;

class CAIControllerComponent final : public IEntityComponent
{
public:
	CAIControllerComponent() = default;
	virtual ~CAIControllerComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CAIControllerComponent>& desc)
	{
		desc.SetGUID("{68542B1B-CD10-4782-9D0E-3538E52063F3}"_cry_guid);
		desc.SetEditorCategory("Controllers");
		desc.SetDescription("AI Controller Component");
	}


private:
	Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterControllerComponent = nullptr;
	IEntityNavigationComponent* m_pNavigationComponent = nullptr;
	CUnitStateManagerComponent* m_pUnitStateManagerComponent = nullptr;

private:
	void Move();
public:
	void MoveTo(Vec3 position);
	void LookAtMovePosition();

	bool IsMoving();
	Vec3 GetVelocity();
};
