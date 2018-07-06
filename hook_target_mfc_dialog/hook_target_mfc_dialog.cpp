// hook_target_mfc_dialog.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "hook_target_mfc_dialog.h"
#include "util.h"
#include "dlg_test0.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HHOOK g_hhook_wnd_ret = NULL;

HWND g_hwnd = NULL;
CBrush g_brush;
BLENDFUNCTION g_blend;
Gdiplus::Image *g_gdi_pimage;
CRect g_CenterArea;
CWnd *parentDlg;
CBrush m_brush;
CDC *m_pMemDC;
CBitmap *m_pBitmap;
int m_nScreenX;
int m_nScreenY;
dlg_test0 *m_pMainDlg;

WNDPROC g_old_proc;
static bool g_subclassed = false;

class resource_handle
{
public:
	resource_handle()
	{
		m_hinstance = NULL;
	}

	~resource_handle()
	{
		if (m_hinstance)
			FreeLibrary(m_hinstance);
	}

public:
	bool load(const CString &module)
	{
		m_hinstance = LoadLibrary(module);
		return (m_hinstance == NULL ? false : true);
	}

	HINSTANCE get_hinstance()
	{
		return m_hinstance;
	}

private:
	HINSTANCE m_hinstance;
};

resource_handle g_rh;

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// Chook_target_mfc_dialogApp

BEGIN_MESSAGE_MAP(Chook_target_mfc_dialogApp, CWinApp)
END_MESSAGE_MAP()


// Chook_target_mfc_dialogApp construction

Chook_target_mfc_dialogApp::Chook_target_mfc_dialogApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Chook_target_mfc_dialogApp object

Chook_target_mfc_dialogApp theApp;


// Chook_target_mfc_dialogApp initialization

BOOL Chook_target_mfc_dialogApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

void ReSetChildDlg()
{
	CRect rcWin;
	GetWindowRect(g_hwnd, &rcWin);
	rcWin.left += g_CenterArea.left;
	rcWin.top += g_CenterArea.top;
	rcWin.right = rcWin.left + g_CenterArea.Width();
	rcWin.bottom = rcWin.top + g_CenterArea.Height();
	if (m_pMainDlg)
	{
		m_pMainDlg->MoveWindow(rcWin);
	}
}

void DrawTXBar()
{
	RECT rct;
	GetWindowRect(g_hwnd, &rct);

	HDC hdcTemp = GetDC(g_hwnd);
	HDC m_hdcMemory = CreateCompatibleDC(hdcTemp);
	HBITMAP hBitMap = CreateCompatibleBitmap(hdcTemp, rct.right, rct.bottom);
	SelectObject(m_hdcMemory, hBitMap);

	POINT ptWinPos = { rct.left, rct.top };

	Gdiplus::Graphics graphics(m_hdcMemory);
	RECT rcClient;
	GetClientRect(g_hwnd, &rcClient);

	graphics.DrawImage(g_gdi_pimage, Gdiplus::Rect(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom));

	SIZE sizeWindow = { rct.right, rct.bottom };
	POINT ptSrc = { 0, 0 };

	DWORD dwExStyle = GetWindowLong(g_hwnd, GWL_EXSTYLE);
	if ((dwExStyle & WS_EX_LAYERED) != WS_EX_LAYERED)
		SetWindowLong(g_hwnd, GWL_EXSTYLE, dwExStyle ^ WS_EX_LAYERED);

	HDC hdcScreen = GetDC(g_hwnd);
	::UpdateLayeredWindow(g_hwnd, hdcScreen, &ptWinPos, &sizeWindow, m_hdcMemory, &ptSrc, 0, &g_blend, 2);
	graphics.ReleaseHDC(m_hdcMemory);
	::ReleaseDC(g_hwnd, hdcScreen);
	hdcScreen = NULL;
	::ReleaseDC(g_hwnd, hdcTemp);
	hdcTemp = NULL;
	DeleteObject(hBitMap);
	DeleteDC(m_hdcMemory);
	m_hdcMemory = NULL;
}

void set_image()
{
	GdiImageFromResource(&g_gdi_pimage, IDB_PNG_BK, L"PNG");
	if (g_gdi_pimage)
	{
		CWnd *pcwnd = CWnd::FromHandle(g_hwnd);
		pcwnd->MoveWindow(CRect(0, 0, g_gdi_pimage->GetWidth(), g_gdi_pimage->GetHeight()));
		pcwnd->CenterWindow();
		g_CenterArea = CRect(31, 24, g_gdi_pimage->GetWidth() - 27, g_gdi_pimage->GetHeight() - 23);
		ReSetChildDlg();
		m_pMainDlg->SetDrawRect(g_CenterArea);
		DrawTXBar();
	}
}

