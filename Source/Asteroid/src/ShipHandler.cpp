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

using namespace Graphics;

// ***************************************************************
ShipHandler::ShipHandler()
: m_pfnCallBack(NULL)
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
	if (m_bKey['S'])
	{
		if (m_pfnCallBack)
		{
			m_pfnCallBack(SA_MOVE_BK);
		}
	}

	if (m_bKey['W'])
	{
		if (m_pfnCallBack)
		{
			m_pfnCallBack(SA_MOVE_FWD);
		}
	}

	if (m_bKey['A'])
	{
		if (m_pfnCallBack)
		{
			m_pfnCallBack(SA_ROTATE_LEFT);
		}
	}

	if (m_bKey['D'])
	{
		if (m_pfnCallBack)
		{
			m_pfnCallBack(SA_ROTATE_RIGHT);
		}
	}

	if (m_bKey[VK_SPACE])
	{
		if (m_pfnCallBack)
		{
			m_pfnCallBack(SA_FIRE);
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