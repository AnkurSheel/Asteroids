// *****************************************************************************
//  PauseScreen   version:  1.0   Ankur Sheel  date: 2013/03/20
// *****************************************************************************
//  purpose:	
// *****************************************************************************
#include "stdafx.h"
#include "PauseScreen.h"
#include "Game\Game.h"
#include "HumanView.h"
#include "ControlStructures.h"
#include "BaseControl.hxx"
#include "Color.h"
#include "MessageDispatchManager.hxx"
#include "FSM\Telegram.h"
#include "Game\GameFlowStateMachine.h"
#include "OptionScreen.h"
#include "HelpScreen.h"

using namespace Base;
using namespace Graphics;
using namespace GameBase;
using namespace AI;

cStatePauseScreen::cStatePauseScreen()
	: m_pPauseScreen(NULL)
{
}

// *****************************************************************************
cStatePauseScreen::~cStatePauseScreen()
{
}

// *****************************************************************************
cStatePauseScreen* cStatePauseScreen::Instance()
{
	static cStatePauseScreen instance;
	return &instance;
}

// *****************************************************************************
void cStatePauseScreen::VOnEnter(cGame *pGame)
{
	IGameFlowStates::VOnEnter(pGame);
	if (pGame->m_pHumanView->m_pAppWindowControl != NULL)
	{
		cWindowControlDef pauseDef;
		pauseDef.strControlName = "PauseScreen";
		pauseDef.wType = cWindowControlDef::WT_STANDARD;
		pauseDef.vPosition = cVector2(0, 0);
		pauseDef.vSize = pGame->m_pHumanView->m_pAppWindowControl->VGetSize();
		pauseDef.strBGImageFile = "WindowBG.png";
		m_pPauseScreen = IBaseControl::CreateWindowControl(pauseDef);
		pGame->m_pHumanView->m_pAppWindowControl->VAddChildControl(shared_ptr<IBaseControl>(m_pPauseScreen));
		pGame->m_pHumanView->m_pAppWindowControl->VMoveToFront(m_pPauseScreen);

		cLabelControlDef def;
		def.strControlName = "LabelOptions";
		def.strFont = "JokerMan"; 
		def.textColor = cColor::RED;
		def.strText = "Pause";
		def.fTextHeight = 200;
		def.vPosition = cVector2(407.0f, 0.0f);
		IBaseControl * pTitleLabelControl = IBaseControl::CreateLabelControl(def);
		m_pPauseScreen->VAddChildControl(shared_ptr<IBaseControl>(pTitleLabelControl));

		cButtonControlDef buttonDef;
		buttonDef.strControlName = "btnOptions";
		buttonDef.bAutoSize = true;
		buttonDef.vPosition = cVector2(490, 220);
		buttonDef.strDefaultImage = "buttonDefault.png";
		buttonDef.strPressedImage = "buttonPressed.png";
		buttonDef.labelControlDef.strFont = "licorice";
		buttonDef.labelControlDef.strText = "Options";
		buttonDef.labelControlDef.textColor = cColor::BLUE;
		buttonDef.labelControlDef.fTextHeight = 70;

		IBaseControl * pOptionsButton = IBaseControl::CreateButtonControl(buttonDef);
		m_pPauseScreen->VAddChildControl(shared_ptr<IBaseControl>(pOptionsButton));
		UIEventCallBackFn callbackOptionsBtn;
		callbackOptionsBtn = bind(&cStatePauseScreen::OptionsButtonPressed, this, _1);
		pOptionsButton->VRegisterCallBack(UIET_BTNRELEASED, callbackOptionsBtn);

		buttonDef.strControlName = "btnHelp";
		buttonDef.bAutoSize = false;
		buttonDef.vSize = pOptionsButton->VGetSize();
		buttonDef.vPosition = cVector2(490, 320);
		buttonDef.labelControlDef.strText = "Help";

		IBaseControl * pHelpButton = IBaseControl::CreateButtonControl(buttonDef);
		m_pPauseScreen->VAddChildControl(shared_ptr<IBaseControl>(pHelpButton));
		UIEventCallBackFn callbackHelpBtn;
		callbackHelpBtn = bind(&cStatePauseScreen::HelpButtonPressed, this, _1);
		pHelpButton->VRegisterCallBack(UIET_BTNRELEASED, callbackHelpBtn);

		buttonDef.strControlName = "btnQuit";
		buttonDef.labelControlDef.strText = "Quit";
		buttonDef.vPosition = cVector2(490, 420);

		IBaseControl * pQuitButton = IBaseControl::CreateButtonControl(buttonDef);
		m_pPauseScreen->VAddChildControl(shared_ptr<IBaseControl>(pQuitButton));
		UIEventCallBackFn callbackQuitBtn;
		callbackQuitBtn = bind(&cStatePauseScreen::QuitButtonPressed, this, _1);
		pQuitButton->VRegisterCallBack(UIET_BTNRELEASED, callbackQuitBtn);

		buttonDef.strControlName = "btnBack";
		buttonDef.labelControlDef.strText = "Back";
		buttonDef.vPosition = cVector2(490, 520);

		IBaseControl * pBackButton = IBaseControl::CreateButtonControl(buttonDef);
		m_pPauseScreen->VAddChildControl(shared_ptr<IBaseControl>(pBackButton));
		UIEventCallBackFn callbackBackBtn;
		callbackBackBtn = bind(&cStatePauseScreen::BackButtonPressed, this, _1);
		pBackButton->VRegisterCallBack(UIET_BTNRELEASED, callbackBackBtn);
	}
}

