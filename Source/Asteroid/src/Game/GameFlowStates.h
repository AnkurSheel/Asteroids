// *****************************************************************************
//  GameFlowStates   version:  1.0   Ankur Sheel  date: 05/24/2008
//  ----------------------------------------------------------------------------
//  
//  ----------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// *****************************************************************************
// 
// *****************************************************************************
#ifndef GameFlowStates_h__
#define GameFlowStates_h__

#include "fsm/State.h"

namespace Graphics
{
	class IBaseControl;
	struct stUIEventCallbackParam;
}

class cGame;
class cAsteroidGameElement;

class IGameFlowStates
	: public AI::cState<cGame>
{
};

class cStateTitleScreen 
	: public IGameFlowStates
{
public:
	~cStateTitleScreen();
	static cStateTitleScreen *Instance();
	void VOnEnter(cGame *pGame);
	void VOnUpdate();
	void VOnExit();
	bool VOnMessage(const AI::Telegram &msg);
private:
	cStateTitleScreen();

private:
	Graphics::IBaseControl * m_pTitleScreen; 
};

class cStateMenuScreen 
	: public IGameFlowStates
{
public:
	~cStateMenuScreen();
	static cStateMenuScreen *Instance();
	void VOnEnter(cGame *pGame);
	void VOnUpdate();
	void VOnExit();
	void VOnPause();
	void VOnResume();
	bool VOnMessage(const AI::Telegram &msg);

private:
	cStateMenuScreen();
	void SinglePlayerButtonPressed(const Graphics::stUIEventCallbackParam& params);
	void HelpButtonPressed(const Graphics::stUIEventCallbackParam& params);
	void OptionsButtonPressed(const Graphics::stUIEventCallbackParam& params);
	void QuitButtonPressed(const Graphics::stUIEventCallbackParam& params);

private:
	Graphics::IBaseControl * m_pMenuScreen;
	
};

class cStatePlayGame 
	: public IGameFlowStates
{
public:
	virtual ~cStatePlayGame();
	static cStatePlayGame *Instance();
	virtual void VOnEnter(cGame *pGame);
	virtual void VOnUpdate();
	virtual void VOnExit();
	virtual bool VOnMessage(const AI::Telegram &msg);
private:
	cStatePlayGame();
	void CheckForCollisions(cAsteroidGameElement * const pAsteriodElement);
};

class cStateHelpScreen 
	: public IGameFlowStates
{
public:
	virtual ~cStateHelpScreen();
	static cStateHelpScreen *Instance();
	virtual void VOnEnter(cGame *pGame);
	virtual void VOnUpdate();
	virtual void VOnExit();
	virtual bool VOnMessage(const AI::Telegram &msg);

private:
	cStateHelpScreen();
	void BackButtonPressed(const Graphics::stUIEventCallbackParam& params);
};

class cStateOptionsScreen 
	: public IGameFlowStates
{
public:
	virtual ~cStateOptionsScreen();
	static cStateOptionsScreen *Instance();
	virtual void VOnEnter(cGame *pGame);
	virtual void VOnUpdate();
	virtual void VOnExit();
	virtual bool VOnMessage(const AI::Telegram &msg);

private:
	cStateOptionsScreen();
	void BackButtonPressed(const Graphics::stUIEventCallbackParam& params);
	void MusicScrollbarChanged(const Graphics::stUIEventCallbackParam& params);
	void SFXScrollbarChanged(const Graphics::stUIEventCallbackParam& params);

private:
	Graphics::IBaseControl * m_pOptionsScreen;
};

class cStatePauseScreen 
	: public IGameFlowStates
{
public:
	virtual ~cStatePauseScreen();
	static cStatePauseScreen *Instance();
	virtual void VOnEnter(cGame *pGame);
	virtual void VOnUpdate();
	virtual void VOnExit();
	void VOnPause();
	void VOnResume();
	virtual bool VOnMessage(const AI::Telegram &msg);

private:
	cStatePauseScreen();
	void OptionsButtonPressed(const Graphics::stUIEventCallbackParam& params);
	void HelpButtonPressed(const Graphics::stUIEventCallbackParam& params);
	void QuitButtonPressed(const Graphics::stUIEventCallbackParam& params);
	void BackButtonPressed(const Graphics::stUIEventCallbackParam& params);

private:
	Graphics::IBaseControl * m_pPauseScreen;
};
#endif // GameFlowStates_h__