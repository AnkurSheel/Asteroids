// *****************************************************************************
//  RedefineControlsScreen   version:  1.0   Ankur Sheel  date: 2013/03/20
// *****************************************************************************
//  purpose:	
// *****************************************************************************
#ifndef RedefineControlsScreen_h__
#define RedefineControlsScreen_h__

#include "Game/GameFlowStates.h"

namespace Graphics
{
	struct stUIEventCallbackParam;
	class IBaseControl;
}

class cStateRedefineControlsScreen 
	: public IGameFlowStates
{
public:
	static cStateRedefineControlsScreen * Instance();
	~cStateRedefineControlsScreen();
	virtual void VOnEnter(cGame * pGame);
	virtual void VOnUpdate();
	virtual void VOnExit();
	virtual bool VOnMessage(const AI::Telegram & msg);

private:
	cStateRedefineControlsScreen();
	void OKButtonPressed(const Graphics::stUIEventCallbackParam& params);
	void CancelButtonPressed(const Graphics::stUIEventCallbackParam& params);
	void ResetButtonPressed(const Graphics::stUIEventCallbackParam& params);
	void OnKeyChanged(unsigned int uiCharId);
	void SetAllKeyControlsText();

private:
	Graphics::IBaseControl *	m_pRedefineControlsScreen;
	unsigned int				m_icurrentlyEditingID;
	std::vector<Graphics::IBaseControl *> m_pKeyControls;
};
#endif // RedefineControlsScreen_h__