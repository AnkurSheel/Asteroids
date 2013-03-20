// *****************************************************************************
//  OptionScreen   version:  1.0   Ankur Sheel  date: 2013/03/20
// *****************************************************************************
//  purpose:	
// *****************************************************************************
#ifndef OptionScreen_h__
#define OptionScreen_h__

#include "Game/GameFlowStates.h"

namespace Graphics
{
	class IBaseControl;
	struct stUIEventCallbackParam;
}

class cGame;

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

#endif // OptionScreen_h__