// *****************************************************************************
//  Bullet   version:  1.0   Ankur Sheel  date: 2013/02/21
//  ----------------------------------------------------------------------------
//  
//  ----------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// *****************************************************************************
// 
// *****************************************************************************
#include "stdafx.h"
#include "Bullet.h"
#include "AABB.hxx"
#include "Game\Game.h"
#include "CollisionChecker.hxx"
#include "Asteroid.h"
#include "Ship.h"

using namespace Base;
using namespace GameBase;
using namespace Graphics;
using namespace Utilities;

// *****************************************************************************
cBullet::cBullet()
: m_fTTL(0.0f)
, m_fDeactivateTime(0.0f)
{
}

// *****************************************************************************
cBullet::~cBullet()
{
}

// *****************************************************************************
void cBullet::VInitialize(const cGameElementDef & def )
{
	cAsteroidGameElement::VInitialize(def);
	m_fAcceleration = 4.0f;
	m_fDragFactor = 0.0f;
	m_fRotationPower = DegtoRad(30.0f);
	m_vForward = cVector3(1, 0, 0);
	m_vLookAt = m_vForward;
	m_bWrap = false;
	m_fTTL = 3.0f;
}

// *****************************************************************************
void cBullet::OnUpdate(float fElapsedTime)
{
	cAsteroidGameElement::OnUpdate(fElapsedTime);

	if(!m_bActive)
		return;

	IGame::GameElementList pGameElements;
	m_pGame->VGetGameElements(pGameElements);

	if (m_fDeactivateTime > 0.0f && m_fDeactivateTime <= m_pGame->GetRunningTime())
	{
		pGameElements.front()->CastToShip()->BulletDestroyed(this);
	}
}

// *****************************************************************************
void cBullet::VSetActive(const bool bActive)
{
	cAsteroidGameElement::VSetActive(bActive);
	if (bActive)
	{
		ReCalculateLookAt();
		m_vVelocity = (m_vLookAt * m_fAcceleration);
		m_fDeactivateTime = m_pGame->GetRunningTime() + m_fTTL;
	}
	else
	{
		m_fDeactivateTime = 0.0f;
	}
}

// *****************************************************************************
cBullet * cBullet::CastToBullet()
{
	return this;
}