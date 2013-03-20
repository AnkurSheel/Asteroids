// *****************************************************************************
//  HelpScreen   version:  1.0   Ankur Sheel  date: 2013/03/20
// *****************************************************************************
//  purpose:	
// *****************************************************************************
#ifndef HelpScreen_h__
#define HelpScreen_h__

#include "Game/GameFlowStates.h"

namespace Graphics
{
	class IBaseControl;
	struct stUIEventCallbackParam;
}

class cGame;

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

#endif // HelpScreen_h__