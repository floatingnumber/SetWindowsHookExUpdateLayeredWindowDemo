// dlg_test0.cpp : implementation file
//

#include "stdafx.h"
#include "hook_target_mfc_dialog.h"
#include "dlg_test0.h"
#include "afxdialogex.h"


// dlg_test0 dialog

IMPLEMENT_DYNAMIC(dlg_test0, CDialogEx)

	dlg_test0::dlg_test0(CWnd* pParent /*=NULL*/)
	: CDialogEx(dlg_test0::IDD, pParent)
{

}

dlg_test0::~dlg_test0()
{
}

void dlg_test0::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(dlg_test0, CDialogEx)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON0, &dlg_test0::OnBnClickedButton0)
END_MESSAGE_MAP()


// dlg_test0 message handlers

void dlg_test0::DrawSomething()
{
	m_BK.Render(m_pMemDC->GetSafeHdc(), Gdiplus::Point(0, 0));
}

int dlg_test0::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CDC *pDC = GetDC();
	m_pMemDC = new CDC();
	m_pBitmap = new CBitmap();
	m_nScreenX = 1024;
	m_nScreenY = 640;
	m_pMemDC->CreateCompatibleDC(pDC);
	m_pBitmap->CreateCompatibleBitmap(pDC, m_nScreenX, m_nScreenY);
	CBitmap *oldBitmap = m_pMemDC->SelectObject(m_pBitmap);
	CBrush brush(RGB(255, 255, 255));
	CRect rect;
	GetClientRect(&rect);
	m_pMemDC->FillRect(CRect(rect.left, rect.top, m_nScreenX, m_nScreenY), &brush);
	m_pMemDC->SelectObject(oldBitmap);
	ReleaseDC(pDC);

	m_BK.SetImage(IDB_PNG_BK, L"PNG");

	return 0;
}


HBRUSH dlg_test0::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_DLG)
	{
		return m_brush;
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void dlg_test0::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDC *pDC = GetDC();
		CRect rect;
		GetClientRect(rect);
		CBitmap *oldBitmap = m_pMemDC->SelectObject(m_pBitmap);
		DrawSomething();
		pDC->BitBlt(rect.left, rect.top, m_DrawRect.Width(), m_DrawRect.Height(), m_pMemDC, m_DrawRect.left, m_DrawRect.top, SRCCOPY);
		m_pMemDC->SelectObject(oldBitmap);
	}
}


void dlg_test0::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	::SendMessage(parentDlg->GetSafeHwnd(), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);

	CDialogEx::OnLButtonDown(nFlags, point);
}


void dlg_test0::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	if (m_pMemDC)
	{
		delete m_pMemDC;
		m_pMemDC = NULL;
	}

	if (m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
}


void dlg_test0::OnBnClickedButton0()
{
	// TODO: Add your control notification handler code here
	AfxMessageBox(L"Button0 clicked");
}
