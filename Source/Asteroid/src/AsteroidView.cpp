// *****************************************************************************
//  AsteroidView   version:  1.0   Ankur Sheel  date: 2013/02/21
//  ----------------------------------------------------------------------------
//  
//  ----------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// *****************************************************************************
// 
// *****************************************************************************
#include "stdafx.h"
#include "AsteroidView.h"
#include "Game/Game.h"
#include "Game/Elements/AsteroidGameElement.h"
#include "ShipHandler.h"
#include "Camera.hxx"
#include "MessageDispatchManager.hxx"

using namespace Utilities;
using namespace Graphics;
using namespace GameBase;
using namespace Base;

// *****************************************************************************
cAsteroidView::cAsteroidView()
: m_pGame(NULL)
{
}

// *****************************************************************************
cAsteroidView::~cAsteroidView()
{
}

void cAsteroidView::VOnCreateDevice(IBaseApp * pGame,
								 const HINSTANCE & hInst,
								 const HWND & hWnd)
{
	cHumanView::VOnCreateDevice(pGame, hInst, hWnd);
	m_pGame = dynamic_cast<cGame *>(pGame);
	if (m_pCamera)
	{
		m_pCamera->VSetPosition(cVector3(0.0f, 0.0f, -20.0f));
		m_pCamera->VUpdate();
	}
}

// *****************************************************************************
void cAsteroidView::VOnUpdate(const TICK tickCurrent, const float fElapsedTime)
{
	cHumanView::VOnUpdate(tickCurrent, fElapsedTime);
	if (m_ShipHandler)
	{
		m_ShipHandler->OnUpdate();
	}
}

// *****************************************************************************
bool cAsteroidView::VOnMsgProc( const Base::AppMsg & msg )
{
	if(!cHumanView::VOnMsgProc(msg))
	{
		if(msg.m_uMsg == WM_KEYDOWN)
		{
			if (msg.m_wParam == VK_ESCAPE  && !IsKeyLocked(VK_ESCAPE))
			{
				// lock the ESC key
				LockKey(VK_ESCAPE);
				IMessageDispatchManager::GetInstance()->VDispatchMessage(0.0f, m_pGame->VGetID(),
					m_pGame->VGetID(), MSG_ESCAPE_PRESSED, NULL);
			}
			if (m_ShipHandler)
			{
				m_ShipHandler->VOnKeyDown(msg.m_wParam);
			}
		}
		else if (msg.m_uMsg == WM_KEYUP)
		{
			if (msg.m_wParam == VK_F2)
			{
				UnlockKey(VK_F2);
			}
			if (msg.m_wParam == VK_ESCAPE)
			{
				UnlockKey(VK_ESCAPE);
			}
			if (m_ShipHandler)
			{
				m_ShipHandler->VOnKeyUp(msg.m_wParam);
			}
		}
	}
	return true;
}

// *****************************************************************************
void cAsteroidView::OnPlaySelected(cGame * const pGame)
{
	m_ShipHandler = shared_ptr<ShipHandler>(DEBUG_NEW ShipHandler());
	ShipHandler::ShipInputCallBackFn callbackShip;
	callbackShip = bind(&cGame::MoveShip, pGame, _1);
	m_ShipHandler->RegisterCallBack(callbackShip);
	m_ShipHandler->SetGameControls(pGame->GetGameControls());
}

// *******************************************************************************************
void cAsteroidView::VRenderPrivate()
{
	IGame::GameElementList::iterator iter;
	IGame::GameElementList pGameElements;
	m_pGame->VGetGameElements(pGameElements);
	for (iter = pGameElements.begin(); iter != pGameElements.end(); iter++)
	{
		(*iter)->VOnRender(m_pCamera);
	}
	//for (int i=0; i<m_pGame->PGE_TOTAL; i++)
	//{
	//	if(m_pGame->VGetGameElements() && m_pGame->VGetGameElements()[i])
	//	{
	//		m_pGame->VGetGameElements()[i]->Render(m_pCamera);
	//	}
	//}
	cHumanView::VRenderPrivate();
}
