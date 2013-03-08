// *****************************************************************************
//  Asteroid   version:  1.0   Ankur Sheel  date: 2013/02/24
//  ----------------------------------------------------------------------------
//  
//  ----------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// *****************************************************************************
// 
// *****************************************************************************
#include "stdafx.h"
#include "Asteroid.h"
#include "RandomGenerator.hxx"
#include "AABB.hxx"
#include "Game\Game.h"
#include "CollisionChecker.hxx"
#include "HumanView.h"

using namespace Graphics;
using namespace Base;
using namespace Utilities;
using namespace GameBase;

// *****************************************************************************
cAsteroid::cAsteroid()
: m_fMinSize(0)
, m_fSizeVariance(0)
, m_iCurrentSize(0)
, m_iMaxSize(0)
{
}

// *****************************************************************************
cAsteroid::~cAsteroid()
{
	Cleanup();
}

// *****************************************************************************
void cAsteroid::VInitialize(const cGameElementDef & def )
{
	BaseInitialize(def);

	m_iCurrentSize = 1;

	cVector3 pos;
	int minPosX = static_cast<int>(m_pGame->VGetScreenTopLeftPos().x);
	int minPosY = static_cast<int>(m_pGame->VGetScreenBottomRightPos().y);
	int maxPosX = static_cast<int>(m_pGame->VGetScreenBottomRightPos().x);
	int maxPosY = static_cast<int>(m_pGame->VGetScreenTopLeftPos().y);

	pos.x = static_cast<float>(m_pGame->GetRandomGenerator()->Random(minPosX, maxPosX));
	pos.y = static_cast<float>(m_pGame->GetRandomGenerator()->Random(minPosY, maxPosY));
	SetPosition(pos);

	m_fMinSize = 2;
	m_fSizeVariance = 1;

	cVector3 scale;
	scale.x = m_fMinSize + (m_pGame->GetRandomGenerator()->Random() * (m_fSizeVariance));
	scale.y = m_fMinSize + (m_pGame->GetRandomGenerator()->Random() * (m_fSizeVariance));
	SetScale(scale);

	RandomizeRotation();

	RandomizeAcceleration();

	m_vVelocity = (m_vLookAt * m_fAcceleration);
}

// *****************************************************************************
void cAsteroid::OnRestart()
{
	cAsteroidGameElement::OnRestart();
	//m_vVelocity = cVector3(static_cast<float>(m_pGame->GetRandomGenerator()->Random(5,10)), static_cast<float>(m_pGame->GetRandomGenerator()->Random(5,10)), 0.0f);
}

// *****************************************************************************
void cAsteroid::Cleanup()
{
	cGameElement::Cleanup();
}

// *****************************************************************************
void cAsteroid::Hit()
{
	m_bRemove = true;
	if (m_iCurrentSize <= m_iMaxSize)
	{
		m_iCurrentSize++;
		cGameElementDef asteroidDef;
		asteroidDef.strModelName= "cube";

		for (int i=0 ;i<2;i++)
		{
			asteroidDef.vPosition = m_vPosition;
			asteroidDef.vScale= m_vScale/2.0f;
			m_pGame->AddAsteroid(asteroidDef, m_iCurrentSize);
		}
	}
}

// *****************************************************************************
void cAsteroid::InitializeFromParent(const cGameElementDef & def, const int iSize)
{
	m_iCurrentSize = iSize;

	BaseInitialize(def);

	RandomizeRotation();

	RandomizeAcceleration();
	m_vVelocity = (m_vLookAt * m_fAcceleration);
}

// *****************************************************************************
void cAsteroid::BaseInitialize(const cGameElementDef & def)
{
	cAsteroidGameElement::VInitialize(def);

	m_fRotationPower = DegtoRad(30.0f);
	m_fDragFactor = 0.0f;
	m_vForward = cVector3(1, 0, 0);
	m_vLookAt = m_vForward;
	m_iMaxSize = 2;
}

// *****************************************************************************
cAsteroid * cAsteroid::CastToAsteroid()
{
	return this;
}

// *****************************************************************************
void cAsteroid::RandomizeRotation()
{
	cVector3 rot;
	rot.z = static_cast<float>(m_pGame->GetRandomGenerator()->Random(0, 359));
	SetRotation(rot);
	ReCalculateLookAt();
}
// *****************************************************************************
void cAsteroid::RandomizeAcceleration()
{
	m_fAcceleration = static_cast<float>(m_pGame->GetRandomGenerator()->Random(-3,3));
	if(m_fAcceleration == 0.0f)
		m_fAcceleration = 1.0f;
}

// *****************************************************************************
int cAsteroid::GetCurrentSize() const
{
	return m_iCurrentSize;
}
