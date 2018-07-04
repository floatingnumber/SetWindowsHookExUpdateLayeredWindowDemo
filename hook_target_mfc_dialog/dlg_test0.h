#pragma once

#include "gdi_image_render.h"

// dlg_test0 dialog

class dlg_test0 : public CDialogEx
{
	DECLARE_DYNAMIC(dlg_test0)

public:
	dlg_test0(CWnd* pParent = NULL);   // standard constructor
	virtual ~dlg_test0();

	// Dialog Data
	enum { IDD = IDD_DIALOG_TEST0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();

public:
	void SetParentDlg(CWnd * dlg){ parentDlg = dlg; }
	void SetDrawRect(CRect & crt){ m_DrawRect = crt; }

private:
	CRect m_DrawRect;
	CWnd *parentDlg;
	CDC *m_pMemDC;
	CBitmap* m_pBitmap;
	int m_nScreenX;
	int m_nScreenY;
	gdi_image_render m_BK;
	CBrush m_brush;
	HICON m_hIcon;

	void DrawSomething();

public:
	afx_msg void OnBnClickedButton0();
};
