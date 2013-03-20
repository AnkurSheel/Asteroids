// *****************************************************************************
//  TitleScreen   version:  1.0   Ankur Sheel  date: 2013/03/20
// *****************************************************************************
//  purpose:	
// *****************************************************************************
#ifndef TitleScreen_h__
#define TitleScreen_h__

#include "Game/GameFlowStates.h"

namespace Graphics
{
	class IBaseControl;
}

class cGame;

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


#endif // TitleScreen_h__