// CRegistDlg.cpp: 实现文件
//

#include "pch.h"
#include "LingHelper.h"
#include "afxdialogex.h"
#include "CRegistDlg.h"
#include "CInfoFile.h"

// CRegistDlg 对话框

IMPLEMENT_DYNAMIC(CRegistDlg, CDialogEx)

CRegistDlg::CRegistDlg(CWnd *pParent /*=nullptr*/)
	: CDialogEx(IDD_REGIST, pParent), m_user(_T("")), m_newpwdeye(FALSE), m_ackpwdeye(FALSE), PasswordFrame(0)
{
}

CRegistDlg::~CRegistDlg()
{
}

void CRegistDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_read);
	DDX_Text(pDX, IDC_EDIT1, m_user);
	DDX_Control(pDX, IDC_EDIT3, m_newpwd);
	DDX_Control(pDX, IDC_EDIT2, m_ackpwd);
	DDX_Check(pDX, IDC_CHECK2, m_newpwdeye);
	DDX_Check(pDX, IDC_CHECK3, m_ackpwdeye);
}

BEGIN_MESSAGE_MAP(CRegistDlg, CDialogEx)
ON_BN_CLICKED(IDC_BUTTON2, &CRegistDlg::OnBnClickedButton2)
ON_BN_CLICKED(IDC_CHECK2, &CRegistDlg::OnBnClickedCheck2)
ON_BN_CLICKED(IDC_CHECK3, &CRegistDlg::OnBnClickedCheck3)
ON_BN_CLICKED(IDC_BUTTON4, &CRegistDlg::OnBnClickedButton4)
END_MESSAGE_MAP()

// CRegistDlg 消息处理程序

void CRegistDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	// 单击确认事件
	UpdateData(true);

	if (m_user.IsEmpty())
	{
		MessageBox(TEXT("用户名不能为空"));
		return;
	}
	CString newpwd;
	m_newpwd.GetWindowText(newpwd);
	if (newpwd.IsEmpty())
	{
		MessageBox(TEXT("新密码不能为空"));
		return;
	}
	CString ackpwd;
	m_ackpwd.GetWindowText(ackpwd);
	if (ackpwd.IsEmpty() || ackpwd != newpwd)
	{
		MessageBox(TEXT("未填写确认密码"));
		return;
	}
	if (!m_read.GetState())
	{
		MessageBox(TEXT("请勾选用户协议"));
		return;
	}

	CStringA Tuser;
	Tuser = m_user;
	CStringA Tackpwd;
	Tackpwd = ackpwd;
	CInfoFile file;
	file.WritePwd(Tuser.GetBuffer(), Tackpwd.GetBuffer());
	CDialogEx::OnCancel();
}

void CRegistDlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	// 新密码可见点击事件
	UpdateData(true);
	if (m_newpwdeye)
	{
		m_newpwd.SetPasswordChar(0);
	}
	else
	{
		m_newpwd.SetPasswordChar(PasswordFrame);
	}
	InvalidateRect(NULL);
}

void CRegistDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	// CDialogEx::OnOK();
}

void CRegistDlg::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	// 确认密码可见点击事件

	UpdateData(true);
	if (m_ackpwdeye)
	{
		m_ackpwd.SetPasswordChar(0);
	}
	else
	{
		m_ackpwd.SetPasswordChar(PasswordFrame);
	}
	InvalidateRect(NULL);
}

BOOL CRegistDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	PasswordFrame = m_newpwd.GetPasswordChar();

	return TRUE; // return TRUE unless you set the focus to a control
				 // 异常: OCX 属性页应返回 FALSE
}

void CRegistDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
