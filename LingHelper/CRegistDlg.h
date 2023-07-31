#pragma once
#include "afxdialogex.h"

// CRegistDlg 对话框

class CRegistDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegistDlg)

public:
	CRegistDlg(CWnd *pParent = nullptr); // 标准构造函数
	virtual ~CRegistDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum
	{
		IDD = IDD_REGIST
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CButton m_read;

public:
	afx_msg void OnBnClickedButton2();

private:
	CString m_user;
	CEdit m_newpwd;
	CEdit m_ackpwd;

public:
	afx_msg void OnBnClickedCheck2();

private:
	BOOL m_newpwdeye;
	BOOL m_ackpwdeye;
	TCHAR PasswordFrame;
	virtual void OnOK();

public:
	afx_msg void OnBnClickedCheck3();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton4();
};
