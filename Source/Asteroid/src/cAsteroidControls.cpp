// *****************************************************************************
//  AsteroidControls   version:  1.0   Ankur Sheel  date: 2013/03/19
//  ----------------------------------------------------------------------------
//  
//  ----------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// *****************************************************************************
// 
// *****************************************************************************
#include "stdafx.h"
#include "cAsteroidControls.h"

using namespace Base;
using namespace GameBase;

// *****************************************************************************
cAsteroidControls::cAsteroidControls(const cString & strPath)
: cGameControls(strPath)
{
}


// *****************************************************************************
cAsteroidControls::~cAsteroidControls()
{
}

// *****************************************************************************
void cAsteroidControls::VSetDefaults()
{
	m_keyMap.clear();

	m_keyMap[SA_MOVE_FWD] = stGameControl("Move Ship Forward", 'W');
	m_keyMap[SA_MOVE_BK] = stGameControl("Move Ship BackWard", 'S');
	m_keyMap[SA_ROTATE_LEFT] = stGameControl("Rotate Ship Left", 'A');
	m_keyMap[SA_ROTATE_RIGHT] = stGameControl("Rotate Ship Right", 'D');
	m_keyMap[SA_FIRE] = stGameControl("Fire", VK_SPACE);
}