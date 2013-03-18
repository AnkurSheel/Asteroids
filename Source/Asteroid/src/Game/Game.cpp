// *****************************************************************************
//  Game   version:  1.0   Ankur Sheel  date: 04/30/2008
//  ----------------------------------------------------------------------------
//  
//  ----------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// *****************************************************************************
// 
// *****************************************************************************
#include "stdafx.h"
#include "Game.h"
#include "Sprite.hxx"
#include "Elements/Ship.h"
#include "Elements/Asteroid.h"
#include "GameFlowStateMachine.h"
#include "GameFlowStates.h"
#include "AsteroidView.h"
#include "Timer.hxx"
#include "ProcessManager.hxx"
#include "MainWindow.hxx"
#include "ResourceManager.hxx"
#include "EntityManager.hxx"
#include "MessageDispatchManager.hxx"
#include "BaseControl.hxx"
#include "RandomGenerator.hxx"
#include "HighScoreTable.h"

using namespace MySound;
using namespace Graphics;
using namespace Base;
using namespace GameBase;
using namespace Utilities;

// *****************************************************************************
cGame::cGame(const cString strName)
: cBaseApp(strName)
, m_pStateMachine(NULL)
, m_pRandomGenerator(NULL)
, m_bGameOver(false)
, m_iCurrentLevel(0)
, m_iNumberOfAsteroids(0)
, m_pHighScoreTable(NULL)
{
}

// *****************************************************************************
cGame::~cGame()
{
	VCleanup();
}

// *****************************************************************************
void cGame::VOnInitialization(const HINSTANCE & hInstance, const int nCmdShow,
							  const cString & strOptionsFile)
{
	cBaseApp::VOnInitialization(hInstance, nCmdShow, strOptionsFile);

	m_iDisplayHeight = static_cast<int>(m_pHumanView->m_pAppWindowControl->VGetHeight());
	m_iDisplayWidth = static_cast<int>(m_pHumanView->m_pAppWindowControl->VGetWidth());

	m_pStateMachine = DEBUG_NEW cGameFlowStateMachine(this);

	IEntityManager::GetInstance()->VRegisterEntity(this);
	m_pStateMachine->SetCurrentState(cStateTitleScreen::Instance());

	m_pRandomGenerator = IRandomGenerator::CreateRandomGenerator();
	if (m_pRandomGenerator)
	{
		Log_Write_L1(ILogger::LT_DEBUG, cString(100, "Random Generator created with seed %u", m_pRandomGenerator->GetRandomSeed()));
	}
	
	m_pHighScoreTable = DEBUG_NEW cHighScoreTable("Scores", 10);
	m_pHighScoreTable->Initialize();
	OnRestart();
}

void cGame::VCreateHumanView()
{
	m_pHumanView = DEBUG_NEW cAsteroidView();
}

// *****************************************************************************
cString cGame::VGetGameTitle() const
{
	return VGetName();
}

// *****************************************************************************
void cGame::VOnUpdate()
{
	cBaseApp::VOnUpdate();
	m_pStateMachine->Update();
	IMessageDispatchManager::GetInstance()->VOnUpdate();
}

// *****************************************************************************
void cGame::VCleanup()
{
	m_pGameElements.clear();

	SafeDelete(&m_pRandomGenerator);
	SafeDelete(&m_pStateMachine);
	m_pHUDScreen.reset();
	SafeDelete(&m_pHighScoreTable);
	cBaseApp::VCleanup();
}

void cGame::MoveShip(const ShipActions eShipActions)
{
	cShip * pShip = (m_pGameElements.front())->CastToShip();
	if(pShip)
	{
		switch(eShipActions)
		{
			case SA_MOVE_FWD:
			{
				pShip->MoveForward(m_pGameTimer->VGetDeltaTime());
				break;
			}
			case SA_MOVE_BK:
			{
				pShip->MoveBack(m_pGameTimer->VGetDeltaTime());
				break;
			}
			case SA_ROTATE_LEFT:
			{
				pShip->RotateLeft(m_pGameTimer->VGetDeltaTime());
				break;
			}
			case SA_ROTATE_RIGHT:
			{
				pShip->RotateRight(m_pGameTimer->VGetDeltaTime());
				break;
			}
			case SA_FIRE:
			{
				pShip->Fire();
				break;
			}
			
		}
	}
}

// *****************************************************************************
bool cGame::VOnHandleMessage(const AI::Telegram & telegram)
{
	return m_pStateMachine->HandleMessage(telegram);
}

//// *****************************************************************************
void cGame::VGetGameElements(GameElementList & gameElements) const
{
	gameElements = m_pGameElements; 
}

// *****************************************************************************
cVector3 cGame::VGetScreenTopLeftPos() const
{
	return m_vScreenTopLeftPos;
}

