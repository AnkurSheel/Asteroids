// *****************************************************************************
//  Game   version:  1.0   ·  date: 04/30/2008
//  ----------------------------------------------------------------------------
//  
//  ----------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// *****************************************************************************
// 
// *****************************************************************************
#ifndef Game_h__
#define Game_h__

#include "Game.hxx"
#include "BaseApp.h"

namespace GameBase
{
	class cHumanView;
	class cGameElementDef;
	class cHighScoreTable;
}

namespace Utilities
{
	class IRandomGenerator;
}

namespace MySound
{
	class ISound;
}

namespace Graphics
{
	class IBaseControl;
}

class cAsteroidGameElement;
class cGameFlowStateMachine;

class cGame 
	: public IGame
	, public GameBase::cBaseApp
{
public:
	cGame(const Base::cString strName);
	void VGetGameElements(GameElementList & gameElements) const;
	Base::cVector3 VGetScreenTopLeftPos() const;
	Base::cVector3 VGetScreenBottomRightPos() const;
	void MoveShip(const ShipActions eShipActions);
	Utilities::IRandomGenerator * const GetRandomGenerator() const;
	void AddGameElement(shared_ptr<cAsteroidGameElement> const pGameElement);
	void AddAsteroid(const GameBase::cGameElementDef & asteroidDef, const int iSize);

private:
	~cGame();
	void VOnInitialization(const HINSTANCE & hInstance, const int nCmdShow,
		const Base::cString & strOptionsFile);
	Base::cString VGetGameTitle() const;
	void VOnUpdate();
	void VCleanup();
	bool VOnHandleMessage(const AI::Telegram & telegram);
	void VCreateHumanView();
	void AsteroidHitByBullet(cAsteroidGameElement * const pBulletElement,
		cAsteroidGameElement * const pAsteroidElement);
	void ShipHitByAsteroid();
	void OnRestart();
	void NextLevel();

private:
	int										m_iDisplayHeight ;		// the display height of the window
	int										m_iDisplayWidth ;		// the display width of the window
	GameElementList							m_pGameElements;		// ptr to the gameelements
	cGameFlowStateMachine *					m_pStateMachine;
	Base::cVector3							m_vScreenTopLeftPos;
	Base::cVector3							m_vScreenBottomRightPos;
	Utilities::IRandomGenerator *			m_pRandomGenerator;
	shared_ptr<Graphics::IBaseControl>		m_pHUDScreen;
	bool									m_bGameOver;
	int										m_iCurrentLevel;
	int										m_iNumberOfAsteroids;
	GameBase::cHighScoreTable *				m_pHighScoreTable;

private:
	friend class cStateTitleScreen;
	friend class cStateMenuScreen;
	friend class cStatePlayGame;
	friend class cStateHelpScreen;
	friend class cStateOptionsScreen;
	friend class cStatePauseScreen;
	friend class cStateHighScoreScreen;
	friend class cStateEnterNameScreen;
	friend class cAsteroidView;
};
#endif // Game_h__