// *****************************************************************************
//  HelpScreen   version:  1.0   Ankur Sheel  date: 2013/03/20
// *****************************************************************************
//  purpose:	
// *****************************************************************************
#include "stdafx.h"
#include "HelpScreen.h"
#include "Game\Game.h"
#include "HumanView.h"
#include "ControlStructures.h"
#include "BaseControl.hxx"
#include "FSM\Telegram.h"
#include "Game\GameFlowStateMachine.h"

using namespace Base;
using namespace Graphics;
using namespace GameBase;
using namespace AI;

// *****************************************************************************
cStateHelpScreen::cStateHelpScreen()
{
}

// *****************************************************************************
cStateHelpScreen::~cStateHelpScreen()
{
}

// *****************************************************************************
cStateHelpScreen* cStateHelpScreen::Instance()
{
	static cStateHelpScreen instance;
	return &instance;
}

// *****************************************************************************
void cStateHelpScreen::VOnEnter(cGame *pGame)
{
	IGameFlowStates::VOnEnter(pGame);
	if (pGame->m_pHumanView->m_pAppWindowControl != NULL)
	{
		cWindowControlDef helpDef;
		helpDef.strControlName = "HelpScreen";
		helpDef.wType = cWindowControlDef::WT_STANDARD;
		helpDef.vPosition = cVector2(0, 0);
		helpDef.strBGImageFile = "WindowBG.png";
		helpDef.vSize = pGame->m_pHumanView->m_pAppWindowControl->VGetSize();
		IBaseControl * pHelpScreen = IBaseControl::CreateWindowControl(helpDef);
		pGame->m_pHumanView->m_pAppWindowControl->VAddChildControl(shared_ptr<IBaseControl>(pHelpScreen));
		pGame->m_pHumanView->m_pAppWindowControl->VMoveToFront(pHelpScreen);

		cLabelControlDef def;
		def.strControlName = "LabelHelp";
		def.strFont = "JokerMan"; 
		def.textColor = cColor::RED;
		def.strText = "Help";
		def.fTextHeight = 200;
		def.vPosition = cVector2(457.0f, 0.0f);
		IBaseControl * pTitleLabelControl = IBaseControl::CreateLabelControl(def);
		pHelpScreen->VAddChildControl(shared_ptr<IBaseControl>(pTitleLabelControl));

		def.strControlName = "labelHelpText";
		def.strFont = "licorice"; 
		def.textColor = cColor::VIOLET;
		def.strText = "Default Keys\n";
		def.strText += "\nPress W to Move forward\nPress S to Move Backward";
		def.strText += "\nPress A to Rotate Left\nPress D to Rotate Right";
		def.strText += "\nPress Space to Fire";
		def.fTextHeight = 30;
		def.vPosition = cVector2(0, 170);
		IBaseControl * pHelpTextLabelControl = IBaseControl::CreateLabelControl(def);
		pHelpScreen->VAddChildControl(shared_ptr<IBaseControl>(pHelpTextLabelControl));

		cButtonControlDef buttonDef;
		buttonDef.strControlName = "btnBack";
		buttonDef.bAutoSize = true;
		buttonDef.vPosition = cVector2(0, 480);
		buttonDef.strDefaultImage = "buttonDefault.png";
		buttonDef.strPressedImage = "buttonPressed.png";
		buttonDef.labelControlDef.strFont = "licorice";
		buttonDef.labelControlDef.strText = "Back";
		buttonDef.labelControlDef.textColor = cColor::BLUE;
		buttonDef.labelControlDef.fTextHeight = 50;

		IBaseControl * pBackButton = IBaseControl::CreateButtonControl(buttonDef);
		pHelpScreen->VAddChildControl(shared_ptr<IBaseControl>(pBackButton));
		UIEventCallBackFn callBackBtn;
		callBackBtn = bind(&cStateHelpScreen::BackButtonPressed, this, _1);
		pBackButton->VRegisterCallBack(UIET_BTNRELEASED, callBackBtn);
	}
}

// *****************************************************************************
void cStateHelpScreen::VOnUpdate()
{

}

// *****************************************************************************
void cStateHelpScreen::VOnExit()
{
	if (m_pOwner->m_pHumanView->m_pAppWindowControl != NULL)
	{
		m_pOwner->m_pHumanView->m_pAppWindowControl->VRemoveChildControl("HelpScreen");
	}
}

// *****************************************************************************
bool cStateHelpScreen::VOnMessage(const Telegram &msg)
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
void cStateHelpScreen::BackButtonPressed(const stUIEventCallbackParam & params)
{
	if(m_pOwner != NULL && m_pOwner->m_pStateMachine != NULL)
	{
		m_pOwner->m_pStateMachine->RequestPopState();
	}
}
