#include "StdAfx.h"
#include "Attacker.h"
#include "GamePlugin.h"

#include <Components/Selectables/OwnerInfo.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Controller/AIController.h>
#include <Components/Weapons/BaseWeapon.h>
#include <Components/Units/UnitAnimation.h>
#include <Components/Managers/ActionManager.h>
#include <Actions/IBaseAction.h>
#include <Components/Player/Player.h>
#include <Components/Player/PlayerController.h>
#include <Components/Selectables/Health.h>
#include <Components/Selectables/OwnerInfo.h>

#include <Utils/MathUtils.h>
#include <Utils/EntityUtils.h>
#include <Utils/PhysicsUtils.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


namespace
{
	static void RegisterAttackerComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CAttackerComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterAttackerComponent);
}

/******************************************************************************************************************************************************************************/
void CAttackerComponent::Initialize()
{
	//AnimationComponent Initialization
	m_pAnimationComponent = m_pEntity->GetComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();

	//AIController Initialization
	m_pAIControllerComponent = m_pEntity->GetComponent<CAIControllerComponent>();

	//tateManagerComponent Initialization
	m_pStateManagerComponent = m_pEntity->GetComponent<CUnitStateManagerComponent>();

	//WeaponComponent Initialization
	m_pWeaponComponent = m_pEntity->GetComponent<CBaseWeaponComponent>();

	//UnitAnimationComponent Initialization
	m_pUnitAnimationComponent = m_pEntity->GetOrCreateComponent<CUnitAnimationComponent>();

	//ActionManager Initializations
	m_pActionManagerComponent = m_pEntity->GetComponent<CActionManagerComponent>();

	//OwnerInfoComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetComponent<COwnerInfoComponent>();
}

/******************************************************************************************************************************************************************************/
Cry::Entity::EventFlags CAttackerComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

/******************************************************************************************************************************************************************************/
void CAttackerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		f32 DeltaTime = event.fParam[0];
 
		if (!bIsCheckedForHstilePlayers) {
			m_hostilePlayers = g_EntityUtils->FindHostilePlayers(m_pEntity);
			bIsCheckedForHstilePlayers = true;
		}
		else {
			//Target/Attack
			//if (m_pOwnerInfoComponent->GetOwnerInfo().m_pPlayerComponent->IsAI()) {
				FindRandomTarget();
				AttackRandomTarget();
			//}
		}

		//Timers
		if (m_attackTimePassed < m_pAttackInfo.m_timeBetweenAttacks) {
			m_attackTimePassed += 0.5f * DeltaTime;
		}
		if (m_attackCountResetTimePassed < m_timeBetweenAttackCountReset) {
			m_attackCountResetTimePassed += 0.5f * DeltaTime;
		}
		else {
			m_pAttackInfo.m_attackCount = 0;
		}

		if (m_lookingForRandomTargetTimePassed < m_timeBetweenLookingForRandomTarget) {
			m_lookingForRandomTargetTimePassed += 0.5f * DeltaTime;
		}

		//TODO : ATTACK ANIMATION EVENT (FOR NOT RANGED)

		ValidateTarget();

	}break;
	case Cry::Entity::EEvent::Reset: {
		m_pAttackTargetEntity = nullptr;
		m_pRandomAttackTarget = nullptr;

	}break;
	default:
		break;
	}
}

/******************************************************************************************************************************************************************************/
void CAttackerComponent::Attack(IEntity* target)
{
	if (!target) {
		return;
	}
	if (m_pAttackInfo.m_pAttackType == EAttackType::MELEE && m_attackTimePassed < m_pAttackInfo.m_timeBetweenAttacks || m_pAttackInfo.m_attackCount >= m_pAttackInfo.m_maxAttackCount) {
		return;
	}

	//Depending on this unit's attack type preform an attack 
	switch (m_pAttackInfo.m_pAttackType)
	{

	//If attacker is ranged unit
	case EAttackType::RANGED: {
		PerformRangedAttack(target);
	}break;


	//If attacker is melee unit
	case EAttackType::MELEE: {
		PerformMeleeAttack(target);
	}break;

	default:
break;
	}

	m_attackTimePassed = 0;
	m_attackCountResetTimePassed = -1.f;
}

