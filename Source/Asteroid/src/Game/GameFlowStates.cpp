// *****************************************************************************
//  GameFlowStates   version:  1.0   Ankur Sheel  date: 05/24/2008
//  ----------------------------------------------------------------------------
//  
//  ----------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// *****************************************************************************
// 
// *****************************************************************************
#include "stdafx.h"
#include "GameFlowStates.h"
#include "Game.h"
#include "GameFlowStateMachine.h"
#include "Elements/Ship.h"
#include "Elements/Asteroid.h"
#include "Sprite.hxx"
#include "AsteroidView.h"
#include "Timer.hxx"
#include "BaseControl.hxx"
#include "MessageDispatchManager.hxx"
#include "FSM\Telegram.h"
#include "GraphicUtils.hxx"
#include "MainWindow.hxx"
#include "GameOptions.h"
#include "CollisionChecker.hxx"
#include "Elements\Bullet.h"
#include "HighScoreTable.h"
#include "Score.h"
#include "optional.h"

using namespace Graphics;
using namespace Base;
using namespace AI;
using namespace GameBase;
using namespace Utilities;

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
		buttonDef.bAutoSize = true;
		buttonDef.vPosition = cVector2(412, 270);
		buttonDef.strDefaultImage = "buttonDefault.png";
		buttonDef.strPressedImage = "buttonPressed.png";
		buttonDef.labelControlDef.strFont = "licorice";
		buttonDef.labelControlDef.strText = "Single Player";
		buttonDef.labelControlDef.textColor = cColor::BLUE;
		buttonDef.labelControlDef.fTextHeight = 70;

		IBaseControl * pSinglePlayerButton = IBaseControl::CreateButtonControl(buttonDef);
		m_pMenuScreen->VAddChildControl(shared_ptr<IBaseControl>(pSinglePlayerButton));
		UIEventCallBackFn callbackSinglePlayerBtn;
		callbackSinglePlayerBtn = bind(&cStateMenuScreen::SinglePlayerButtonPressed, this, _1);
		pSinglePlayerButton->VRegisterCallBack(UIET_BTNRELEASED, callbackSinglePlayerBtn);

		buttonDef.strControlName = "btnOption";
		buttonDef.bAutoSize = false;
		buttonDef.vSize = pSinglePlayerButton->VGetSize();
		buttonDef.labelControlDef.strText = "Options";
		buttonDef.vPosition = cVector2(412, 370);

		IBaseControl * pOptionsButton = IBaseControl::CreateButtonControl(buttonDef);
		m_pMenuScreen->VAddChildControl(shared_ptr<IBaseControl>(pOptionsButton));
		UIEventCallBackFn callbackOptionsBtn;
		callbackOptionsBtn = bind(&cStateMenuScreen::OptionsButtonPressed, this, _1);
		pOptionsButton->VRegisterCallBack(UIET_BTNRELEASED, callbackOptionsBtn);

		buttonDef.strControlName = "btnHelp";
		buttonDef.labelControlDef.strText = "Help";
		buttonDef.vPosition = cVector2(412, 470);

		IBaseControl * pHelpButton = IBaseControl::CreateButtonControl(buttonDef);
		m_pMenuScreen->VAddChildControl(shared_ptr<IBaseControl>(pHelpButton));
		UIEventCallBackFn callbackHelpBtn;
		callbackHelpBtn = bind(&cStateMenuScreen::HelpButtonPressed, this, _1);
		pHelpButton->VRegisterCallBack(UIET_BTNRELEASED, callbackHelpBtn);
		
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
void cStateMenuScreen::SinglePlayerButtonPressed(const stUIEventCallbackParam & params)
{
	cAsteroidView * pView = dynamic_cast<cAsteroidView *>(m_pOwner->m_pHumanView);
	if(pView)
		pView->OnSinglePlayerSelected(m_pOwner);

	if (m_pOwner && m_pOwner->m_pStateMachine)
	{
		m_pOwner->m_pStateMachine->RequestChangeState(cStatePlayGame::Instance());
	}
}

