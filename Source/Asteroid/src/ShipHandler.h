// *****************************************************************************
//  ShipHandler   version:  1.0   Ankur Sheel  date: 2013/02/21
//  ----------------------------------------------------------------------------
//  
//  ----------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// *****************************************************************************
// 
// *****************************************************************************
#ifndef ShipHandler_h__
#define ShipHandler_h__


#include "KeyboardHandler.hxx"
namespace Graphics
{
	struct AppMsg;
}

class ShipHandler 
	: public Graphics::IKeyboardHandler
{
public:
	typedef function<void (ShipActions)> ShipInputCallBackFn;

public:
	ShipHandler();
	~ShipHandler();
	bool VOnKeyDown(const unsigned int iCharID);
	bool VOnKeyUp(const unsigned int iCharID);
	bool VOnCharPress(const unsigned int iCharID);
	void VLockKey(const BYTE c, const bool bLock);
	void OnUpdate();
	void RegisterCallBack(ShipInputCallBackFn callback);
	void UnregisterCallBack();
private:
	BYTE					m_bKey[KEYBOARD_KEYS];			// Which keys are up and down
	ShipInputCallBackFn		m_pfnCallBack;

};
#endif // ShipHandler_h__