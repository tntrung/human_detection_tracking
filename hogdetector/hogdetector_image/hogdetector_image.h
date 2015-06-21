
// hogdetector_image.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Chogdetector_imageApp:
// See hogdetector_image.cpp for the implementation of this class
//

class Chogdetector_imageApp : public CWinApp
{
public:
	Chogdetector_imageApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Chogdetector_imageApp theApp;