/******************************************************************************************************************************************************************************/
void CAttackerComponent::AttackRandomTarget()
{
	//|| m_pStateManagerComponent->IsRunning()
	if (!m_pRandomAttackTarget || m_pAttackTargetEntity) {
		return;
	}

	//Attack RandomAttackTarget if it's in unit's Attack range
	if (CanAttack()) {
		this->Attack(m_pRandomAttackTarget);
		this->LookAt(m_pRandomAttackTarget->GetWorldPos());
		this->m_pAIControllerComponent->StopMoving();
	}

	else {
		//If is not a follwer empty randomAttackTarget
		if (!m_pAttackInfo.bIsFollower) {
			//m_pRandomAttackTarget = nullptr;
		}

		//If is a follwer follow random target if it's not in unit attack range
		else {
			this->m_pAIControllerComponent->MoveTo(g_EntityUtils->GetClosetPointOnMeshBorder(m_pEntity->GetWorldPos(), m_pRandomAttackTarget));
			this->m_pAIControllerComponent->LookAtMovePosition();
		}
	}
}

/******************************************************************************************************************************************************************************/
void CAttackerComponent::ValidateTarget()
{
	IEntity* target = m_pAttackTargetEntity ? m_pAttackTargetEntity : m_pRandomAttackTarget;
	if (m_pAIControllerComponent->IsMoving()) {
		m_pRandomAttackTarget = nullptr;
	}

	if (!IsTargetVisible(target)) {
		m_pRandomAttackTarget = nullptr;
	}

	if (!target || target && target->IsGarbage() || m_pAttackInfo.m_pAttackType == EAttackType::MELEE && !m_pAIControllerComponent->IsDestinationReachable(target->GetWorldPos()) || m_pActionManagerComponent->IsProcessingAnAction() && !m_pActionManagerComponent->GetCurrentAction()->CanBeSkipped()) {
		m_pAttackTargetEntity = nullptr;
		m_pRandomAttackTarget = nullptr;

		//m_pAttackInfo.m_attackCount = 0;
	}
}

/******************************************************************************************************************************************************************************/
void CAttackerComponent::PerformMeleeAttack(IEntity* target)
{
	m_pUnitAnimationComponent->PlayRandomAttackAnimation();

	this->ApplyDamageToTarget(target);

	m_pAttackInfo.m_attackCount++;
}

/******************************************************************************************************************************************************************************/
void CAttackerComponent::PerformRangedAttack(IEntity* target)
{
	if (!m_pWeaponComponent) {
		CryWarning(EValidatorModule::VALIDATOR_MODULE_GAME, EValidatorSeverity::VALIDATOR_WARNING, "CAttackerComponent : (PerformRangedAttack) m_pWeaponComponent is null.");
		m_pWeaponComponent = m_pEntity->GetComponent<CBaseWeaponComponent>();
		return;
	}
	if (m_pWeaponComponent->Fire(target)) {
		m_pUnitAnimationComponent->PlayRandomAttackAnimation();

		m_pAttackInfo.m_attackCount++;

		//TODO
		Vec3 from = m_pWeaponComponent->GetMuzzlePosition();
		//from.z += 0.4f;
		Vec3 to = target->GetWorldPos();
		to.x += g_MathUtils->GetRandomFloat(0, m_pAttackInfo.m_missChance);
		to.y += g_MathUtils->GetRandomFloat(0, m_pAttackInfo.m_missChance);

		CUnitStateManagerComponent* pUnitStateManagerComponent = target->GetComponent<CUnitStateManagerComponent>();
		if (pUnitStateManagerComponent) {
			to.z += pUnitStateManagerComponent->GetCurrentHeight();
		}
		IEntity* pHitEntity = g_PhysicsUtils->RaycastGetEntity(from, to);
		IPersistantDebug* pd = gEnv->pGameFramework->GetIPersistantDebug();
		pd->Begin("CAttackerComponent_PerformRangedAttack", false);
		if (pHitEntity && pHitEntity == target) {
			pd->AddLine(from, to, ColorF(1, 0, 0), 0.3f);
			this->ApplyDamageToTarget(target);
		}
		else {
			if (pHitEntity) {
				pd->AddLine(from, to, ColorF(1, 0, 1), 0.3f);
				CryLog("attacker hit : %s", pHitEntity->GetName());
			}
		}
	}
}

/******************************************************************************************************************************************************************************/
void CAttackerComponent::ApplyDamageToTarget(IEntity* target)
{
	CHealthComponent* healthComponent = target->GetComponent<CHealthComponent>();
	if (!healthComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "AttackerComponent : (ApplyDamageToTarget) target have to healthComonent assigned.");
		return;
	}
	healthComponent->ApplyDamage(m_pAttackInfo.m_damageAmount);
}

