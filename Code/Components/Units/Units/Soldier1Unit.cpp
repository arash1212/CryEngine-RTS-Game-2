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
	m_pAnimationComponent = m_pEntity->GetComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
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
	m_pAttackerComponent = m_pEntity->GetComponent<CAttackerComponent>();
	m_pAttackerComponent->SetDamageAmount(2.f);
	//Attack Info
	SUnitAttackInfo pAttckInfo;
	pAttckInfo.m_pAttackType = EAttackType::RANGED;
	pAttckInfo.bIsHumanoid = true;
	pAttckInfo.bIsFollower = false;
	pAttckInfo.m_maxAttackDistance = 25.f;
	pAttckInfo.m_maxAttackCount = 7;
	pAttckInfo.m_timeBetweenAttacks = 0.04f;
	m_pAttackerComponent->SetAttackInfo(pAttckInfo);
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