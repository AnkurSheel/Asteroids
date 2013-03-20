// *****************************************************************************
//  EnterNameScreen   version:  1.0   Ankur Sheel  date: 2013/03/20
// *****************************************************************************
//  purpose:	
// *****************************************************************************
#include "stdafx.h"
#include "EnterNameScreen.h"
#include "Game\Game.h"
#include "HumanView.h"
#include "ControlStructures.h"
#include "BaseControl.hxx"
#include "Color.h"
#include "MessageDispatchManager.hxx"
#include "FSM\Telegram.h"
#include "Game\GameFlowStateMachine.h"
#include "HighScoreTable.h"
#include "Score.h"
#include "TitleScreen.h"

using namespace Base;
using namespace Graphics;
using namespace GameBase;
using namespace AI;

// *****************************************************************************
cStateEnterNameScreen::cStateEnterNameScreen()
	: m_iNewScorePos(11)
	, m_iNewScore(0)
{
}
// *****************************************************************************
cStateEnterNameScreen::~cStateEnterNameScreen()
{

}
// *****************************************************************************
cStateEnterNameScreen * cStateEnterNameScreen::Instance()
{
	static cStateEnterNameScreen instance;
	return &instance;
}

// *****************************************************************************
void cStateEnterNameScreen::VOnEnter(cGame * pGame)
{
	IGameFlowStates::VOnEnter(pGame);
	if (pGame->m_pHumanView->m_pAppWindowControl != NULL)
	{
		cWindowControlDef enterNameDef;
		enterNameDef.strControlName = "EnterNameScreen";
		enterNameDef.wType = cWindowControlDef::WT_STANDARD;
		enterNameDef.vPosition = cVector2(0, 0);
		enterNameDef.strBGImageFile = "WindowBG.png";
		enterNameDef.vSize = pGame->m_pHumanView->m_pAppWindowControl->VGetSize();
		IBaseControl * pEnterNameScreen = IBaseControl::CreateWindowControl(enterNameDef);
		pGame->m_pHumanView->m_pAppWindowControl->VAddChildControl(shared_ptr<IBaseControl>(pEnterNameScreen));
		pGame->m_pHumanView->m_pAppWindowControl->VMoveToFront(pEnterNameScreen);

		cLabelControlDef def;
		def.strControlName = "LabelHighScore";
		def.strFont = "JokerMan"; 
		def.textColor = cColor::RED;
		def.strText = "High Scores";
		def.fTextHeight = 200;
		def.vPosition = cVector2(257.0f, 0.0f);
		IBaseControl * pTitleLabelControl = IBaseControl::CreateLabelControl(def);
		pEnterNameScreen->VAddChildControl(shared_ptr<IBaseControl>(pTitleLabelControl));

		cHighScoreTable::ScoreSet highScores = pGame->m_pHighScoreTable->GetScores();
		cHighScoreTable::ScoreSet::iterator iter;
		cHighScoreTable::ScoreSet::iterator lastiter = highScores.end();
		lastiter--;

		cTextBoxControlDef tbDef;
		tbDef.strControlName = "TBNewHighScoreName";
		tbDef.strFont = "JokerMan"; 
		tbDef.textColor = cColor::BLACK;
		tbDef.strBGImage = "TextBox.png";
		tbDef.strCaretImage = "Caret.png";
		tbDef.iCaretWidth = 5;
		tbDef.fCaretUpdateTime = 0.25f;
		tbDef.fTextHeight = 30;
		tbDef.vSize = cVector2(200.0f, 30.0f);

		def.strFont = "licorice"; 
		def.textColor = cColor::BLUE;
		def.fTextHeight = 30;
		def.vSize = cVector2(200.0f, 30.0f);

		int i = 0;
		int currentPosY = 220.0f;

		for(iter = highScores.begin(); iter != highScores.end(); iter++)
		{
			if (i == m_iNewScorePos)
			{
				tbDef.vPosition = cVector2(0.0f, currentPosY);
				IBaseControl * pNameControl = IBaseControl::CreateTextBoxControl(tbDef);
				pEnterNameScreen->VAddChildControl(shared_ptr<IBaseControl>(pNameControl));
				UIEventCallBackFn callBackTextBox;
				callBackTextBox = bind(&cStateEnterNameScreen::NameEntered, this, _1);
				pNameControl->VRegisterCallBack(UIET_TBCHANGED, callBackTextBox);

				def.strControlName = cString(100, "Score%d", i);
				def.strText = cString(50, "%d",m_iNewScore);
				def.vPosition = cVector2(250.0f, currentPosY);
				IBaseControl * pScoreControl = IBaseControl::CreateLabelControl(def);
				pEnterNameScreen->VAddChildControl(shared_ptr<IBaseControl>(pScoreControl));

				currentPosY += 40;
			}
			if (iter == lastiter)
			{
				continue;
			}

			shared_ptr<cScore> pScore = (*iter);

			def.strControlName = cString(100, "Name%d", i);
			def.strText = pScore->GetPlayerName();
			def.vPosition = cVector2(0.0f, currentPosY);
			IBaseControl * pNameControl = IBaseControl::CreateLabelControl(def);
			pEnterNameScreen->VAddChildControl(shared_ptr<IBaseControl>(pNameControl));

			def.strControlName = cString(100, "Score%d", i);
			def.strText = cString(50, "%d",pScore->GetScore());
			def.vPosition = cVector2(250.0f, currentPosY);
			IBaseControl * pScoreControl = IBaseControl::CreateLabelControl(def);
			pEnterNameScreen->VAddChildControl(shared_ptr<IBaseControl>(pScoreControl));

			i++;
			currentPosY += 40;
		}
		iter--;
		highScores.erase(iter);
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
		pEnterNameScreen->VAddChildControl(shared_ptr<IBaseControl>(pBackButton));
		UIEventCallBackFn callBackBtn;
		callBackBtn = bind(&cStateEnterNameScreen::BackButtonPressed, this, _1);
		pBackButton->VRegisterCallBack(UIET_BTNRELEASED, callBackBtn);
	}
}
// *****************************************************************************
void cStateEnterNameScreen::VOnUpdate()
{
}

// *****************************************************************************
void cStateEnterNameScreen::VOnExit()
{
	if (m_pOwner->m_pHumanView->m_pAppWindowControl != NULL)
	{
		m_pOwner->m_pHumanView->m_pAppWindowControl->VRemoveChildControl("EnterNameScreen");
	}
}
// *****************************************************************************
bool cStateEnterNameScreen::VOnMessage( const AI::Telegram & msg )
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
void cStateEnterNameScreen::BackButtonPressed(const Graphics::stUIEventCallbackParam& params)
{
	shared_ptr<cScore> pScore(DEBUG_NEW cScore());
	pScore->SetPlayerName(m_strPlayerName);
	pScore->SetScore(m_iNewScore);
	m_pOwner->m_pHighScoreTable->VAddNewScore(pScore);
	m_pOwner->m_pHighScoreTable->VSave();

	if(m_pOwner != NULL && m_pOwner->m_pStateMachine != NULL)
	{
		m_pOwner->m_pStateMachine->RequestChangeState(cStateTitleScreen::Instance());
	}
}

// *****************************************************************************
void cStateEnterNameScreen::SetNewScorePos(int val)
{
	m_iNewScorePos = val;
}

// *****************************************************************************
void cStateEnterNameScreen::SetNewScore(int val)
{
	m_iNewScore = val;
}

// *****************************************************************************
void cStateEnterNameScreen::NameEntered(const Graphics::stUIEventCallbackParam& params)
{
	m_strPlayerName = params.strText;
}
