// *****************************************************************************
//  RedefineControlsScreen   version:  1.0   Ankur Sheel  date: 2013/03/20
// *****************************************************************************
//  purpose:	
// *****************************************************************************
#include "stdafx.h"
#include "RedefineControlsScreen.h"

// *****************************************************************************
cStateRedefineControlsScreen::cStateRedefineControlsScreen()
{

}

// *****************************************************************************
cStateRedefineControlsScreen::~cStateRedefineControlsScreen()
{

}

// *****************************************************************************
cStateRedefineControlsScreen * cStateRedefineControlsScreen::Instance()
{
	static cStateRedefineControlsScreen instance;
	return &instance;
}

// *****************************************************************************
void cStateRedefineControlsScreen::VOnEnter(cGame *)
{
	throw std::exception("The method or operation is not implemented.");
}

// *****************************************************************************
void cStateRedefineControlsScreen::VOnUpdate()
{
	throw std::exception("The method or operation is not implemented.");
}

// *****************************************************************************
void cStateRedefineControlsScreen::VOnExit()
{
	throw std::exception("The method or operation is not implemented.");
}

// *****************************************************************************
bool cStateRedefineControlsScreen::VOnMessage(const AI::Telegram & msg)
{
	throw std::exception("The method or operation is not implemented.");
}
