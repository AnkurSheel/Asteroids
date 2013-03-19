// *****************************************************************************
//  ShipHandler   version:  1.0   Ankur Sheel  date: 2013/02/21
//  ----------------------------------------------------------------------------
//  
//  ----------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// *****************************************************************************
// 
// *****************************************************************************
#include "stdafx.h"
#include "ShipHandler.h"
#include "Structures.h"
#include "AsteroidControls.h"

using namespace Graphics;
using namespace GameBase;

// ***************************************************************
ShipHandler::ShipHandler()
: m_pfnCallBack(NULL)
, m_pGameControls(NULL)
{
	memset(m_bKey, 0, sizeof(m_bKey));
}

// ***************************************************************
ShipHandler::~ShipHandler()
{
	UnregisterCallBack();
}

// ***************************************************************
bool ShipHandler::VOnKeyDown(const unsigned int iCharID)
{
	BYTE c = static_cast<const BYTE>(iCharID);
	m_bKey[c] = true;
	return true;
}

// ***************************************************************
bool ShipHandler::VOnKeyUp(const unsigned int iCharID)
{
	BYTE c = static_cast<const BYTE>(iCharID);
	m_bKey[c] = false;
	return true;
}

// ***************************************************************
bool ShipHandler::VOnCharPress(const unsigned int iCharID)
{
	return false;
}

// ***************************************************************
void ShipHandler::OnUpdate()
{
	if (m_pGameControls == NULL)
	{
		return;
	}

	if (m_bKey[(*m_pGameControls)[AC_MOVE_BK]])
	{
		if (m_pfnCallBack)
		{
			m_pfnCallBack(AC_MOVE_BK);
		}
	}

	if (m_bKey[(*m_pGameControls)[AC_MOVE_FWD]])
	{
		if (m_pfnCallBack)
		{
			m_pfnCallBack(AC_MOVE_FWD);
		}
	}

	if (m_bKey[(*m_pGameControls)[AC_ROTATE_LEFT]])
	{
		if (m_pfnCallBack)
		{
			m_pfnCallBack(AC_ROTATE_LEFT);
		}
	}

	if (m_bKey[(*m_pGameControls)[AC_ROTATE_RIGHT]])
	{
		if (m_pfnCallBack)
		{
			m_pfnCallBack(AC_ROTATE_RIGHT);
		}
	}

	if (m_bKey[(*m_pGameControls)[AC_FIRE]])
	{
		if (m_pfnCallBack)
		{
			m_pfnCallBack(AC_FIRE);
		}
	}
}

// ***************************************************************
void ShipHandler::RegisterCallBack(ShipInputCallBackFn callback )
{
	m_pfnCallBack = callback;
}

// ***************************************************************
void ShipHandler::UnregisterCallBack()
{
	m_pfnCallBack = NULL;
}

// ***************************************************************
void ShipHandler::VLockKey( const BYTE c, const bool bLock )
{

}

// *****************************************************************************
void ShipHandler::SetGameControls(cGameControls * pGameControls)
{
	m_pGameControls = pGameControls;
}
