#pragma once

#include <DefaultComponents/Input/InputComponent.h>
#include <DefaultComponents/Cameras/CameraComponent.h>
#include <DefaultComponents/Audio/ListenerComponent.h>

class CBoxSelectionUIComponent;
class CInGameUIComponent;
class IBaseUIItem;
class UIElementEventListener;
class CPlayerComponent;
class COwnerInfoComponent;

static const string PLAYER_NAME = "playerName";
static const f32 PLAYER_CAMERA_DEFAULT_HEIGHT = 25.f;
static const f32 PLAYER_CAMERA_MAX_ZOOM_AMOUNT = 6.f;
static constexpr f32 PLAYER_DEFAULT_MOVEMENT_SPEED = 10.f;

class CPlayerControllerComponent final : public IEntityComponent
{
public:
	CPlayerControllerComponent() = default;
	virtual ~CPlayerControllerComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CPlayerControllerComponent>& desc)
	{
		desc.SetGUID("{63F4C0C6-32AF-4ACB-8FB0-57D45DD14725}"_cry_guid);
		desc.SetEditorCategory("Player");
		desc.SetDescription("Player Component");
	}

protected:
	Cry::DefaultComponents::CInputComponent* m_pInputComponent = nullptr;
	Cry::DefaultComponents::CCameraComponent* m_pCameraComponent = nullptr;

	CBoxSelectionUIComponent* m_pBoxSelectionUIComponent = nullptr;
	CInGameUIComponent* m_pInGameUIComponent = nullptr;
	UIElementEventListener* m_pUIElementEventListener = nullptr;
	CPlayerComponent* m_pPlayerComponent = nullptr;
	COwnerInfoComponent* m_pOwnerInfoComponent = nullptr;
	Cry::Audio::DefaultComponents::CListenerComponent* m_pAudioListenerComp = nullptr;

private:
	f32 m_movementSpeed = PLAYER_DEFAULT_MOVEMENT_SPEED;
	Vec3 m_movementOffset = ZERO;

	//ZoomIn/Out
	f32 m_cameraMaxZoomAmount = PLAYER_CAMERA_MAX_ZOOM_AMOUNT;
	f32 m_currentZoomAmount = 0;
	f32 m_defaultPosZ = 0;

	//Right click count
	int32 m_rightClickCount = 0;
	f32 m_timeBetweenRightClickCountRestart = 0.15f;
	f32 m_rightClickCountRestartTimePassed = 0.0f;

	//Selections
	DynArray<IEntity*> m_selectedEntities;
	int32 m_lastSelectablesCheckSize = 0;

	//UIItems
	DynArray<IBaseUIItem*> m_currentUIItems;


private:
	void InitInputs();

	void Move(f32 deltaTime);

	//Input Handlers
	void MoveForwardPressed(int activationMode, float value);
	void MoveBackwardPressed(int activationMode, float value);
	void MoveRightPressed(int activationMode, float value);
	void MoveLeftPressed(int activationMode, float value);
	void SelectionPressed(int activationMode, float value);
	void CommandPressed(int activationMode, float value);
	void ZoonInPressed(int activationMode, float value);
	void ZoomOutPressed(int activationMode, float value);

	//Selection Functions
	void SelectSelectables();
	void DeSelectSelectables();
	void ValidateSelectables();

	//Commands
	void CommandSelectedUnitsToMoveTo(Vec3 position);
	void CommandUnitsToAttack(IEntity* target);

	//UIItems
	void UpdateActionbarItems();

public:
	//UIItems
	void ExecuteActionbarItem(int32 index);
};
