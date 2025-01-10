#pragma once
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

	LRESULT GetIt();

	IMediaEventEx* pEvent;	// COM-Interface / Behandlung der Ereignisse 
	IMediaSeeking* pSeek;	// Slidercontroll und Zeitsteuerung
	IVideoWindow* pVidWin;	// Interface zur Steuerung des Videofensters
private:
	IGraphBuilder* pGraph;	// COM-Interface / Aufbau des Filtergraphen
	IMediaControl* pMediaControl;	// COM-Interface / Interface zur Steuerung des Graphen
	REFERENCE_TIME rtTotal;
};

