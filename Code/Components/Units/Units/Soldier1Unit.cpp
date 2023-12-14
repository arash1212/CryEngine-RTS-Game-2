#include "StdAfx.h"
#include "Soldier1Unit.h"
#include "GamePlugin.h"

#include <Components/Weapons/BaseWeapon.h>
#include <Components/Units/UnitAnimation.h>
#include <Components/Units/BaseUnit.h>
#include <CryAISystem/Components/IEntityNavigationComponent.h>

#include <UIItems/Items/Actionbar/CancelUIItem.h>
#include <UIItems/Items/Actionbar/ChangeStanceUIItem.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterSoldier1UnitComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CSoldier1UnitComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterSoldier1UnitComponent);
}

/******************************************************************************************************************************************************************************/
void CSoldier1UnitComponent::Initialize()
{
	CBaseUnitComponent::Initialize();

	//AnimationComponent Initialization
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0)));
	m_pAnimationComponent->SetCharacterFile("Objects/Characters/units/soldier1/soldier_1.cdf");
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/soldier1.adb");
	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Idle");
	m_pAnimationComponent->SetAnimationDrivenMotion(false);
	m_pAnimationComponent->LoadFromDisk();
	m_pAnimationComponent->ResetCharacter();

	//UIItemProviderComponent Initialization
	m_pUIItemProviderComponent = m_pEntity->GetComponent<CUIItemProviderComponent>();
	m_pUIItemProviderComponent->AddGeneralUIItem(new CancelUIItem(m_pEntity));
	m_pUIItemProviderComponent->AddGeneralUIItem(new ChangeStanceUIItem(m_pEntity));

	//BaseWeaponComponent Initialization
	m_pBaseWeaponComponent = m_pEntity->GetOrCreateComponent<CBaseWeaponComponent>();
	m_pBaseWeaponComponent->Draw();

	//AttackerComponent Initialization
	m_pAttackerComponent = m_pEntity->GetOrCreateComponent<CAttackerComponent>();
	//Attack Info
	SUnitAttackInfo pAttckInfo;
	pAttckInfo.m_pAttackType = EAttackType::RANGED;
	pAttckInfo.bIsHumanoid = true;
	pAttckInfo.bIsFollower = false;
	pAttckInfo.m_maxAttackDistance = 40.f;
	pAttckInfo.m_maxAttackCount = 15;
	pAttckInfo.m_timeBetweenAttacks = 0.3f;

	//Set Damage Amount
	if (m_pOwnerInfoComponent->GetOwnerInfo().m_pPlayerComponent->IsAI()) {
		pAttckInfo.m_missChance = 0.9f;
		m_pHealthComponent->SetMaxHealth(70);
	}
	else {
		pAttckInfo.m_missChance = 0.15f;
		//pAttckInfo.m_damageAmount = pAttckInfo.m_damageAmount * 2;
		m_pHealthComponent->SetMaxHealth(300);
	}

	m_pAttackerComponent->SetAttackInfo(pAttckInfo);

	//UnitAnimationComponent Initialization
	m_pUnitAnimationComponent->InitAnimations();

	m_pEntity->SetName("Soldier1");
}

/******************************************************************************************************************************************************************************/
Cry::Entity::EventFlags CSoldier1UnitComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::Initialize |
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

/******************************************************************************************************************************************************************************/
void CSoldier1UnitComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

/******************************************************************************************************************************************************************************/