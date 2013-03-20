// *****************************************************************************
//  HighScoreScreen   version:  1.0   Ankur Sheel  date: 2013/03/20
// *****************************************************************************
//  purpose:	
// *****************************************************************************
#include "stdafx.h"
#include "HighScoreScreen.h"
#include "Game\Game.h"
#include "HumanView.h"
#include "ControlStructures.h"
#include "BaseControl.hxx"
#include "FSM\Telegram.h"
#include "Game\GameFlowStateMachine.h"
#include "HighScoreTable.h"
#include "Score.h"

using namespace Base;
using namespace Graphics;
using namespace GameBase;
using namespace AI;

// *****************************************************************************
cStateHighScoreScreen::cStateHighScoreScreen()
{
}

// *****************************************************************************
cStateHighScoreScreen::~cStateHighScoreScreen()
{
}

// *****************************************************************************
cStateHighScoreScreen* cStateHighScoreScreen::Instance()
{
	static cStateHighScoreScreen instance;
	return &instance;
}

// *****************************************************************************
void cStateHighScoreScreen::VOnEnter(cGame *pGame)
{
	IGameFlowStates::VOnEnter(pGame);
	if (pGame->m_pHumanView->m_pAppWindowControl != NULL)
	{
		cWindowControlDef highScoreDef;
		highScoreDef.strControlName = "HighScoreScreen";
		highScoreDef.wType = cWindowControlDef::WT_STANDARD;
		highScoreDef.vPosition = cVector2(0, 0);
		highScoreDef.strBGImageFile = "WindowBG.png";
		highScoreDef.vSize = pGame->m_pHumanView->m_pAppWindowControl->VGetSize();
		IBaseControl * pHighScoreScreen = IBaseControl::CreateWindowControl(highScoreDef);
		pGame->m_pHumanView->m_pAppWindowControl->VAddChildControl(shared_ptr<IBaseControl>(pHighScoreScreen));
		pGame->m_pHumanView->m_pAppWindowControl->VMoveToFront(pHighScoreScreen);

		cLabelControlDef def;
		def.strControlName = "LabelHighScore";
		def.strFont = "JokerMan"; 
		def.textColor = cColor::RED;
		def.strText = "High Scores";
		def.fTextHeight = 200;
		def.vPosition = cVector2(257.0f, 0.0f);
		IBaseControl * pTitleLabelControl = IBaseControl::CreateLabelControl(def);
		pHighScoreScreen->VAddChildControl(shared_ptr<IBaseControl>(pTitleLabelControl));

		cHighScoreTable::ScoreSet highScores = pGame->m_pHighScoreTable->GetScores();
		cHighScoreTable::ScoreSet::iterator iter;
		int i = 0;
		int currentPosY = 220;
		for(iter = highScores.begin(); iter != highScores.end(); iter++)
		{
			shared_ptr<cScore> pScore = (*iter);

			def.strControlName = cString(100, "Name%d", i);
			def.strFont = "licorice"; 
			def.textColor = cColor::BLUE;
			def.strText = pScore->GetPlayerName();
			def.fTextHeight = 30;
			def.vPosition = cVector2(0.0f, currentPosY);
			def.vSize = cVector2(200.0f, 30.0f);
			IBaseControl * pNameControl = IBaseControl::CreateLabelControl(def);
			pHighScoreScreen->VAddChildControl(shared_ptr<IBaseControl>(pNameControl));

			def.strControlName = cString(100, "Score%d", i);
			def.strText = cString(50, "%d",pScore->GetScore());
			def.vPosition = cVector2(250.0f, currentPosY);
			IBaseControl * pScoreControl = IBaseControl::CreateLabelControl(def);
			pHighScoreScreen->VAddChildControl(shared_ptr<IBaseControl>(pScoreControl));

			i++;
			currentPosY += 40;
		}

		cButtonControlDef buttonDef;
		buttonDef.strControlName = "btnBack";
		buttonDef.bAutoSize = true;
		buttonDef.vPosition = cVector2(0, 650);
		buttonDef.strDefaultImage = "buttonDefault.png";
		buttonDef.strPressedImage = "buttonPressed.png";
		buttonDef.labelControlDef.strFont = "licorice";
		buttonDef.labelControlDef.strText = "Back";
		buttonDef.labelControlDef.textColor = cColor::BLUE;
		buttonDef.labelControlDef.fTextHeight = 50;

		IBaseControl * pBackButton = IBaseControl::CreateButtonControl(buttonDef);
		pHighScoreScreen->VAddChildControl(shared_ptr<IBaseControl>(pBackButton));
		UIEventCallBackFn callBackBtn;
		callBackBtn = bind(&cStateHighScoreScreen::BackButtonPressed, this, _1);
		pBackButton->VRegisterCallBack(UIET_BTNRELEASED, callBackBtn);
	}
}

// *****************************************************************************
void cStateHighScoreScreen::VOnUpdate()
{

}

// *****************************************************************************
void cStateHighScoreScreen::VOnExit()
{
	if (m_pOwner->m_pHumanView->m_pAppWindowControl != NULL)
	{
		m_pOwner->m_pHumanView->m_pAppWindowControl->VRemoveChildControl("HighScoreScreen");
	}
}

// *****************************************************************************
bool cStateHighScoreScreen::VOnMessage(const Telegram &msg)
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
void cStateHighScoreScreen::BackButtonPressed(const stUIEventCallbackParam & params)
{
	if(m_pOwner != NULL && m_pOwner->m_pStateMachine != NULL)
	{
		m_pOwner->m_pStateMachine->RequestPopState();
	}
}
