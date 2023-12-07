#include "StdAfx.h"
#include "CoverPosition.h"
#include "GamePlugin.h"


#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

/******************************************************************************************************************************************************************************/
CCoverPosition::CCoverPosition(IEntity* coverObject, Vec3 coverPosition)
{
	this->m_pCoverObject = coverObject;
	this->m_coverPosition = coverPosition;
}

/******************************************************************************************************************************************************************************/
IEntity* CCoverPosition::GetCoverObject() const
{
	return m_pCoverObject;
}

/******************************************************************************************************************************************************************************/
Vec3 CCoverPosition::GetCoverPosition() const
{
	return m_coverPosition;
}

/******************************************************************************************************************************************************************************/
f32 CCoverPosition::GetCoverHeight()
{
	AABB aabb;
	m_pCoverObject->GetWorldBounds(aabb);

	return crymath::abs(aabb.max.z - aabb.min.z);
}

/******************************************************************************************************************************************************************************/