/******************************************************************************************************************************************************************************/
void CAttackerComponent::FindRandomTarget()
{
	//|| m_pActionManagerComponent->IsProcessingAnAction()
	if (m_pRandomAttackTarget || m_pAttackTargetEntity || m_pActionManagerComponent->IsProcessingAnAction() && !m_pActionManagerComponent->GetCurrentAction()->CanBeSkipped()) {
		return;
	}
	if (m_lookingForRandomTargetTimePassed < m_timeBetweenLookingForRandomTarget) {
		return;
	}

	//TODO : error pure function call midad vaghti entity null mishod (remove mishod)
	m_hostilePlayers = g_EntityUtils->FindHostilePlayers(m_pEntity);
	for (IEntity* entity : m_hostilePlayers)
	{
		if (!entity) {
			return;
		}

		//for (IEntity* pEntity : entity->GetComponent<ResourceManagerComponent>()->GetOwnedEntities()) {
		CPlayerComponent* pPlayerComponent = entity->GetComponent<CPlayerComponent>();
		if (!pPlayerComponent) {
			continue;
		}

		DynArray<IEntity*> enemyOwnedEntitis = pPlayerComponent->GetOwnedEntities();
		g_EntityUtils->SortEntitiesByDistance(enemyOwnedEntitis, m_pEntity->GetWorldPos(), enemyOwnedEntitis.size());
		for (IEntity* pEntity : enemyOwnedEntitis) {
			if (!pEntity || pEntity->IsGarbage()) {
				continue;
			}
			CPlayerComponent* pOtherPlayerComponent = pEntity->GetComponent<CPlayerComponent>();
			if (pOtherPlayerComponent) {
				continue;
			}
			if (!IsTargetVisible(pEntity)) {
				continue;
			}

			f32 distanceToTarget = g_EntityUtils->GetDistance(m_pEntity->GetWorldPos(), pEntity->GetWorldPos(), pEntity);
			COwnerInfoComponent* otherEntityOwnerInfo = pEntity->GetComponent<COwnerInfoComponent>();
			//Ignore entity if it's not in detection range
			if (!otherEntityOwnerInfo || distanceToTarget > m_pAttackInfo.m_detectionDistance || !otherEntityOwnerInfo->CanBeTarget()) {
				continue;
			}

			//set entity as randomAttackTarget if it's team is not same as this unit's team
			COwnerInfoComponent* pOwnerInfoComponent = m_pEntity->GetComponent<COwnerInfoComponent>();
			if (pOwnerInfoComponent && otherEntityOwnerInfo && otherEntityOwnerInfo->GetOwnerInfo().m_pPlayerTeam != pOwnerInfoComponent->GetOwnerInfo().m_pPlayerTeam) {
				m_pRandomAttackTarget = pEntity;
			}
		}
	}
	m_lookingForRandomTargetTimePassed = 0;
}

/******************************************************************************************************************************************************************************/
void CAttackerComponent::LookAt(Vec3 position)
{
	if (m_pAttackInfo.bIsHumanoid) {
		ISkeletonPose* pPose = m_pAnimationComponent->GetCharacter()->GetISkeletonPose();
		IDefaultSkeleton& pDefaultSkeleton = m_pAnimationComponent->GetCharacter()->GetIDefaultSkeleton();
		int headId = pDefaultSkeleton.GetJointIDByName("miamorig:Head");
		Vec3 headPos = m_pEntity->GetWorldPos() + pPose->GetAbsJointByID(headId).t;

		Vec3 targetPos = position;
		Vec3 currentPos = m_pEntity->GetWorldPos();
		Vec3 dir = targetPos - currentPos;
		Vec3 forwardCross = m_pEntity->GetForwardDir().cross(dir.normalized());
		Vec3 rightCross = m_pEntity->GetRightDir().cross(dir.normalized());

		//Look Up/Down
		f32 dot = m_pEntity->GetForwardDir().normalized().dot(dir.normalized());
		f32 diff = 0.2f;
		int32 uInv = 1;
		if (targetPos.z < currentPos.z) {
			uInv = -1;
		}
		f32 ac = crymath::acos(dot) * uInv;
		ac -= diff;
		m_pAnimationComponent->SetMotionParameter(EMotionParamID::eMotionParamID_TurnAngle, ac);


		//Look Left/Right
		//m_pAnimationComponent->SetMotionParameter(EMotionParamID::eMotionParamID_BlendWeight, forwardCross.z);
		m_pAIControllerComponent->LookAt(position);
	}
}