LRESULT CALLBACK new_proc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (Msg == WM_CTLCOLORBTN)
	{
		HBRUSH hbrush;
		hbrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		SetBkMode((HDC)wParam, TRANSPARENT);
		return ((LRESULT)hbrush);
	}

	if (Msg == WM_DESTROY)
	{
		g_subclassed = false;
	}

	return CallWindowProc(g_old_proc, hWnd, Msg, wParam, lParam);
}

LRESULT CALLBACK CallWndRetProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	CWPRETSTRUCT *p = (CWPRETSTRUCT *)lParam;

	switch (p->message)
	{
	case WM_INITDIALOG:
		{
			if (g_subclassed)
			{
				break;
			}
			g_subclassed = true;

			OutputDebugString(L"WM_INITDIALOG");

			Gdiplus::GdiplusStartupInput input;
			ULONG_PTR token;
			GdiplusStartup(&token, &input, NULL);

			HWND hwnd = FindWindow(NULL, L"target_mfc_dialog");
			if (NULL == hwnd)
			{
				OutputDebugString(L"FindWindow target_mfc_dialog error");
				break;
			}

			g_hwnd = hwnd;

			// OnCreate
			CDC *pDC = CDC::FromHandle(GetDC(hwnd));
			m_pMemDC = new CDC();
			m_pBitmap = new CBitmap();
			m_nScreenX = 556;
			m_nScreenY = 397;
			m_pMemDC->CreateCompatibleDC(pDC);
			m_pBitmap->CreateCompatibleBitmap(pDC, m_nScreenX, m_nScreenY);
			CBitmap *oldBitmap = m_pMemDC->SelectObject(m_pBitmap);
			CBrush brush(RGB(255, 255, 255));
			CRect rect;
			GetClientRect(hwnd, &rect);
			m_pMemDC->FillRect(CRect(rect.left, rect.top, m_nScreenX, m_nScreenY), &brush);
			m_pMemDC->SelectObject(oldBitmap);
			ReleaseDC(hwnd, pDC->GetSafeHdc());

			// fix "Debug Assertion Failed!" at dlgcore.cpp:213
			if (g_rh.load(L"hook_target_mfc_dialog"))
			{
				AfxSetResourceHandle(g_rh.get_hinstance());

				m_pMainDlg = new dlg_test0(CWnd::FromHandle(hwnd));
				if (m_pMainDlg)
				{
					// here will cause dlgcore.cpp:213 "Debug Assertion Failed!"
					// ERROR: Cannot find dialog template with IDD IDD_DIALOG_TEST0
					m_pMainDlg->Create(IDD_DIALOG_TEST0, CWnd::FromHandle(hwnd));
					m_pMainDlg->ShowWindow(SW_SHOW);
					m_pMainDlg->SetParentDlg(CWnd::FromHandle(hwnd));
				}
			}

			// OnInitDialog
			COLORREF transColor = RGB(0, 255, 0);
			g_brush.CreateSolidBrush(transColor);

			DWORD dwExStyle = GetWindowLong(hwnd, GWL_STYLE);
			SetWindowLong(hwnd, GWL_STYLE, dwExStyle | WS_EX_LAYERED);

			// When bAlpha is 0, the window is completely transparent,
			// When bAlpha is 255, the window is opaque.
			SetLayeredWindowAttributes(hwnd, transColor, 0, LWA_COLORKEY);

			DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);

			VERIFY((dwStyle & WS_POPUP) != 0);
			VERIFY((dwStyle & WS_CHILD) == 0);

			g_blend.BlendOp = 0;
			g_blend.BlendFlags = 0;
			g_blend.AlphaFormat = 1;
			g_blend.SourceConstantAlpha = 255;

			set_image();

			g_old_proc = (WNDPROC)SetWindowLong(hwnd, GWL_WNDPROC, (LONG)new_proc);
		}
		break;
	}

	return CallNextHookEx(g_hhook_wnd_ret, nCode, wParam, lParam);
}

extern "C" __declspec(dllexport) void BegDialogHook(HWND hwnd)
{
	DWORD tid = GetWindowThreadProcessId(hwnd, NULL);
	if (0 == tid)
	{
		OutputDebugString(L"GetWindowThreadProcessId return 0");
		return;
	}

	g_hhook_wnd_ret = SetWindowsHookEx(
		WH_CALLWNDPROCRET, CallWndRetProc, GetModuleHandle(L"hook_target_mfc_dialog"), tid);
}

extern "C" __declspec(dllexport) void EndDialogHook()
{
	if (NULL != g_hhook_wnd_ret)
		UnhookWindowsHookEx(g_hhook_wnd_ret);
}
