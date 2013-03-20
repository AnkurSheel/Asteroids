// *****************************************************************************
//  EnterNameScreen   version:  1.0   Ankur Sheel  date: 2013/03/20
// *****************************************************************************
//  purpose:	
// *****************************************************************************
#ifndef EnterNameScreen_h__
#define EnterNameScreen_h__

#include "Game/GameFlowStates.h"

namespace Graphics
{
	class IBaseControl;
	struct stUIEventCallbackParam;
}

class cStateEnterNameScreen 
	: public IGameFlowStates
{
public:
	virtual ~cStateEnterNameScreen();
	static cStateEnterNameScreen *Instance();
	virtual void VOnEnter(cGame * pGame);
	virtual void VOnUpdate();
	virtual void VOnExit();
	virtual bool VOnMessage( const AI::Telegram & msg );
	void SetNewScorePos(int val);
	void SetNewScore(int val);

private:
	cStateEnterNameScreen();
	void BackButtonPressed(const Graphics::stUIEventCallbackParam& params);
	void NameEntered(const Graphics::stUIEventCallbackParam& params);

private:
	int				m_iNewScorePos;
	int				m_iNewScore;
	Base::cString	m_strPlayerName;
};

class cGame;

#endif // EnterNameScreen_h__