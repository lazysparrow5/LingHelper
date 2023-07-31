// CLoginDlg.cpp: 实现文件
//

#include "pch.h"
#include "LingHelper.h"
#include "afxdialogex.h"
#include "CLoginDlg.h"
#include "CInfoFile.h"
#include "CRegistDlg.h"

// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd *pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGIN, pParent), m_user(_T("")), m_pwd(_T(""))
{
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_user);
	DDX_Text(pDX, IDC_EDIT2, m_pwd);
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
ON_BN_CLICKED(IDC_BUTTON1, &CLoginDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON2, &CLoginDlg::OnBnClickedButton2)
ON_BN_CLICKED(IDC_BUTTON3, &CLoginDlg::OnBnClickedButton3)
END_MESSAGE_MAP()

// CLoginDlg 消息处理程序

void CLoginDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	// 登录按钮点击事件

	UpdateData(TRUE);
	if (m_user.IsEmpty())
	{
		MessageBox(TEXT("用户名不能为空"));
		return;
	}
	if (m_pwd.IsEmpty())
	{
		MessageBox(TEXT("密码不能为空"));
		return;
	}

	CInfoFile file;
	CString name, pwd;
	if (!file.ReadLogin(name, pwd))
	{
		MessageBox(TEXT("用户名不存在"));
		return;
	}
	if (name != m_user || pwd != m_pwd) // 用户名一致
	{
		MessageBox(TEXT("用户名或密码错误"));
		return;
	}
	CDialogEx::OnCancel();
}

void CLoginDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
}

void CLoginDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	exit(0);
	// CDialogEx::OnCancel();
}

BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 默认登录信息
	CInfoFile file;
	CString name, pwd;
	if (file.ReadLogin(name, pwd))
	{
		m_user = name;
		m_pwd = pwd;
		UpdateData(false);
	}

	return TRUE; // return TRUE unless you set the focus to a control
				 // 异常: OCX 属性页应返回 FALSE
}

void CLoginDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	exit(0);
}

void CLoginDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	// 注册按钮点击事件
	CRegistDlg dlg;
	dlg.DoModal();
}
