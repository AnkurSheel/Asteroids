// *****************************************************************************
//  Bullet   version:  1.0   Ankur Sheel  date: 2013/02/21
//  ----------------------------------------------------------------------------
//  
//  ----------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// *****************************************************************************
// 
// *****************************************************************************
#ifndef Bullet_h__
#define Bullet_h__

#include "AsteroidGameElement.h"

class cBullet 
	: public cAsteroidGameElement
{
public:
	cBullet();
	~cBullet();
	void VInitialize(const GameBase::cGameElementDef & def);
	void OnUpdate(float fElapsedTime);
	cBullet * CastToBullet();
	void VSetActive(const bool bActive);

private:
	float	m_fTTL;
	float	m_fDeactivateTime;
};
#endif // Bullet_h__