// cbt_hook.h : main header file for the cbt_hook DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Ccbt_hookApp
// See cbt_hook.cpp for the implementation of this class
//

class Ccbt_hookApp : public CWinApp
{
public:
	Ccbt_hookApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
