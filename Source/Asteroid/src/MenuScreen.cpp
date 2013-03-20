// *****************************************************************************
//  MenuScreen   version:  1.0   Ankur Sheel  date: 2013/03/20
// *****************************************************************************
//  purpose:	
// *****************************************************************************
#include "stdafx.h"
#include "MenuScreen.h"
#include "Game\Game.h"
#include "HumanView.h"
#include "ControlStructures.h"
#include "BaseControl.hxx"
#include "FSM\Telegram.h"
#include "Game\GameFlowStateMachine.h"
#include "AsteroidView.h"
#include "StatePlayGame.h"
#include "HelpScreen.h"
#include "OptionScreen.h"
#include "HighScoreScreen.h"
#include "RedefineControlsScreen.h"

using namespace Base;
using namespace Graphics;
using namespace GameBase;
using namespace AI;

// *****************************************************************************
cStateMenuScreen::cStateMenuScreen()
	: m_pMenuScreen(NULL)
{
}

// *****************************************************************************
cStateMenuScreen::~cStateMenuScreen()
{
}

// *****************************************************************************
cStateMenuScreen* cStateMenuScreen::Instance()
{
	static cStateMenuScreen instance;
	return &instance;
}

// *****************************************************************************
void cStateMenuScreen::VOnEnter(cGame *pGame)
{
	IGameFlowStates::VOnEnter(pGame);
	if (pGame->m_pHumanView->m_pAppWindowControl != NULL)
	{
		cWindowControlDef menuDef;
		menuDef.strControlName = "MenuScreen";
		menuDef.wType = cWindowControlDef::WT_STANDARD;
		menuDef.vPosition = cVector2(0, 0);
		menuDef.vSize = pGame->m_pHumanView->m_pAppWindowControl->VGetSize();
		m_pMenuScreen = IBaseControl::CreateWindowControl(menuDef);
		pGame->m_pHumanView->m_pAppWindowControl->VAddChildControl(shared_ptr<IBaseControl>(m_pMenuScreen));
		pGame->m_pHumanView->m_pAppWindowControl->VMoveToFront(m_pMenuScreen);

		cButtonControlDef buttonDef;
		buttonDef.strControlName = "btnSinglePlayer";
		buttonDef.vPosition = cVector2(412, 270);
		buttonDef.strDefaultImage = "buttonDefault.png";
		buttonDef.strPressedImage = "buttonPressed.png";
		buttonDef.labelControlDef.strFont = "licorice";
		buttonDef.labelControlDef.strText = "Play";
		buttonDef.labelControlDef.textColor = cColor::BLUE;
		buttonDef.labelControlDef.fTextHeight = 70;
		buttonDef.vSize = cVector2(350.0f, 70.0f);

		IBaseControl * pPlayButton = IBaseControl::CreateButtonControl(buttonDef);
		m_pMenuScreen->VAddChildControl(shared_ptr<IBaseControl>(pPlayButton));
		UIEventCallBackFn callbackPlayBtn;
		callbackPlayBtn = bind(&cStateMenuScreen::PlayButtonPressed, this, _1);
		pPlayButton->VRegisterCallBack(UIET_BTNRELEASED, callbackPlayBtn);

		buttonDef.strControlName = "btnOption";
		buttonDef.vSize = pPlayButton->VGetSize();
		buttonDef.labelControlDef.strText = "Options";
		buttonDef.vPosition = cVector2(412, 370);

		IBaseControl * pOptionsButton = IBaseControl::CreateButtonControl(buttonDef);
		m_pMenuScreen->VAddChildControl(shared_ptr<IBaseControl>(pOptionsButton));
		UIEventCallBackFn callbackOptionsBtn;
		callbackOptionsBtn = bind(&cStateMenuScreen::OptionsButtonPressed, this, _1);
		pOptionsButton->VRegisterCallBack(UIET_BTNRELEASED, callbackOptionsBtn);

		buttonDef.strControlName = "btnControls";
		buttonDef.labelControlDef.strText = "Controls";
		buttonDef.vPosition = cVector2(412, 470);

		IBaseControl * pRedefineControlsButton = IBaseControl::CreateButtonControl(buttonDef);
		m_pMenuScreen->VAddChildControl(shared_ptr<IBaseControl>(pRedefineControlsButton));
		UIEventCallBackFn callbackRedefineControlsBtn = bind(&cStateMenuScreen::ControlsButtonPressed, this, _1);
		pRedefineControlsButton->VRegisterCallBack(UIET_BTNRELEASED, callbackRedefineControlsBtn);

		buttonDef.strControlName = "btnHighScore";
		buttonDef.labelControlDef.strText = "High Score";
		buttonDef.vPosition = cVector2(412, 570);

		IBaseControl * pHighScoreButton = IBaseControl::CreateButtonControl(buttonDef);
		m_pMenuScreen->VAddChildControl(shared_ptr<IBaseControl>(pHighScoreButton));
		UIEventCallBackFn callbackHighScoreBtn = bind(&cStateMenuScreen::HighScoreButtonPressed, this, _1);
		pHighScoreButton->VRegisterCallBack(UIET_BTNRELEASED, callbackHighScoreBtn);

		buttonDef.strControlName = "btnQuit";
		buttonDef.labelControlDef.strText = "Quit";
		buttonDef.vPosition = cVector2(412, 670);

		IBaseControl * pQuitButton = IBaseControl::CreateButtonControl(buttonDef);
		m_pMenuScreen->VAddChildControl(shared_ptr<IBaseControl>(pQuitButton));
		UIEventCallBackFn callbackQuitBtn;
		callbackQuitBtn = bind(&cStateMenuScreen::QuitButtonPressed, this, _1);
		pQuitButton->VRegisterCallBack(UIET_BTNRELEASED, callbackQuitBtn);
	}
}

