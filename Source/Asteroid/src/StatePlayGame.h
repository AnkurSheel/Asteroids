// *****************************************************************************
//  StatePlayGame   version:  1.0   Ankur Sheel  date: 2013/03/20
// *****************************************************************************
//  purpose:	
// *****************************************************************************
#ifndef StatePlayGame_h__
#define StatePlayGame_h__

#include "Game/GameFlowStates.h"

class cGame;
class cAsteroidGameElement;

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


#endif // StatePlayGame_h__