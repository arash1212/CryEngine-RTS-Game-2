#include "StdAfx.h"
#include "UnitAnimation.h"
#include "GamePlugin.h"

#include <CryAISystem/Components/IEntityNavigationComponent.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Controller/AIController.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterUnitAnimationComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CUnitAnimationComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterUnitAnimationComponent);
}

/******************************************************************************************************************************************************************************/
void CUnitAnimationComponent::Initialize()
{
	//AnimationComponent Initialization
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();

	//AIControllerComponent Initialization
	m_pAIControllerComponent = m_pEntity->GetComponent<CAIControllerComponent>();

	//UnitStateManagerComponent Initialization
	m_pUnitStateManagerComponent = m_pEntity->GetComponent<CUnitStateManagerComponent>();

	//Animations
	m_idleFragmentId = m_pAnimationComponent->GetFragmentId("Idle");
	m_runFragmentId = m_pAnimationComponent->GetFragmentId("Run");
	m_walkFragmentId = m_pAnimationComponent->GetFragmentId("Walk");
	m_crouchFragmentId = m_pAnimationComponent->GetFragmentId("Crouch");
	m_proneFragmentId = m_pAnimationComponent->GetFragmentId("Prone");

	m_attack1FragmentId = m_pAnimationComponent->GetFragmentId("Attack1");
	m_attack2FragmentId = m_pAnimationComponent->GetFragmentId("Attack2");
	m_attack3FragmentId = m_pAnimationComponent->GetFragmentId("Attack3");
}

/******************************************************************************************************************************************************************************/
Cry::Entity::EventFlags CUnitAnimationComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::Initialize |
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

/******************************************************************************************************************************************************************************/
void CUnitAnimationComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		UpdateAnimations();

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

/******************************************************************************************************************************************************************************/
void CUnitAnimationComponent::UpdateAnimations()
{
	if (!m_pUnitStateManagerComponent || !m_pAIControllerComponent) {
		return;
	}

	/////////////////////////////////////////
	m_pAnimationComponent->SetMotionParameter(EMotionParamID::eMotionParamID_TravelSpeed, m_pAIControllerComponent->IsMoving() ? 3.f : 0.f);

	//Run/Walk BlendSpaces
	Vec3 forwardVector = m_pEntity->GetForwardDir().normalized();
	Vec3 rightVector = m_pEntity->GetRightDir().normalized();
	Vec3 velocity = m_pAIControllerComponent->GetVelocity().normalized();

	float forwardDot = velocity.dot(forwardVector);
	float rightDot = velocity.dot(rightVector);

	int32 inv = rightDot < 0 ? 1 : -1;
	m_pAnimationComponent->SetMotionParameter(EMotionParamID::eMotionParamID_TravelAngle, crymath::acos(forwardDot) * inv);
	/////////////////////////////////////////

	//Update Animation
	FragmentID currentFragmentId;

	//Update Animation
	//Idle
	if (m_pUnitStateManagerComponent->GetStance() == EUnitStance::STANDING) {
		currentFragmentId = m_walkFragmentId;
	}

	//Walk
	else if (m_pUnitStateManagerComponent->GetStance() == EUnitStance::CROUCH) {
		currentFragmentId = m_crouchFragmentId;
	}

	//Prone
	else if (m_pUnitStateManagerComponent->GetStance() == EUnitStance::PRONE) {
		currentFragmentId = m_proneFragmentId;
	}

	//Run
	else if (m_pUnitStateManagerComponent->GetStance() == EUnitStance::RUNNING) {
		currentFragmentId = m_runFragmentId;
	}

	if (m_activeFragmentId != currentFragmentId) {
		m_activeFragmentId = currentFragmentId;
		m_pAnimationComponent->QueueFragmentWithId(m_activeFragmentId);
	}
}

/******************************************************************************************************************************************************************************/
void CUnitAnimationComponent::PlayRandomAttackAnimation()
{
}

/******************************************************************************************************************************************************************************/