// *****************************************************************************
void cStateMenuScreen::HelpButtonPressed(const stUIEventCallbackParam & params)
{
	if(m_pOwner != NULL && m_pOwner->m_pStateMachine != NULL)
	{
		m_pOwner->m_pStateMachine->RequestPushState(cStateHelpScreen::Instance());
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

// *****************************************************************************
cStatePlayGame::cStatePlayGame()
{
}
// *****************************************************************************

cStatePlayGame::~cStatePlayGame()
{
}
// *****************************************************************************

cStatePlayGame* cStatePlayGame::Instance()
{
	static cStatePlayGame instance;
	return &instance;
}

// *****************************************************************************
void cStatePlayGame::VOnEnter(cGame *pGame)
{
	IGameFlowStates::VOnEnter(pGame);
	cAsteroidGameElement::SetGame(pGame);

	pGame->m_bGameOver = false;

	cWindowControlDef HUDDef;
	HUDDef.strControlName = "HUDScreen";
	HUDDef.wType = cWindowControlDef::WT_STANDARD;
	HUDDef.vSize = pGame->m_pHumanView->m_pAppWindowControl->VGetSize();
	HUDDef.vPosition = cVector2(0.0f, 0.0f);
	m_pOwner->m_pHUDScreen = shared_ptr<IBaseControl>(IBaseControl::CreateWindowControl(HUDDef));
	pGame->m_pHumanView->m_pAppWindowControl->VAddChildControl(m_pOwner->m_pHUDScreen);

	pGame->m_vScreenTopLeftPos = IGraphicUtils::GetInstance()->ScreenToWorldSpace(cVector2(0,0), pGame->m_pHumanView->GetCamera());
	pGame->m_vScreenBottomRightPos = IGraphicUtils::GetInstance()->ScreenToWorldSpace(cVector2(static_cast<float>(pGame->m_iDisplayWidth), static_cast<float>(pGame->m_iDisplayHeight)),
		pGame->m_pHumanView->GetCamera());
	cVector3 vMiddlePos = (pGame->m_vScreenTopLeftPos + pGame->m_vScreenBottomRightPos)/2.0f;

	cGameElementDef shipDef;
	shipDef.strModelName= "ship";
	shipDef.vPosition= vMiddlePos;
	shipDef.vScale = cVector3(1.0f, 0.5f, 0.5f);
	shared_ptr<cAsteroidGameElement> pShip(DEBUG_NEW cShip());
	pShip->VInitialize(shipDef);
	pGame->m_pGameElements.push_front(pShip);

	cLabelControlDef labelDef;
	labelDef.strControlName = "ScoreLabel";
	labelDef.strFont = "arial"; // forte
	labelDef.textColor = cColor::TURQUOISE;
	labelDef.strText = cString(20, "Score: %02d", 0);
	labelDef.fTextHeight = 40;
	labelDef.vPosition = cVector2(0.0f, 0.0f);
	IBaseControl * pScoreLabel = IBaseControl::CreateLabelControl(labelDef);
	m_pOwner->m_pHUDScreen->VAddChildControl(shared_ptr<IBaseControl>(pScoreLabel));

	labelDef.strControlName = "LivesLabel";
	labelDef.strText = cString(20, "Lives: %02d", pShip->CastToShip()->GetLives());
	labelDef.fTextHeight = 40;
	labelDef.vPosition = cVector2(static_cast<float>(m_pOwner->m_iDisplayWidth), 0.0f);
	IBaseControl * pLivesLabel = IBaseControl::CreateLabelControl(labelDef);
	m_pOwner->m_pHUDScreen->VAddChildControl(shared_ptr<IBaseControl>(pLivesLabel));
	m_pOwner->NextLevel();
}
// *****************************************************************************

void cStatePlayGame::VOnUpdate()
{
	m_pOwner->m_pHumanView->VOnUpdate(m_pOwner->m_pGameTimer->VGetRunningTicks(), m_pOwner->m_pGameTimer->VGetDeltaTime());

	if (m_pOwner->m_bGameOver)
	{
		return;
	}

	if(m_pOwner != NULL && m_pOwner->m_pGameTimer != NULL)
	{
		cGame::GameElementList::iterator iter = m_pOwner->m_pGameElements.begin();
		while(iter != m_pOwner->m_pGameElements.end())
		{
			cAsteroidGameElement *pAsteriodElement = (*iter).get();
			pAsteriodElement->OnUpdate(m_pOwner->m_pGameTimer->VGetDeltaTime());
			
			if (pAsteriodElement->GetRemove())
			{
				iter = m_pOwner->m_pGameElements.erase(iter);
			}
			else
			{
				if (pAsteriodElement->IsActive())
				{
					CheckForCollisions(pAsteriodElement);
				}
				iter++;
			}
		}
	}
	if (m_pOwner->m_bGameOver)
	{
		cShip * pShip = (m_pOwner->m_pGameElements.front())->CastToShip();
		if(pShip != NULL)
		{
			cLabelControlDef labelDef;
			labelDef.strControlName = "GameOverLabel";
			labelDef.strFont = "JokerMan"; // forte
			labelDef.textColor = cColor::TURQUOISE;
			labelDef.fTextHeight = 200;
			labelDef.vPosition = cVector2(m_pOwner->m_iDisplayWidth/4.0f, m_pOwner->m_iDisplayHeight/2.0f - 100.0f);

			int iScore = pShip->GetScore();
			tOptional<int> iPos = m_pOwner->m_pHighScoreTable->IsHighScore(iScore);
			if(iPos.IsValid())
			{
				labelDef.strText = "High Score";
				IMessageDispatchManager::GetInstance()->VDispatchMessage(5.0f, m_pOwner->VGetID(),
					m_pOwner->VGetID(), MSG_HIGH_SCORE, NULL);
			}
			else
			{
				labelDef.strText = "Game Over";
				IMessageDispatchManager::GetInstance()->VDispatchMessage(5.0f, m_pOwner->VGetID(),
					m_pOwner->VGetID(), MSG_GAME_OVER, NULL);
			}
			IBaseControl * pGameOverLabel = IBaseControl::CreateLabelControl(labelDef);
			m_pOwner->m_pHUDScreen->VAddChildControl(shared_ptr<IBaseControl>(pGameOverLabel));
		}


	}
}
// *****************************************************************************

void cStatePlayGame::VOnExit()
{
	if (m_pOwner->m_pHumanView->m_pAppWindowControl != NULL)
	{
		m_pOwner->m_pHumanView->m_pAppWindowControl->VRemoveChildControl("HUDScreen");
	}
	m_pOwner->OnRestart();
}
// *****************************************************************************

bool cStatePlayGame::VOnMessage(const Telegram &msg)
{
	if(msg.Msg == MSG_GAME_OVER)
	{
		m_pOwner->m_pStateMachine->RequestChangeState(cStateTitleScreen::Instance());
		return true;
	}
	else if(msg.Msg == MSG_NEXT_LEVEL)
	{
		m_pOwner->NextLevel();
		return true;
	}
	else if(msg.Msg == MSG_ESCAPE_PRESSED)
	{
		if(m_pOwner != NULL && m_pOwner->m_pStateMachine != NULL)
		{
			m_pOwner->m_pStateMachine->RequestPushState(cStatePauseScreen::Instance());
		}
		return true;
	}
	else if(msg.Msg == MSG_HIGH_SCORE)
	{
		return true;
	}
	return false;
}

// *****************************************************************************
void cStatePlayGame::CheckForCollisions(cAsteroidGameElement * const pAsteriodElement)
{
	IGame::GameElementList::iterator iter;
	cAsteroid * pAsteroid = NULL;
	cContact contact;
	for (iter = m_pOwner->m_pGameElements.begin(); iter != m_pOwner->m_pGameElements.end(); iter++)
	{
		pAsteroid = (*iter)->CastToAsteroid();
		if(pAsteroid && pAsteroid->IsActive())
		{
			if ((ICollisionChecker::GetInstance()->VCheckForCollisions(pAsteriodElement->GetAABB(),
				pAsteroid->GetAABB(), contact)))
			{
				cBullet * pBullet = pAsteriodElement->CastToBullet();
				if (pBullet != NULL)
				{
					m_pOwner->AsteroidHitByBullet(pBullet, pAsteroid);
				}
				else
				{
					cShip * pShip = pAsteriodElement->CastToShip();
					if (pShip != NULL)
					{
						m_pOwner->ShipHitByAsteroid();
					}
				}
				break;
			}
		}
	}
}

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
		def.strText = "Press W to Move forward\nPress S to Move Backward";
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

void cStateHelpScreen::BackButtonPressed(const stUIEventCallbackParam & params)
{
	if(m_pOwner != NULL && m_pOwner->m_pStateMachine != NULL)
	{
		m_pOwner->m_pStateMachine->RequestPopState();
	}
}

// *****************************************************************************
cStateOptionsScreen::cStateOptionsScreen()
: m_pOptionsScreen(NULL)
{
}

// *****************************************************************************
cStateOptionsScreen::~cStateOptionsScreen()
{
}

// *****************************************************************************
cStateOptionsScreen* cStateOptionsScreen::Instance()
{
	static cStateOptionsScreen instance;
	return &instance;
}

// *****************************************************************************
void cStateOptionsScreen::VOnEnter(cGame *pGame)
{
	IGameFlowStates::VOnEnter(pGame);
	if (pGame->m_pHumanView->m_pAppWindowControl != NULL)
	{
		cWindowControlDef optionsDef;
		optionsDef.strControlName = "OptionsScreen";
		optionsDef.wType = cWindowControlDef::WT_STANDARD;
		optionsDef.vPosition = cVector2(0, 0);
		optionsDef.vSize = pGame->m_pHumanView->m_pAppWindowControl->VGetSize();
		optionsDef.strBGImageFile = "WindowBG.png";
		m_pOptionsScreen = IBaseControl::CreateWindowControl(optionsDef);
		pGame->m_pHumanView->m_pAppWindowControl->VAddChildControl(shared_ptr<IBaseControl>(m_pOptionsScreen));
		pGame->m_pHumanView->m_pAppWindowControl->VMoveToFront(m_pOptionsScreen);

		cLabelControlDef def;
		def.strControlName = "LabelOptions";
		def.strFont = "JokerMan"; 
		def.textColor = cColor::RED;
		def.strText = "Options";
		def.fTextHeight = 200;
		def.vPosition = cVector2(357.0f, 0.0f);
		IBaseControl * pTitleLabelControl = IBaseControl::CreateLabelControl(def);
		m_pOptionsScreen->VAddChildControl(shared_ptr<IBaseControl>(pTitleLabelControl));
		
		cCheckBoxControlDef checkboxControlDef;
		checkboxControlDef.strControlName = "cbMusic";
		checkboxControlDef.bChecked = cGameOptions::GameOptions().bPlayMusic;
		checkboxControlDef.buttonControlDef.strDefaultImage = "Unchecked.png";
		checkboxControlDef.buttonControlDef.strPressedImage = "Checked.png";
		checkboxControlDef.labelControlDef.strText = "Music";
		checkboxControlDef.labelControlDef.strFont = "licorice";
		checkboxControlDef.labelControlDef.fTextHeight = 20;
		checkboxControlDef.labelControlDef.textColor = cColor::WHITE;
		checkboxControlDef.buttonControlDef.vSize = cVector2(50, 30);
		checkboxControlDef.iSpaceCaption = 10;
		checkboxControlDef.vPosition = cVector2(0.f, 250.f);

		IBaseControl * pMusicCheckBoxControl = IBaseControl::CreateCheckBoxControl(checkboxControlDef);
		m_pOptionsScreen->VAddChildControl(shared_ptr<IBaseControl>(pMusicCheckBoxControl));
		UIEventCallBackFn musicCheckBoxCallback;
		musicCheckBoxCallback = bind(&cHumanView::MusicCheckBoxPressed, m_pOwner->m_pHumanView, _1);
		pMusicCheckBoxControl->VRegisterCallBack(UIET_BTNPRESSED, musicCheckBoxCallback);

		checkboxControlDef.strControlName = "cbSFX";
		checkboxControlDef.labelControlDef.strText = "SFX";
		checkboxControlDef.vPosition = cVector2(0.f, 300.f);
		checkboxControlDef.bChecked = cGameOptions::GameOptions().bPlaySfx;

		IBaseControl * pSfxCheckBoxControl = IBaseControl::CreateCheckBoxControl(checkboxControlDef);
		m_pOptionsScreen->VAddChildControl(shared_ptr<IBaseControl>(pSfxCheckBoxControl ));
		UIEventCallBackFn sfxCheckBoxCallback;
		sfxCheckBoxCallback = bind(&cHumanView::SfxCheckBoxPressed, m_pOwner->m_pHumanView, _1);
		pSfxCheckBoxControl->VRegisterCallBack(UIET_BTNPRESSED, sfxCheckBoxCallback);

		checkboxControlDef.strControlName = "cbFullscreen";
		checkboxControlDef.labelControlDef.strText = "FullScreen (will be applied";
		checkboxControlDef.labelControlDef.strText += " after restarting the game)";
		checkboxControlDef.vPosition = cVector2(0.f, 350.f);
		checkboxControlDef.bChecked = cGameOptions::GameOptions().bFullScreen;
		
		IBaseControl * pFullscreenCheckBoxControl = IBaseControl::CreateCheckBoxControl(checkboxControlDef);
		m_pOptionsScreen->VAddChildControl(shared_ptr<IBaseControl>(pFullscreenCheckBoxControl));
		UIEventCallBackFn fullScreenCheckBoxCallback;
		fullScreenCheckBoxCallback = bind(&cHumanView::FullScreenCheckBoxPressed, m_pOwner->m_pHumanView, _1);
		pFullscreenCheckBoxControl->VRegisterCallBack(UIET_BTNPRESSED, fullScreenCheckBoxCallback);

		def.strControlName = "MusicVolume";
		def.strFont = "licorice"; 
		def.textColor = cColor::WHITE;
		def.strText = "Music Volume";
		def.fTextHeight = 20;
		def.vPosition = cVector2(0.0f, 406.0f);
		IBaseControl * pMusicVolumeLabelControl = IBaseControl::CreateLabelControl(def);
		m_pOptionsScreen->VAddChildControl(shared_ptr<IBaseControl>(pMusicVolumeLabelControl));

		cScrollBarControlDef hScrollBarDef;
		hScrollBarDef.strControlName = "hsbMusicVolume";
		hScrollBarDef.strBGImage = "ScrollBar_BG.png";
		hScrollBarDef.iMinPos = 0;
		hScrollBarDef.iMaxPos = 20;
		hScrollBarDef.iInitialThumbPosition = (cGameOptions::GameOptions().iMusicVolume) / 5;
		hScrollBarDef.thumbBtnDef.strDefaultImage = "ScrollBar_Thumb.png";
		hScrollBarDef.thumbBtnDef.strPressedImage = "ScrollBar_Thumb.png";
		hScrollBarDef.TopLeftArrowDef.strDefaultImage = "ScrollBar_Left.png";
		hScrollBarDef.TopLeftArrowDef.strPressedImage = "ScrollBar_Left.png";
		hScrollBarDef.BottomRightArrowDef.strDefaultImage = "ScrollBar_Right.png";
		hScrollBarDef.BottomRightArrowDef.strPressedImage = "ScrollBar_Right.png";
		hScrollBarDef.vPosition = cVector2(120.f, 400.f);
		hScrollBarDef.vSize = cVector2(200, 30);

		IBaseControl * pMusicScrollBarControl = IBaseControl::CreateHScrollBarControl(hScrollBarDef);
		m_pOptionsScreen->VAddChildControl(shared_ptr<IBaseControl>(pMusicScrollBarControl));

		UIEventCallBackFn musicScrollbarcallback;
		musicScrollbarcallback = bind(&cStateOptionsScreen::MusicScrollbarChanged, this, _1);
		pMusicScrollBarControl->VRegisterCallBack(UIET_SCBCHANGED, musicScrollbarcallback);

		def.strControlName = "tbMusicVolume";
		def.strBGImageFile = "TextBox.png";
		def.textColor = cColor::BLACK;
		def.strText = cString(30, "%d", cGameOptions::GameOptions().iMusicVolume);
		def.vPosition = cVector2(340.0f, 406.0f);
		IBaseControl * pMusicVolumeTextControl = IBaseControl::CreateLabelControl(def);
		m_pOptionsScreen->VAddChildControl(shared_ptr<IBaseControl>(pMusicVolumeTextControl));

		def.strControlName = "SFXVolume";
		def.strBGImageFile = "";
		def.textColor = cColor::WHITE;
		def.strText = "SFX Volume";
		def.vPosition = cVector2(0.0f, 456.0f);
		IBaseControl * pSFXVolumeLabelControl = IBaseControl::CreateLabelControl(def);
		m_pOptionsScreen->VAddChildControl(shared_ptr<IBaseControl>(pSFXVolumeLabelControl));

		hScrollBarDef.strControlName = "hsbSFXVolume";
		hScrollBarDef.iInitialThumbPosition = (cGameOptions::GameOptions().iSFXVolume) / 5;
		hScrollBarDef.vPosition = cVector2(120.f, 450.f);

		IBaseControl * pSFXScrollBarControl = IBaseControl::CreateHScrollBarControl(hScrollBarDef);
		m_pOptionsScreen->VAddChildControl(shared_ptr<IBaseControl>(pSFXScrollBarControl));

		UIEventCallBackFn SFXScrollbarcallback;
		SFXScrollbarcallback = bind(&cStateOptionsScreen::SFXScrollbarChanged, this, _1);
		pSFXScrollBarControl->VRegisterCallBack(UIET_SCBCHANGED, SFXScrollbarcallback);

		def.strControlName = "tbSFXVolume";
		def.strBGImageFile = "TextBox.png";
		def.textColor = cColor::BLACK;
		def.strText = cString(30, "%d", cGameOptions::GameOptions().iSFXVolume);
		def.vPosition = cVector2(340.0f, 456.0f);
		IBaseControl * pSFXVolumeTextControl = IBaseControl::CreateLabelControl(def);
		m_pOptionsScreen->VAddChildControl(shared_ptr<IBaseControl>(pSFXVolumeTextControl));

		cButtonControlDef buttonDef;
		buttonDef.bAutoSize = true;
		buttonDef.vPosition = cVector2(0, 550);
		buttonDef.strDefaultImage = "buttonDefault.png";
		buttonDef.strPressedImage = "buttonPressed.png";
		buttonDef.labelControlDef.strFont = "licorice";
		buttonDef.labelControlDef.strText = "Back";
		buttonDef.labelControlDef.textColor = cColor::BLUE;
		buttonDef.labelControlDef.fTextHeight = 50;

		IBaseControl * pBackButton = IBaseControl::CreateButtonControl(buttonDef);
		m_pOptionsScreen->VAddChildControl(shared_ptr<IBaseControl>(pBackButton));
		UIEventCallBackFn callBackBtn;
		callBackBtn = bind(&cStateOptionsScreen::BackButtonPressed, this, _1);
		pBackButton->VRegisterCallBack(UIET_BTNRELEASED, callBackBtn);
	}
}

// *****************************************************************************
void cStateOptionsScreen::VOnUpdate()
{

}

// *****************************************************************************
void cStateOptionsScreen::VOnExit()
{
	if (m_pOwner->m_pHumanView->m_pAppWindowControl != NULL)
	{
		m_pOwner->m_pHumanView->m_pAppWindowControl->VRemoveChildControl("OptionsScreen");
	}
}

// *****************************************************************************
bool cStateOptionsScreen::VOnMessage(const Telegram &msg)
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
void cStateOptionsScreen::BackButtonPressed(const stUIEventCallbackParam & params)
{
	if(m_pOwner != NULL)
	{
		if(m_pOwner->m_pStateMachine != NULL)
		{
			m_pOwner->m_pStateMachine->RequestPopState();
		}
		cGameOptions::SaveGameOptions(cGameDirectories::GameDirectories().strMediaDirectory + "PlayerOptions.xml");
	}
}

// *****************************************************************************
void cStateOptionsScreen::MusicScrollbarChanged(const stUIEventCallbackParam & params)
{
	IBaseControl * pMusicTextBox = m_pOptionsScreen->VFindChildControl("tbMusicVolume");
	cGameOptions::GameOptions().iMusicVolume = params.iThumbPos * 5;
	pMusicTextBox->VSetText(cString(20, "%d", cGameOptions::GameOptions().iMusicVolume));
	m_pOwner->m_pHumanView->SetMusicVolume();
}

// *****************************************************************************
void cStateOptionsScreen::SFXScrollbarChanged(const Graphics::stUIEventCallbackParam& params)
{
	IBaseControl * pMusicTextBox = m_pOptionsScreen->VFindChildControl("tbSFXVolume");
	cGameOptions::GameOptions().iSFXVolume = params.iThumbPos * 5;
	pMusicTextBox->VSetText(cString(20, "%d", cGameOptions::GameOptions().iSFXVolume));
	m_pOwner->m_pHumanView->SetSFXVolume();
}

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
		int currentPosY = 220.0f;
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

void cStateHighScoreScreen::BackButtonPressed(const stUIEventCallbackParam & params)
{
	if(m_pOwner != NULL && m_pOwner->m_pStateMachine != NULL)
	{
		m_pOwner->m_pStateMachine->RequestPopState();
	}
}