// *****************************************************************************
cVector3 cGame::VGetScreenBottomRightPos() const
{
	return m_vScreenBottomRightPos;
}

// *****************************************************************************
IRandomGenerator * const cGame::GetRandomGenerator() const
{
	return m_pRandomGenerator;
}

// *****************************************************************************
void cGame::AddGameElement(shared_ptr<cAsteroidGameElement> const pGameElement)
{
	m_pGameElements.push_back(pGameElement);
}

// *****************************************************************************
void cGame::AsteroidHitByBullet(cAsteroidGameElement * const pBulletElement,
								cAsteroidGameElement * const pAsteroidElement)
{
	cBullet * pBullet = pBulletElement->CastToBullet();
	m_pGameElements.front()->CastToShip()->BulletDestroyed(pBullet);

	cAsteroid * pAsteroid = pAsteroidElement->CastToAsteroid();
	if (pAsteroid != NULL)
	{
		cShip * pShip = m_pGameElements.front()->CastToShip();
		if (pShip)
		{
			pShip->IncrementScore(pAsteroid->GetCurrentSize() * 10);
			IBaseControl * pScoreLabel = m_pHUDScreen->VFindChildControl("ScoreLabel");
			if (pScoreLabel != NULL)
			{
				pScoreLabel->VSetText(cString(20, "Score: %02d", pShip->GetScore()));
			}
		}
		m_iNumberOfAsteroids--;
		pAsteroid->Hit();
		m_pHumanView->PlaySFX("collision1.wav");
		if (m_iNumberOfAsteroids == 0)
		{
			m_pHumanView->PlaySFX("win.wav");
			cLabelControlDef labelDef;
			labelDef.strControlName = "NewWaveLabel";
			labelDef.strFont = "JokerMan"; // forte
			labelDef.textColor = cColor::TURQUOISE;
			labelDef.strText = "NewWave in 5 seconds";
			labelDef.fTextHeight = 100;
			labelDef.vPosition = cVector2(100.0f, m_iDisplayHeight/2.0f - 50.0f);
			IBaseControl * pGameOverLabel = IBaseControl::CreateLabelControl(labelDef);
			m_pHUDScreen->VAddChildControl(shared_ptr<IBaseControl>(pGameOverLabel));
			IMessageDispatchManager::GetInstance()->VDispatchMessage(5.0f, VGetID(),
				VGetID(), MSG_NEXT_LEVEL, NULL);
		}
	}
}

// *****************************************************************************
void cGame::ShipHitByAsteroid()
{
	cShip * pShip = (m_pGameElements.front())->CastToShip();
	if (pShip != NULL && !pShip->IsInvincible())
	{
		m_pHumanView->PlaySFX("collision2.wav");
		pShip->MakeInactiveFor(1);
		pShip->DecrementLives(1);
		IBaseControl * pScoreLabel = m_pHUDScreen->VFindChildControl("LivesLabel");
		if (pScoreLabel != NULL)
		{
			pScoreLabel->VSetText(cString(20, "Lives: %02d", pShip->GetLives()));
		}
		if (pShip->GetLives() == 0)
		{
			m_bGameOver = true;
		}
	}
}

// *****************************************************************************
void cGame::OnRestart()
{
	m_bGameOver = false;
	m_pHUDScreen.reset();
	m_pGameElements.clear();
	m_iCurrentLevel = 0;
}

// *****************************************************************************
void cGame::NextLevel()
{
	m_iCurrentLevel++;	
	m_pHUDScreen->VRemoveChildControl("NewWaveLabel");
	cGameElementDef asteroidDef;
	asteroidDef.strModelName= "asteroid";
	asteroidDef.vPosition= cVector3(-100.0f, -100.0f, -100.0f);
	m_iNumberOfAsteroids = m_iCurrentLevel + 2;
	for(int i=0; i < m_iNumberOfAsteroids; i++)
	{
		shared_ptr<cAsteroidGameElement> pAsteroid(DEBUG_NEW cAsteroid());
		pAsteroid->VInitialize(asteroidDef);
		m_pGameElements.push_back(pAsteroid);
	}
	m_pGameElements.front()->CastToShip()->SetInvincible();
}

// *****************************************************************************
void cGame::AddAsteroid(const cGameElementDef & asteroidDef, const int iSize)
{
	m_iNumberOfAsteroids++;
	shared_ptr<cAsteroidGameElement> pGameElement(DEBUG_NEW cAsteroid());
	shared_ptr<cAsteroid> pAsteroid  = dynamic_pointer_cast<cAsteroid>(pGameElement);
	pAsteroid->InitializeFromParent(asteroidDef, iSize);
	AddGameElement(pGameElement);

}

// *****************************************************************************
IBaseApp * IGame::CreateGame(const cString strName)
{
	return DEBUG_NEW cGame(strName);
}

