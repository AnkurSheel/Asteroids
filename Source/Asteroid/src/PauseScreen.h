// *****************************************************************************
//  PauseScreen   version:  1.0   Ankur Sheel  date: 2013/03/20
// *****************************************************************************
//  purpose:	
// *****************************************************************************
#ifndef PauseScreen_h__
#define PauseScreen_h__

#include "Game/GameFlowStates.h"

namespace Graphics
{
	class IBaseControl;
	struct stUIEventCallbackParam;
}

class cGame;

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

#endif // PauseScreen_h__