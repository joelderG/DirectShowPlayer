#pragma once
#include "atlstr.h"

class DirectShow
{
public: 
	DirectShow();
	~DirectShow();

	bool Initialize();
	void Play(const wchar_t* filename);
	void Pause();
	void Resume();
	void Stop();
	void Close();
	void CleanUp();
	void SetFullScreen(bool enable);
	void SetVideoWindowPosition(int x, int y, int width, int height);
	void SetFileName(CString selectedFile);

	LRESULT GetIt();
	CString FileName = _T("example.mpg");
	IMediaEventEx* pEvent;
	IMediaSeeking* pSeek;
	IVideoWindow* pVidWin;
private:
	IGraphBuilder* pGraph;
	IMediaControl* pMediaControl;
	REFERENCE_TIME rtTotal;
};

