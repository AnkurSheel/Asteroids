// *****************************************************************************
//  OptionScreen   version:  1.0   Ankur Sheel  date: 2013/03/20
// *****************************************************************************
//  purpose:	
// *****************************************************************************
#include "stdafx.h"
#include "OptionScreen.h"
#include "Game\Game.h"
#include "HumanView.h"
#include "ControlStructures.h"
#include "BaseControl.hxx"
#include "FSM\Telegram.h"
#include "Game\GameFlowStateMachine.h"
#include "GameDirectories.h"
#include "RedefineControlsScreen.h"

using namespace Base;
using namespace Graphics;
using namespace GameBase;
using namespace AI;
using namespace Utilities;

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
		buttonDef.strDefaultImage = "buttonDefault.png";
		buttonDef.strPressedImage = "buttonPressed.png";
		buttonDef.labelControlDef.strFont = "licorice";
		buttonDef.labelControlDef.textColor = cColor::BLUE;
		buttonDef.labelControlDef.fTextHeight = 50;

		buttonDef.strControlName = "btnControls";
		buttonDef.labelControlDef.strText = "Redefine Controls";
		buttonDef.vPosition = cVector2(660, 230);

		IBaseControl * pRedefineControlsButton = IBaseControl::CreateButtonControl(buttonDef);
		m_pOptionsScreen->VAddChildControl(shared_ptr<IBaseControl>(pRedefineControlsButton));
		UIEventCallBackFn callbackRedefineControlsBtn = bind(&cStateOptionsScreen::ControlsButtonPressed, this, _1);
		pRedefineControlsButton->VRegisterCallBack(UIET_BTNRELEASED, callbackRedefineControlsBtn);

		buttonDef.strControlName = "btnQuit";
		buttonDef.vPosition = cVector2(0, 550);
		buttonDef.labelControlDef.strText = "Back";

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

// *****************************************************************************
void cStateOptionsScreen::ControlsButtonPressed(const stUIEventCallbackParam & params)
{
	if(m_pOwner != NULL && m_pOwner->m_pStateMachine != NULL)
	{
		m_pOwner->m_pStateMachine->RequestPushState(cStateRedefineControlsScreen::Instance());
	}
}