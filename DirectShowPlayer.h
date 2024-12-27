
// DirectShowPlayer.h: Hauptheaderdatei für die PROJECT_NAME-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error "'pch.h' vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"		// Hauptsymbole


// CDirectShowPlayerApp:
// Siehe DirectShowPlayer.cpp für die Implementierung dieser Klasse
//

class CDirectShowPlayerApp : public CWinApp
{
public:
	CDirectShowPlayerApp();

// Überschreibungen
public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CDirectShowPlayerApp theApp;
