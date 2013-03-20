// *****************************************************************************
//  HighScoreScreen   version:  1.0   Ankur Sheel  date: 2013/03/20
// *****************************************************************************
//  purpose:	
// *****************************************************************************
#ifndef HighScoreScreen_h__
#define HighScoreScreen_h__

#include "Game/GameFlowStates.h"

namespace Graphics
{
	class IBaseControl;
	struct stUIEventCallbackParam;
}

class cGame;

class cStateHighScoreScreen 
	: public IGameFlowStates
{
public:
	virtual ~cStateHighScoreScreen();
	static cStateHighScoreScreen *Instance();
	virtual void VOnEnter(cGame *pGame);
	virtual void VOnUpdate();
	virtual void VOnExit();
	virtual bool VOnMessage(const AI::Telegram &msg);

private:
	cStateHighScoreScreen();
	void BackButtonPressed(const Graphics::stUIEventCallbackParam& params);
};

#endif // HighScoreScreen_h__