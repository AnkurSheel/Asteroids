// ***************************************************************
//  GameFlowStateMachine   version:  1.0   Ankur Sheel  date: 2011/01/21
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#include "stdafx.h"
#include "GameFlowStateMachine.h"
#include "GameFlowStates.h"

cGameFlowStateMachine::cGameFlowStateMachine(cGame *pGame)
: cStateMachine<cGame>(pGame)
{
}
// ***************************************************************

cGameFlowStateMachine::~cGameFlowStateMachine(void)
{
}
// ***************************************************************