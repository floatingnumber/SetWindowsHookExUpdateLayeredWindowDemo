
// target_mfc_dialogDlg.h : header file
//

#pragma once


// Ctarget_mfc_dialogDlg dialog
class Ctarget_mfc_dialogDlg : public CDialogEx
{
// Construction
public:
	Ctarget_mfc_dialogDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TARGET_MFC_DIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