/******************************************************************************************************************************************************************************/
bool CAttackerComponent::IsAttacking()
{
	if (!m_pAttackTargetEntity && !m_pRandomAttackTarget) {
		//return false;
	}
	if (m_pAttackInfo.m_attackCount >= m_pAttackInfo.m_maxAttackCount) {
		return false;
	}

	//f32 distanceToTarget = EntityUtils::GetDistance(m_pEntity->GetWorldPos(), target->GetWorldPos(), target);
	//if (distanceToTarget > m_pAttackInfo.m_detectionDistance) {
	//	return false;
	//}

	return true;
}

/******************************************************************************************************************************************************************************/
bool CAttackerComponent::CanAttack()
{
	if (!m_pAttackTargetEntity && !m_pRandomAttackTarget) {
		return false;
	}
	IEntity* target = m_pAttackTargetEntity ? m_pAttackTargetEntity : m_pRandomAttackTarget;
	if (!target) {
		return false;
	}

	f32 distanceToTarget = g_EntityUtils->GetDistance(m_pEntity->GetWorldPos(), target->GetWorldPos(), target);
	if (distanceToTarget > m_pAttackInfo.m_maxAttackDistance) {
		return false;
	}

	if (!IsTargetVisible(target)) {
		return false;
	}

	return true;
}

/******************************************************************************************************************************************************************************/
void CAttackerComponent::SetTargetEntity(IEntity* target)
{
	this->m_pRandomAttackTarget = target;
	this->m_pAttackTargetEntity = target;
}

/******************************************************************************************************************************************************************************/
IEntity* CAttackerComponent::GetTargetEntity()
{
	return m_pAttackTargetEntity ? m_pAttackTargetEntity : m_pRandomAttackTarget;
}

/******************************************************************************************************************************************************************************/
bool CAttackerComponent::IsTargetVisible(IEntity* target)
{
	if (!target) {
		return false;
	}

	int flags = rwi_pierceability(9);
	std::array<ray_hit, 4> hits;
	static IPhysicalEntity* pSkippedEntities[10];
	pSkippedEntities[0] = m_pEntity->GetPhysicalEntity();

	Vec3 currentPos = m_pEntity->GetPos();

	CUnitStateManagerComponent* pUnitStateManagerComponent = target->GetComponent<CUnitStateManagerComponent>();
	if (!pUnitStateManagerComponent) {
		return false;
	}
	Vec3 origin = Vec3(currentPos.x, currentPos.y, currentPos.z + 1.2f);

	Vec3 targetPosition = target->GetWorldPos();
	//targetPosition.z += 0.3f;
	Vec3 targetPos = targetPosition;
	targetPosition.z += pUnitStateManagerComponent->GetCurrentHeight();

	Vec3 dir = (targetPos - m_pEntity->GetWorldPos()).normalized();

	f32 distanceToTarget = m_pEntity->GetWorldPos().GetDistance(targetPos);

	IPersistantDebug* pd = gEnv->pGameFramework->GetIPersistantDebug();
	pd->Begin("RaycastDetectionComp", false);
	if (gEnv->pPhysicalWorld->RayWorldIntersection(origin, dir * distanceToTarget, ent_all, flags, hits.data(), 4, pSkippedEntities, 4)) {
		//for (int32 i = 0; i < hits.size(); i++) {
			if (hits[0].pCollider) {
				//Debug
				if (pd) {
					pd->AddLine(origin, hits[0].pt, ColorF(0, 1, 0), 0.01f);
				}

				//return true if hitEntity is target
				IEntity* hitEntity = gEnv->pEntitySystem->GetEntityFromPhysics(hits[0].pCollider);
				if (hitEntity == target) {
					return true;
				}

				return false;
			}
		//}
	}
	return false;
}

/******************************************************************************************************************************************************************************/
SUnitAttackInfo CAttackerComponent::GetAttackInfo()
{
	return m_pAttackInfo;
}

/******************************************************************************************************************************************************************************/
void CAttackerComponent::SetAttackInfo(SUnitAttackInfo attackInfo)
{
	m_pAttackInfo = attackInfo;
}

/******************************************************************************************************************************************************************************/