// *****************************************************************************
void cStatePauseScreen::VOnUpdate()
{

}

// *****************************************************************************
void cStatePauseScreen::VOnExit()
{
	if (m_pOwner->m_pHumanView->m_pAppWindowControl != NULL)
	{
		m_pOwner->m_pHumanView->m_pAppWindowControl->VRemoveChildControl("PauseScreen");
	}
}

// *****************************************************************************
bool cStatePauseScreen::VOnMessage(const Telegram &msg)
{
	if(msg.Msg == MSG_ESCAPE_PRESSED)
	{
		stUIEventCallbackParam params;
		BackButtonPressed(params);
		return true;
	}
	return false;
}

// *****************************************************************************
void cStatePauseScreen::VOnPause()
{
	IGameFlowStates::VOnPause();
	if (m_pPauseScreen != NULL)
	{
		m_pPauseScreen->VSetVisible(false);
	}
}

// *****************************************************************************
void cStatePauseScreen::VOnResume()
{
	IGameFlowStates::VOnResume();
	if (m_pPauseScreen != NULL)
	{
		m_pPauseScreen->VSetVisible(true);
	}
}

// *****************************************************************************
void cStatePauseScreen::BackButtonPressed(const stUIEventCallbackParam & params)
{
	if(m_pOwner != NULL)
	{
		if(m_pOwner->m_pStateMachine != NULL)
		{
			m_pOwner->m_pStateMachine->RequestPopState();
		}
	}
}
// *****************************************************************************
void cStatePauseScreen::OptionsButtonPressed(const Graphics::stUIEventCallbackParam& params)
{
	if(m_pOwner != NULL && m_pOwner->m_pStateMachine != NULL)
	{
		m_pOwner->m_pStateMachine->RequestPushState(cStateOptionsScreen::Instance());
	}
}

// *****************************************************************************
void cStatePauseScreen::HelpButtonPressed(const Graphics::stUIEventCallbackParam& params)
{
	if(m_pOwner != NULL && m_pOwner->m_pStateMachine != NULL)
	{
		m_pOwner->m_pStateMachine->RequestPushState(cStateHelpScreen::Instance());
	}
}

// *****************************************************************************
void cStatePauseScreen::QuitButtonPressed(const Graphics::stUIEventCallbackParam& params)
{
	PostQuitMessage(0);
}
