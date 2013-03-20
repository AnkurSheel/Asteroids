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
	: m_pRedefineControlsScreen(NULL)
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
		m_pRedefineControlsScreen = IBaseControl::CreateWindowControl(redefineControlsDef);
		pGame->m_pHumanView->m_pAppWindowControl->VAddChildControl(shared_ptr<IBaseControl>(m_pRedefineControlsScreen));
		pGame->m_pHumanView->m_pAppWindowControl->VMoveToFront(m_pRedefineControlsScreen);

		cLabelControlDef def;
		def.strControlName = "LabelRedefineControls";
		def.strFont = "JokerMan"; 
		def.textColor = cColor::RED;
		def.strText = "Controls";
		def.fTextHeight = 200;
		def.vPosition = cVector2(257.0f, 0.0f);
		IBaseControl * pTitleLabelControl = IBaseControl::CreateLabelControl(def);
		m_pRedefineControlsScreen->VAddChildControl(shared_ptr<IBaseControl>(pTitleLabelControl));

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
			m_pRedefineControlsScreen->VAddChildControl(shared_ptr<IBaseControl>(pNameControl));

			buttonDef.strControlName = cString(100, "Key%d", i);
			buttonDef.labelControlDef.strText = pGame->m_pGameControls->GetKeyName(gameControl.m_uiKeyCode);
			buttonDef.vPosition = cVector2(300.0f, currentPosY);
			IBaseControl * pKeyControl = IBaseControl::CreateButtonControl(buttonDef);
			m_pRedefineControlsScreen->VAddChildControl(shared_ptr<IBaseControl>(pKeyControl));

			unsigned int id = iter->first;
			m_pKeyControls.push_back(pKeyControl);
			UIEventCallBackFn btnClick = [this, pKeyControl, id] (const stUIEventCallbackParam & params)
			{
				m_icurrentlyEditingID = id;
				m_pKeyControls[m_icurrentlyEditingID]->VSetText("Press Any Key");
			};
			pKeyControl->VRegisterCallBack(UIET_BTNRELEASED, btnClick);

			UIEventCallBackFn  btnKeyDown = [this] (const stUIEventCallbackParam & params)
			{
				this->OnKeyChanged(params.uiCharId);
			};
			pKeyControl->VRegisterCallBack(UIET_ONKEYDOWN, btnKeyDown);

			i++;
			currentPosY += 40;
		}

		buttonDef.strControlName = "btnOk";
		buttonDef.vPosition = cVector2(100, 500);
		buttonDef.vSize = cVector2(120.0f, 50.0f);
		buttonDef.labelControlDef.strText = "OK";
		buttonDef.labelControlDef.fTextHeight = 50;

		IBaseControl * pOkBtn = IBaseControl::CreateButtonControl(buttonDef);
		m_pRedefineControlsScreen->VAddChildControl(shared_ptr<IBaseControl>(pOkBtn));
		UIEventCallBackFn cbOkBtn;
		cbOkBtn = bind(&cStateRedefineControlsScreen::OKButtonPressed, this, _1);
		pOkBtn->VRegisterCallBack(UIET_BTNRELEASED, cbOkBtn);

		buttonDef.strControlName = "btnReset";
		buttonDef.vPosition = cVector2(250, 500);
		buttonDef.labelControlDef.strText = "Reset";

		IBaseControl * pResetBtn = IBaseControl::CreateButtonControl(buttonDef);
		m_pRedefineControlsScreen->VAddChildControl(shared_ptr<IBaseControl>(pResetBtn));
		UIEventCallBackFn cbResetBtn;
		cbResetBtn = bind(&cStateRedefineControlsScreen::ResetButtonPressed, this, _1);
		pResetBtn->VRegisterCallBack(UIET_BTNRELEASED, cbResetBtn);

		buttonDef.strControlName = "btnCancel";
		buttonDef.vPosition = cVector2(400, 500);
		buttonDef.labelControlDef.strText = "Cancel";

		IBaseControl * pCancelBtn = IBaseControl::CreateButtonControl(buttonDef);
		m_pRedefineControlsScreen->VAddChildControl(shared_ptr<IBaseControl>(pCancelBtn));
		UIEventCallBackFn cbCancelBtn;
		cbCancelBtn = bind(&cStateRedefineControlsScreen::CancelButtonPressed, this, _1);
		pCancelBtn->VRegisterCallBack(UIET_BTNRELEASED, cbCancelBtn);
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
		OKButtonPressed(params);
		return true;
	}
	return false;
}

// *****************************************************************************
void cStateRedefineControlsScreen::OKButtonPressed(const stUIEventCallbackParam & params)
{
	if(m_pOwner != NULL && m_pOwner->m_pStateMachine != NULL)
	{
		m_pOwner->m_pGameControls->Save();
		m_pOwner->m_pStateMachine->RequestPopState();
	}
}

// *****************************************************************************
void cStateRedefineControlsScreen::CancelButtonPressed(const Graphics::stUIEventCallbackParam& params)
{
	if(m_pOwner != NULL && m_pOwner->m_pStateMachine != NULL)
	{
		m_pOwner->m_pStateMachine->RequestPopState();
	}
}

// *****************************************************************************
void cStateRedefineControlsScreen::ResetButtonPressed(const Graphics::stUIEventCallbackParam& params)
{
	if (m_pOwner != NULL && m_pOwner->m_pGameControls != NULL)
	{
		m_pOwner->m_pGameControls->VSetDefaults();
		SetAllKeyControlsText();
	}
}

// *****************************************************************************
void cStateRedefineControlsScreen::OnKeyChanged(unsigned int uiCharId)
{
	if (m_pOwner != NULL && m_pOwner->m_pGameControls != NULL)
	{
		m_pOwner->m_pGameControls->SetKey(m_icurrentlyEditingID, uiCharId);
		SetAllKeyControlsText();
	}
}

// *****************************************************************************
void cStateRedefineControlsScreen::SetAllKeyControlsText()
{
	cGameControls::KeyMapping keyMap = m_pOwner->m_pGameControls->GetKeyMap();
	cGameControls::KeyMapping::iterator iter;
	for(int i=0; i<m_pKeyControls.size(); i++)
	{
		m_pKeyControls[i]->VSetText(m_pOwner->m_pGameControls->GetKeyName(keyMap[i].m_uiKeyCode));
	}
}
