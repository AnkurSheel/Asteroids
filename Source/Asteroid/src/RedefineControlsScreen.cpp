// *****************************************************************************
//  RedefineControlsScreen   version:  1.0   Ankur Sheel  date: 2013/03/20
// *****************************************************************************
//  purpose:	
// *****************************************************************************
#include "stdafx.h"
#include "RedefineControlsScreen.h"
#include "Game\Game.h"
#include "HumanView.h"
#include "ControlStructures.h"
#include "BaseControl.hxx"
#include "Color.h"
#include "MessageDispatchManager.hxx"
#include "FSM\Telegram.h"
#include "Game\GameFlowStateMachine.h"
#include "GameControls.h"
#include "Optional.h"

using namespace Base;
using namespace Graphics;
using namespace GameBase;
using namespace AI;
using namespace Utilities;

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
void cStateRedefineControlsScreen::VOnEnter(cGame * pGame)
{
	IGameFlowStates::VOnEnter(pGame);
	if (pGame->m_pHumanView->m_pAppWindowControl != NULL)
	{
		cWindowControlDef redefineControlsDef;
		redefineControlsDef.strControlName = "RedefineControlsScreen";
		redefineControlsDef.wType = cWindowControlDef::WT_STANDARD;
		redefineControlsDef.vPosition = cVector2(0, 0);
		redefineControlsDef.strBGImageFile = "WindowBG.png";
		redefineControlsDef.vSize = pGame->m_pHumanView->m_pAppWindowControl->VGetSize();
		IBaseControl * pRedefineControlsScreen = IBaseControl::CreateWindowControl(redefineControlsDef);
		pGame->m_pHumanView->m_pAppWindowControl->VAddChildControl(shared_ptr<IBaseControl>(pRedefineControlsScreen));
		pGame->m_pHumanView->m_pAppWindowControl->VMoveToFront(pRedefineControlsScreen);

		cLabelControlDef def;
		def.strControlName = "LabelRedefineControls";
		def.strFont = "JokerMan"; 
		def.textColor = cColor::RED;
		def.strText = "Controls";
		def.fTextHeight = 200;
		def.vPosition = cVector2(257.0f, 0.0f);
		IBaseControl * pTitleLabelControl = IBaseControl::CreateLabelControl(def);
		pRedefineControlsScreen->VAddChildControl(shared_ptr<IBaseControl>(pTitleLabelControl));

		def.bAutoSize = false;
		def.strBGImageFile = "TextBox.png";
		def.strFont = "licorice"; 
		def.textColor = cColor::BLUE;
		def.fTextHeight = 30;
		def.vSize = cVector2(250.0f, 30.0f);

		cButtonControlDef buttonDef;
		buttonDef.strControlName = "btnBack";
		buttonDef.bAutoSize = false;
		buttonDef.strDefaultImage = "buttonDefault.png";
		buttonDef.strPressedImage = "buttonPressed.png";
		buttonDef.labelControlDef.strFont = "licorice";
		buttonDef.labelControlDef.textColor = cColor::BLUE;
		buttonDef.labelControlDef.fTextHeight = 30;
		buttonDef.vSize = cVector2(200.0f, 30.0f);

		cGameControls::KeyMapping keyMap = pGame->m_pGameControls->GetKeyMap();
		cGameControls::KeyMapping::iterator iter;
		int i = 0;
		int currentPosY = 220;
		for(iter = keyMap.begin(); iter != keyMap.end(); iter++)
		{
			stGameControl gameControl = iter->second;

			def.strControlName = cString(100, "Name%d", i);
			def.strText = gameControl.m_strDisplayName;
			def.vPosition = cVector2(0.0f, currentPosY);
			IBaseControl * pNameControl = IBaseControl::CreateLabelControl(def);
			pRedefineControlsScreen->VAddChildControl(shared_ptr<IBaseControl>(pNameControl));

			buttonDef.strControlName = cString(100, "Key%d", i);
			buttonDef.labelControlDef.strText = pGame->m_pGameControls->GetKeyName(gameControl.m_uiKeyCode);
			buttonDef.vPosition = cVector2(300.0f, currentPosY);
			IBaseControl * pKeyControl = IBaseControl::CreateButtonControl(buttonDef);
			pRedefineControlsScreen->VAddChildControl(shared_ptr<IBaseControl>(pKeyControl));
			
			unsigned int id = iter->first;
			UIEventCallBackFn btnClick = [pKeyControl, this, id] (const stUIEventCallbackParam & params)
			{
				m_icurrentlyEditingID = id;
				pKeyControl->VSetText("Press Any Key");
			};
			pKeyControl->VRegisterCallBack(UIET_BTNRELEASED, btnClick);

			UIEventCallBackFn  btnKeyDown = [pKeyControl, this, &gameControl] (const stUIEventCallbackParam & params)
			{
				pKeyControl->VSetText(m_pOwner->m_pGameControls->GetKeyName(params.uiCharId));
				Log_Write_L1(ILogger::LT_DEBUG, cString(10, "%d", m_icurrentlyEditingID));
				//keyMap[m_icurrentlyEditingID].m_uiKeyCode =iCode;
			};
			pKeyControl->VRegisterCallBack(UIET_ONKEYDOWN, btnKeyDown);

			i++;
			currentPosY += 40;
		}

		buttonDef.strControlName = "btnBack";
		buttonDef.bAutoSize = true;
		buttonDef.vPosition = cVector2(0, 650);
		buttonDef.labelControlDef.strText = "Back";
		buttonDef.labelControlDef.fTextHeight = 50;

		IBaseControl * pBackButton = IBaseControl::CreateButtonControl(buttonDef);
		pRedefineControlsScreen->VAddChildControl(shared_ptr<IBaseControl>(pBackButton));
		UIEventCallBackFn callBackBtn;
		callBackBtn = bind(&cStateRedefineControlsScreen::BackButtonPressed, this, _1);
		pBackButton->VRegisterCallBack(UIET_BTNRELEASED, callBackBtn);
	}
}

// *****************************************************************************
void cStateRedefineControlsScreen::VOnUpdate()
{
	
}

// *****************************************************************************
void cStateRedefineControlsScreen::VOnExit()
{
	if (m_pOwner->m_pHumanView->m_pAppWindowControl != NULL)
	{
		m_pOwner->m_pHumanView->m_pAppWindowControl->VRemoveChildControl("RedefineControlsScreen");
	}
}

// *****************************************************************************
bool cStateRedefineControlsScreen::VOnMessage(const AI::Telegram & msg)
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
void cStateRedefineControlsScreen::BackButtonPressed(const stUIEventCallbackParam & params)
{
	if(m_pOwner != NULL && m_pOwner->m_pStateMachine != NULL)
	{
		m_pOwner->m_pStateMachine->RequestPopState();
	}
}