// *****************************************************************************
void cStateMenuScreen::VOnUpdate()
{
}

// *****************************************************************************
void cStateMenuScreen::VOnExit()
{
	if (m_pOwner->m_pHumanView->m_pAppWindowControl != NULL)
	{
		m_pOwner->m_pHumanView->m_pAppWindowControl->VRemoveChildControl("TitleScreen");
		m_pOwner->m_pHumanView->m_pAppWindowControl->VRemoveChildControl("MenuScreen");
	}
}

// *****************************************************************************
bool cStateMenuScreen::VOnMessage(const Telegram &msg)
{
	if(msg.Msg == MSG_ESCAPE_PRESSED)
	{
		PostQuitMessage(0);
		return true;
	}
	return false;
}

// *****************************************************************************
void cStateMenuScreen::VOnPause()
{
	IGameFlowStates::VOnPause();
	if (m_pMenuScreen != NULL)
	{
		m_pMenuScreen->VSetVisible(false);
	}
}

// *****************************************************************************
void cStateMenuScreen::VOnResume()
{
	IGameFlowStates::VOnResume();
	if (m_pMenuScreen != NULL)
	{
		m_pMenuScreen->VSetVisible(true);
	}
}

// *****************************************************************************
void cStateMenuScreen::PlayButtonPressed(const stUIEventCallbackParam & params)
{
	cAsteroidView * pView = dynamic_cast<cAsteroidView *>(m_pOwner->m_pHumanView);
	if(pView)
		pView->OnPlaySelected(m_pOwner);

	if (m_pOwner && m_pOwner->m_pStateMachine)
	{
		m_pOwner->m_pStateMachine->RequestChangeState(cStatePlayGame::Instance());
	}
}

// *****************************************************************************
void cStateMenuScreen::ControlsButtonPressed(const stUIEventCallbackParam & params)
{
	if(m_pOwner != NULL && m_pOwner->m_pStateMachine != NULL)
	{
		m_pOwner->m_pStateMachine->RequestPushState(cStateRedefineControlsScreen::Instance());
	}
}

// *****************************************************************************
void cStateMenuScreen::OptionsButtonPressed(const stUIEventCallbackParam & params)
{
	if(m_pOwner != NULL && m_pOwner->m_pStateMachine != NULL)
	{
		m_pOwner->m_pStateMachine->RequestPushState(cStateOptionsScreen::Instance());
	}
}

// *****************************************************************************
void cStateMenuScreen::HighScoreButtonPressed(const stUIEventCallbackParam & params)
{
	if(m_pOwner != NULL && m_pOwner->m_pStateMachine != NULL)
	{
		m_pOwner->m_pStateMachine->RequestPushState(cStateHighScoreScreen::Instance());
	}
}

// *****************************************************************************
void cStateMenuScreen::QuitButtonPressed(const stUIEventCallbackParam & params)
{
	PostQuitMessage(0);
}
