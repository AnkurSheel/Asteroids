// ***************************************************************
//  GameFlowStateMachine   version:  1.0   Ankur Sheel  date: 2011/01/21
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef GameFlowStateMachine_h__
#define GameFlowStateMachine_h__

#include "fsm\statemachine.h"
#include "Game.h"

class IGameFlowStates;

class cGameFlowStateMachine :
	public AI::cStateMachine<cGame>
{
public:
	cGameFlowStateMachine(cGame *pGame);
	~cGameFlowStateMachine(void);
};
#endif // GameFlowStateMachine_h__
