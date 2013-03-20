// *****************************************************************************
//  StatePlayGame   version:  1.0   Ankur Sheel  date: 2013/03/20
// *****************************************************************************
//  purpose:	
// *****************************************************************************
#include "stdafx.h"
#include "StatePlayGame.h"
#include "Game\Game.h"
#include "HumanView.h"
#include "ControlStructures.h"
#include "BaseControl.hxx"
#include "MessageDispatchManager.hxx"
#include "FSM\Telegram.h"
#include "Game\GameFlowStateMachine.h"
#include "Game\Elements\AsteroidGameElement.h"
#include "GraphicUtils.hxx"
#include "Game\Elements\Ship.h"
#include "Timer.hxx"
#include "Optional.h"
#include "HighScoreTable.h"
#include "TitleScreen.h"
#include "PauseScreen.h"
#include "EnterNameScreen.h"
#include "CollisionChecker.hxx"
#include "Game\Elements\Asteroid.h"
#include "Game\Elements\Bullet.h"

using namespace Base;
using namespace Graphics;
using namespace GameBase;
using namespace AI;

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
				shared_ptr<int> pPos(DEBUG_NEW int(*iPos));
				labelDef.strText = "High Score";
				IMessageDispatchManager::GetInstance()->VDispatchMessage(5.0f, m_pOwner->VGetID(),
					m_pOwner->VGetID(), MSG_NEW_HIGH_SCORE, pPos);
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
	else if(msg.Msg == MSG_NEW_HIGH_SCORE)
	{
		cStateEnterNameScreen::Instance()->SetNewScorePos(DereferenceToType<int>(msg.pExtraInfo));
		cShip * pShip = (m_pOwner->m_pGameElements.front())->CastToShip();
		if (pShip != NULL)
		{
			cStateEnterNameScreen::Instance()->SetNewScore(pShip->GetScore());
		}
		m_pOwner->m_pStateMachine->RequestChangeState(cStateEnterNameScreen::Instance());
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
