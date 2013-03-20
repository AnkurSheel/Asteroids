// *****************************************************************************
//  TitleScreen   version:  1.0   Ankur Sheel  date: 2013/03/20
// *****************************************************************************
//  purpose:	
// *****************************************************************************

#include "stdafx.h"
#include "TitleScreen.h"
#include "Game\Game.h"
#include "HumanView.h"
#include "ControlStructures.h"
#include "BaseControl.hxx"
#include "Color.h"
#include "MessageDispatchManager.hxx"
#include "FSM\Telegram.h"
#include "Game\GameFlowStateMachine.h"
#include "MenuScreen.h"

using namespace Base;
using namespace Graphics;
using namespace GameBase;
using namespace AI;

// *****************************************************************************
cStateTitleScreen::cStateTitleScreen()
{
}

// *****************************************************************************
cStateTitleScreen::~cStateTitleScreen()
{
}

// *****************************************************************************
cStateTitleScreen* cStateTitleScreen::Instance()
{
	static cStateTitleScreen instance;
	return &instance;
}

// *****************************************************************************
void cStateTitleScreen::VOnEnter(cGame *pGame)
{
	IGameFlowStates::VOnEnter(pGame);
	if (pGame->m_pHumanView->m_pAppWindowControl != NULL)
	{
		cWindowControlDef titleDef;
		titleDef.strControlName = "TitleScreen";
		titleDef.wType = cWindowControlDef::WT_STANDARD;
		titleDef.vSize = pGame->m_pHumanView->m_pAppWindowControl->VGetSize();
		IBaseControl * pTitleScreen = IBaseControl::CreateWindowControl(titleDef);
		pGame->m_pHumanView->m_pAppWindowControl->VAddChildControl(shared_ptr<IBaseControl>(pTitleScreen));

		cLabelControlDef def;
		def.strControlName = "TitleLabel";
		def.strFont = "JokerMan"; 
		def.textColor = cColor::RED;
		def.strText = "Asteroids";
		def.fTextHeight = 200;
		def.vPosition = cVector2(static_cast<float>(pGame->m_iDisplayWidth/4), 0.0f);
		IBaseControl * pLabelControl = IBaseControl::CreateLabelControl(def);
		pTitleScreen->VAddChildControl(shared_ptr<IBaseControl>(pLabelControl));
	}
	pGame->m_pHumanView->PlayMusic("mainmenu.ogg", true);

	IMessageDispatchManager::GetInstance()->VDispatchMessage(2.0f, pGame->VGetID(), pGame->VGetID(), MSG_SHOW_MENU, NULL);
}

// *****************************************************************************
void cStateTitleScreen::VOnUpdate()
{
}

// *****************************************************************************
void cStateTitleScreen::VOnExit()
{
}

// *****************************************************************************
bool cStateTitleScreen::VOnMessage(const Telegram &msg)
{
	if(msg.Msg == MSG_SHOW_MENU)
	{
		m_pOwner->m_pStateMachine->RequestChangeState(cStateMenuScreen::Instance());
		return true;
	}
	else if(msg.Msg == MSG_ESCAPE_PRESSED)
	{
		PostQuitMessage(0);
		return true;
	}
	return false;
}
