// *****************************************************************************
//  AsteroidView   version:  1.0   Ankur Sheel  date: 2013/02/21
//  ----------------------------------------------------------------------------
//  
//  ----------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// *****************************************************************************
// 
// *****************************************************************************
#ifndef AsteroidView_h__
#define AsteroidView_h__

#include "HumanView.h"

class cGame;
class ShipHandler;

namespace GameBase
{
	class IBaseApp;
}

class cAsteroidView 
	: public GameBase::cHumanView
{
public:
	cAsteroidView();
	~cAsteroidView();
	void VOnCreateDevice(GameBase::IBaseApp * pGame,
		const HINSTANCE & hInst, const HWND & hWnd);
	void VOnUpdate(const TICK tickCurrent, const float fElapsedTime);
	void VRenderPrivate();
	bool VOnMsgProc( const Base::AppMsg & msg );	
	void OnSinglePlayerSelected(cGame * pGame);

private:
	shared_ptr<ShipHandler>		m_ShipHandler;
	cGame	*					m_pGame;
};
#endif // AsteroidView_h__