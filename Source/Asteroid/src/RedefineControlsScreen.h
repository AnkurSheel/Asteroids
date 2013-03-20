// *****************************************************************************
//  RedefineControlsScreen   version:  1.0   Ankur Sheel  date: 2013/03/20
// *****************************************************************************
//  purpose:	
// *****************************************************************************
#ifndef RedefineControlsScreen_h__
#define RedefineControlsScreen_h__

#include "Game/GameFlowStates.h"

namespace Graphics
{
	struct stUIEventCallbackParam;
}

class cStateRedefineControlsScreen 
	: public IGameFlowStates
{
public:
	static cStateRedefineControlsScreen * Instance();
	~cStateRedefineControlsScreen();
	virtual void VOnEnter(cGame * pGame);
	virtual void VOnUpdate();
	virtual void VOnExit();
	virtual bool VOnMessage(const AI::Telegram & msg);

private:
	cStateRedefineControlsScreen();
	void BackButtonPressed(const Graphics::stUIEventCallbackParam& params);

private:
	unsigned int m_icurrentlyEditingID;


};
#endif // RedefineControlsScreen_h__