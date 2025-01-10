#include "pch.h"
#include "DirectShow.h"
#include <afxwin.h>
#include <atlbase.h>

DirectShow::DirectShow()
	: pGraph(nullptr), pMediaControl(nullptr), pEvent(nullptr),
	pVidWin(nullptr), pSeek(nullptr), rtTotal(0) {

}

DirectShow::~DirectShow() {
	CleanUp();
}

bool DirectShow::Initialize() {
	CoInitialize(NULL); // COM initialisieren
	return true;
}

void DirectShow::Play(const wchar_t* filename) {
	// COM-Instanz des Filtergraphen erstellen
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder, (void**)&pGraph);

	pGraph->QueryInterface(IID_IMediaControl, (void**)&pMediaControl);
	pGraph->QueryInterface(IID_IMediaEventEx, (void**)&pEvent);

	pGraph->RenderFile(filename, NULL); // Filtergraph wird aufgebaut

	pGraph->QueryInterface(IID_IVideoWindow, (void**)&pVidWin);
	pGraph->QueryInterface(IID_IMediaSeeking, (void**)&pSeek);

	// set timeformat to 100-nanoseconds units
	if (pSeek->IsFormatSupported(&TIME_FORMAT_MEDIA_TIME) == S_OK)
		pSeek->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);
	else
		AfxMessageBox(L"Zeitformat wird nicht unterstützt");

	// Slider initialisierung
	if (pSeek) {
		pSeek->GetDuration(&rtTotal);
	}
	pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
	pVidWin->put_Visible(OATRUE);

	SetVideoWindowPosition(10, 10, 250, 150);

	pVidWin->put_Visible(OAFALSE);
	pVidWin->put_Owner(NULL);
	pVidWin->Release();

	// Nachrichtenbehandlung (Maus, Tastatur)
	pVidWin->put_MessageDrain((OAHWND)AfxGetMainWnd()->GetSafeHwnd());

	pMediaControl->Run(); long evCode;
}

void DirectShow::Pause() {
	if (pMediaControl)
		pMediaControl->Pause();
}

void DirectShow::Resume() {
	if (pMediaControl)
		pMediaControl->Run();
}

void DirectShow::Stop() {
	if (pMediaControl)
		pMediaControl->Stop();
}

void DirectShow::Close() {
	CleanUp();
}

void DirectShow::CleanUp() {
	if (pVidWin) {
		pVidWin->put_Visible(OAFALSE);
		pVidWin->put_Owner(NULL);
		pVidWin->Release();
		pVidWin = NULL;
	}
	if (pSeek) {
		pSeek->Release();
		pSeek = NULL;
	}
	if (pMediaControl) {
		pMediaControl->Release();
		pMediaControl = NULL;
	}
	if (pEvent) {
		pEvent->Release();
		pEvent = NULL;
	}
	if (pGraph) {
		pGraph->Release();
		pGraph = NULL;
	}
	CoUninitialize();
}

LRESULT DirectShow::GetIt() {
	long evCode;
	LONG_PTR param1, param2;

	// Ereignisse abrufen und behandeln
	while ((pEvent != nullptr) && SUCCEEDED(pEvent->GetEvent(&evCode, &param1, &param2, 0))) {
		pEvent->FreeEventParams(evCode, param1, param2);

		switch (evCode) {
		case EC_COMPLETE:
		case EC_USERABORT:
			CleanUp();  // Bereinigt Ressourcen nach Abspielen oder Abbruch
			return 0;
		}
	}
	return 0;
}

void DirectShow::SetFullScreen(bool enable) {
	if (pGraph) {
		IVideoWindow* pVidWin1 = NULL;
		pGraph->QueryInterface(IID_IVideoWindow, (void**)&pVidWin1);
		pVidWin1->put_FullScreenMode(enable ? OATRUE : OAFALSE);
		pVidWin1->Release();
	}
}

void DirectShow::SetVideoWindowPosition(int x, int y, int width, int height) {
	if (pVidWin) {
		pVidWin->SetWindowPosition(x, y, width, height);
	}
}