// *****************************************************************************
//  Asteroid   version:  1.0   Ankur Sheel  date: 2013/02/24
//  ----------------------------------------------------------------------------
//  
//  ----------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// *****************************************************************************
// 
// *****************************************************************************
#ifndef Asteroid_h__
#define Asteroid_h__

#include "AsteroidGameElement.h"

class cAsteroid  
	: public cAsteroidGameElement
{
public:
	cAsteroid();
	~cAsteroid();
	void VInitialize(const GameBase::cGameElementDef & def);
	void BaseInitialize(const GameBase::cGameElementDef & def);
	void InitializeFromParent(const GameBase::cGameElementDef & def, const int iSize);
	void RandomizeAcceleration();
	void RandomizeRotation();
	void OnRestart();
	void Cleanup();
	void Hit();
	int GetCurrentSize() const;
	cAsteroid * CastToAsteroid();

private:
	float		m_fMinSize;
	float		m_fSizeVariance;
	int			m_iCurrentSize;
	int			m_iMaxSize;
};
#endif // Asteroid_h__
