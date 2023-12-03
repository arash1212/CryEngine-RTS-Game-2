#include "StdAfx.h"
#include "Player.h"
#include "GamePlugin.h"

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

#define MOUSE_DELTA_TRESHOLD 0.0001f

namespace
{
	static void RegisterPlayerComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPlayerComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPlayerComponent);
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::Initialize()
{
	//InputComponent Initialization
	m_pInputComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();

	//CameraComponent Initialization
	m_pCameraComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCameraComponent>();
	m_pCameraComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), Quat::CreateRotationX(DEG2RAD(-55)), Vec3(0, 0, 10)));

	//Inputs Initialization
	InitInputs();
}

/******************************************************************************************************************************************************************************/
Cry::Entity::EventFlags CPlayerComponent::GetEventMask() const
{
	return Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		f32 fDeltaTime = event.fParam[0];

		Move(fDeltaTime);

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::InitInputs()
{
	//MoveForward
	m_pInputComponent->RegisterAction("player", "forward", [this](int activationMode, float value) {this->MoveForward(activationMode, value); });
	m_pInputComponent->BindAction("player", "forward", eAID_KeyboardMouse, eKI_W);

	//MoveBackward
	m_pInputComponent->RegisterAction("player", "backward", [this](int activationMode, float value) {this->MoveBackward(activationMode, value); });
	m_pInputComponent->BindAction("player", "backward", eAID_KeyboardMouse, eKI_S);

	//MoveRight
	m_pInputComponent->RegisterAction("player", "right", [this](int activationMode, float value) {this->MoveRight(activationMode, value); });
	m_pInputComponent->BindAction("player", "right", eAID_KeyboardMouse, eKI_D);

	//MoveLeft
	m_pInputComponent->RegisterAction("player", "left", [this](int activationMode, float value) {this->MoveLeft(activationMode, value); });
	m_pInputComponent->BindAction("player", "left", eAID_KeyboardMouse, eKI_A);
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::Move(f32 deltaTime)
{
	Vec3 position = m_pEntity->GetWorldPos();
	position.x += m_movementOffset.x * PLAYER_DEFAULT_MOVEMENT_SPEED * deltaTime;
	position.y += m_movementOffset.y * PLAYER_DEFAULT_MOVEMENT_SPEED * deltaTime;
	m_pEntity->SetPos(position);
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::MoveForward(int activationMode, float value)
{
	m_movementOffset.y = value;
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::MoveBackward(int activationMode, float value)
{
	m_movementOffset.y = -value;
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::MoveRight(int activationMode, float value)
{
	m_movementOffset.x = value;
}

/******************************************************************************************************************************************************************************/
void CPlayerComponent::MoveLeft(int activationMode, float value)
{
	m_movementOffset.x = -value;
}