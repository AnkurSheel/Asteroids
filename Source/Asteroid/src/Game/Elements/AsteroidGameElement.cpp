// *****************************************************************************
//  AsteroidGameElement   version:  1.0   Ankur Sheel  date: 2013/02/21
//  ----------------------------------------------------------------------------
//  
//  ----------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// *****************************************************************************
// 
// *****************************************************************************
#include "stdafx.h"
#include "AsteroidGameElement.h"
#include "Game\Game.h"

using namespace Base;
using namespace GameBase;
using namespace Utilities;

cGame * cAsteroidGameElement::m_pGame = NULL;

// *****************************************************************************
cAsteroidGameElement::cAsteroidGameElement()
: m_fRotationPower(0)
, m_fDragFactor(0)
, m_fAcceleration(0)
, m_bWrap(true)
, m_bRemove(false)
{
}

// *****************************************************************************
cAsteroidGameElement::~cAsteroidGameElement()
{
}

// *****************************************************************************
void cAsteroidGameElement::VInitialize(const cGameElementDef & def)
{
	cGameElement::VInitialize(def);
	m_vInitialPos = GetPosition();
}

// *****************************************************************************
void cAsteroidGameElement::OnRestart()
{
	cGameElement::OnRestart();
	SetPosition(m_vInitialPos);
}

// *****************************************************************************
cShip * cAsteroidGameElement::CastToShip()
{
	return NULL;
}

// *****************************************************************************
cAsteroid * cAsteroidGameElement::CastToAsteroid()
{
	return NULL;
}

cBullet * cAsteroidGameElement::CastToBullet()
{
	return NULL;
}

// *****************************************************************************
void cAsteroidGameElement::SetGame(IBaseApp * const pGame)
{
	cGameElement::SetApp(pGame);
	m_pGame = dynamic_cast<cGame * const>(pGame);
}

// *****************************************************************************
bool cAsteroidGameElement::GetRemove() const
{
	return m_bRemove;
}

// *****************************************************************************
void cAsteroidGameElement::OnUpdate(float fElapsedTime)
{
	if(m_bIsDirty)
	{
		ReCalculateLookAt();
	}
	if (m_vVelocity.LengthSquared() > EpsilonFloat)
	{
		cVector3 vPredictedPos = GetPosition() + (m_vVelocity * fElapsedTime);
		if (m_bWrap)
		{
			WrapAround(vPredictedPos);
		}
		SetPosition(vPredictedPos);
		m_vVelocity -= (m_vVelocity * m_fDragFactor);
	}

	cGameElement::OnUpdate(fElapsedTime);
}

// *****************************************************************************
void cAsteroidGameElement::WrapAround(cVector3 &vPredictedPos)
{
	if (vPredictedPos.x < m_pGame->VGetScreenTopLeftPos().x)
	{
		vPredictedPos.x = m_pGame->VGetScreenBottomRightPos().x;
	}
	else if (vPredictedPos.x > m_pGame->VGetScreenBottomRightPos().x)
	{
		vPredictedPos.x = m_pGame->VGetScreenTopLeftPos().x;
	}
	else if (vPredictedPos.y > m_pGame->VGetScreenTopLeftPos().y)
	{
		vPredictedPos.y = m_pGame->VGetScreenBottomRightPos().y;
	}
	else if (vPredictedPos.y < m_pGame->VGetScreenBottomRightPos().y)
	{
		vPredictedPos.y = m_pGame->VGetScreenTopLeftPos().y;
	}
}

// *****************************************************************************
void cAsteroidGameElement::ReCalculateLookAt()
{
	float cosAngle = cos(m_vRotation.z);
	float sinAngle = sin(m_vRotation.z);
	m_vLookAt.x = m_vForward.x * cosAngle - m_vForward.y * sinAngle;
	m_vLookAt.y = m_vForward.x * sinAngle + m_vForward.y * cosAngle;
	m_vLookAt.z = m_vForward.z;
	m_vLookAt.Normalize();
}