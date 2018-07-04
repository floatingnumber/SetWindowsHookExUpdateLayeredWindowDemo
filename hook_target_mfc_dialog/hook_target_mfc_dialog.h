// hook_target_mfc_dialog.h : main header file for the hook_target_mfc_dialog DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Chook_target_mfc_dialogApp
// See hook_target_mfc_dialog.cpp for the implementation of this class
//

class Chook_target_mfc_dialogApp : public CWinApp
{
public:
	Chook_target_mfc_dialogApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
