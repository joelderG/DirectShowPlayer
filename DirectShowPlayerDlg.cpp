
// DirectShowPlayerDlg.cpp: Implementierungsdatei
//

#include "pch.h"
#include "framework.h"
#include "DirectShowPlayer.h"
#include "DirectShowPlayerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static UINT NEAR WM_GRAPHNOTIFY = RegisterWindowMessage(L"GRAPHNOTIFY");

// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialogfelddaten
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

// Implementierung
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDirectShowPlayerDlg-Dialogfeld



CDirectShowPlayerDlg::CDirectShowPlayerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIRECTSHOWPLAYER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDirectShowPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDirectShowPlayerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_PLAY, &CDirectShowPlayerDlg::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_PAUSE, &CDirectShowPlayerDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_Resume, &CDirectShowPlayerDlg::OnBnClickedResume)
	ON_REGISTERED_MESSAGE(WM_GRAPHNOTIFY, GetIt)
	ON_BN_CLICKED(IDC_STOP, &CDirectShowPlayerDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_Close, &CDirectShowPlayerDlg::OnBnClickedClose)
	ON_WM_TIMER()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_FULLSCR, &CDirectShowPlayerDlg::OnBnClickedFullscr)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_OPEN, &CDirectShowPlayerDlg::OnBnClickedOpen)
END_MESSAGE_MAP()


// CDirectShowPlayerDlg-Meldungshandler

BOOL CDirectShowPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Hinzufügen des Menübefehls "Info..." zum Systemmenü.

	// IDM_ABOUTBOX muss sich im Bereich der Systembefehle befinden.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Symbol für dieses Dialogfeld festlegen.  Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

	// TODO: Hier zusätzliche Initialisierung einfügen
	m_DirectShow.Initialize();
	SetTimer(1, 200, 0);

	if (m_DirectShow.pSeek) {
		REFERENCE_TIME d;
		m_DirectShow.pSeek->GetDuration(&d);
		CSliderCtrl* sl;
		sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER);
		sl->SetRange(0, (int)(d / 1000000)); sl->SetPos(0);
	}


	return TRUE;  // TRUE zurückgeben, wenn der Fokus nicht auf ein Steuerelement gesetzt wird
}

void CDirectShowPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// Wenn Sie dem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie
//  den nachstehenden Code, um das Symbol zu zeichnen.  Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch ausgeführt.

void CDirectShowPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext zum Zeichnen

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Symbol in Clientrechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Die System ruft diese Funktion auf, um den Cursor abzufragen, der angezeigt wird, während der Benutzer
//  das minimierte Fenster mit der Maus zieht.
HCURSOR CDirectShowPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDirectShowPlayerDlg::OnBnClickedPlay()
{
	m_DirectShow.Play(L"example.mpg");
	m_DirectShow.pVidWin->put_Owner((OAHWND)GetSafeHwnd());
}

void CDirectShowPlayerDlg::OnBnClickedPause()
{
	m_DirectShow.Pause();
}


void CDirectShowPlayerDlg::OnBnClickedResume()
{
	m_DirectShow.Resume();
}


void CDirectShowPlayerDlg::OnBnClickedStop()
{
	m_DirectShow.Stop();
}


void CDirectShowPlayerDlg::OnBnClickedClose()
{
	m_DirectShow.Close();
	CDialogEx::OnClose();
}

void CDirectShowPlayerDlg::OnBnClickedFullscr()
{
	m_DirectShow.SetFullScreen(true);
}

void CDirectShowPlayerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_DirectShow.SetFullScreen(false);
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDirectShowPlayerDlg::OnBnClickedOpen()
{
	// Filter für unterstützte Dateitypen (nur MP4 und alle Dateien)
	CString filter = L"Video Files (.mp4;.mpg)|.mp4;.mpg|All Files (.)|.||";

	// Datei-Dialog initialisieren
	CFileDialog fileDialog(TRUE, L"mp4", NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this);

	// Zeige den Dialog an und überprüfe, ob der Benutzer eine Datei ausgewählt hat
	if (fileDialog.DoModal() == IDOK) {
		// Hole den Pfad der ausgewählten Datei
		CString selectedFile = fileDialog.GetPathName();
		m_DirectShow.SetFileName(selectedFile);

		// Bereinige vorherige Medienressourcen
		m_DirectShow.CleanUp();
		//Video abstpielen
		OnBnClickedPlay();
	}
}

LRESULT CDirectShowPlayerDlg::GetIt(WPARAM wparam, LPARAM lparam) {
	/*long evCode;
	LONG_PTR param1, param2;
	while ((m_DirectShow.pEvent != NULL) && SUCCEEDED(m_DirectShow.pEvent->GetEvent(&evCode, &param1, &param2, 0))) {
		m_DirectShow.pEvent->FreeEventParams(evCode, param1, param2);
		switch (evCode) {
		case EC_COMPLETE:
		case EC_USERABORT:
			m_DirectShow.CleanUp(); return 0;
		}
	}
	return 0;
	*/

	m_DirectShow.GetIt();
	return 0;
}

void CDirectShowPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (m_DirectShow.pSeek) {
		REFERENCE_TIME rtTotal, rtNow = 0; CString s;
		m_DirectShow.pSeek->GetDuration(&rtTotal);
		m_DirectShow.pSeek->GetCurrentPosition(&rtNow);
		s.Format(L"Abspielvorgang: %02d:%02d (%d%%)",
			(int)((rtNow / 10000000L) / 60), // min
			(int)((rtNow / 10000000L) % 60), // sek
			(int)((rtNow * 100) / rtTotal)); // Prozent
		GetDlgItem(IDC_STATUS)->SetWindowText(s);

		((CSliderCtrl*)GetDlgItem(IDC_SLIDER))->SetPos((int)(rtNow / 1000000));
	}
	else {
		GetDlgItem(IDC_STATUS)->SetWindowText(L"Abspielvorgang: 00:00 (0%)");
		((CSliderCtrl*)GetDlgItem(IDC_SLIDER))->SetPos(0);
	}
	CDialog::OnTimer(nIDEvent);
}

void CDirectShowPlayerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSliderCtrl* sl1 = (CSliderCtrl*)GetDlgItem(IDC_SLIDER);
	if ((m_DirectShow.pSeek) && ((void*)sl1 == (void*)pScrollBar)) {
		REFERENCE_TIME pos = (REFERENCE_TIME)sl1->GetPos() * 1000000;
		m_DirectShow.pSeek->SetPositions(&pos, AM_SEEKING_AbsolutePositioning,
			NULL, AM_SEEKING_NoPositioning);
	}	
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}