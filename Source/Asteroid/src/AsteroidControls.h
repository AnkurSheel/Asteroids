// *****************************************************************************
//  AsteroidControls   version:  1.0   Ankur Sheel  date: 2013/03/19
//  ----------------------------------------------------------------------------
//  
//  ----------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// *****************************************************************************
// 
// *****************************************************************************
#ifndef AsteroidControls_h__
#define AsteroidControls_h__

#include "GameControls.h"

class cAsteroidControls :
	public GameBase::cGameControls
{
public:
	cAsteroidControls(const Base::cString & strPath);
	~cAsteroidControls();
	void VSetDefaults();
};

#endif