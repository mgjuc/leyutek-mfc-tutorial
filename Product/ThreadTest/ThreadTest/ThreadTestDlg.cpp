/*********************************************************************************************************
* 模块名称: ThreadTestDlg.cpp
* 使用说明:
* 摘    要: 类源文件
* 当前版本: 1.0.0
* 作    者: Leyutek(COPYRIGHT 2018 - 2021 Leyutek. All rights reserved.)
* 完成日期: 2021年01月22日
* 内    容:
* 注    意: none
**********************************************************************************************************
* 取代版本:
* 作    者:
* 完成日期:
* 修改内容:
* 修改文件:
*********************************************************************************************************/

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "pch.h"
#include "framework.h"
#include "ThreadTest.h"
#include "ThreadTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CThreadTestDlg 对话框

/*********************************************************************************************************
*                                              成员函数实现
*********************************************************************************************************/
CThreadTestDlg::CThreadTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_THREADTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CThreadTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CThreadTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CThreadTestDlg::OnBnClickedButtonStart)
END_MESSAGE_MAP()


// CThreadTestDlg 消息处理程序

BOOL CThreadTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	GetDlgItem(IDC_STATIC_COUNTSTATE)->SetWindowPos(NULL, 25, 23, 104, 26, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_COUNTSTATE)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_NUM)->SetWindowPos(NULL, 70, 72, 14, 20, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_NUM)->SetWindowText(_T(""));
	GetDlgItem(IDC_BUTTON_START)->SetWindowPos(NULL, 17, 121, 128, 42, SWP_NOZORDER);

	mNewFont.CreatePointFont(150, _T("黑体"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CThreadTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CThreadTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CThreadTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*********************************************************************************************************
* 函数名称: CountStart
* 函数功能:	计数开始
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CThreadTestDlg::CountStart()
{
	GetDlgItem(IDC_STATIC_COUNTSTATE)->SetWindowText(_T("计时开始"));
	GetDlgItem(IDC_STATIC_COUNTSTATE)->SetFont(&mNewFont);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);
}

/*********************************************************************************************************
* 函数名称: CountOver
* 函数功能:	计数结束
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CThreadTestDlg::CountOver()
{
	GetDlgItem(IDC_STATIC_COUNTSTATE)->SetWindowText(_T("计时结束"));
	GetDlgItem(IDC_STATIC_COUNTSTATE)->SetFont(&mNewFont);

	GetDlgItem(IDC_STATIC_NUM)->SetWindowText(_T(""));

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
}

/*********************************************************************************************************
* 函数名称: CountThread
* 函数功能:	子线程实现
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
UINT CThreadTestDlg::CountThread(void* param)
{
	CThreadTestDlg* dlg = (CThreadTestDlg*)param;

	CStatic* countNum = (CStatic*)dlg->GetDlgItem(IDC_STATIC_NUM);

	CString num;

	for (int i = 5; i >= 0; i--)
	{
		num.Format(_T("%d"), i);

		Sleep(1000);

		countNum->SetWindowText(num);
	}

	Sleep(1000);

	dlg->CountOver();

	return 0;
}

/*********************************************************************************************************
* 函数名称: OnBnClickedButtonStart
* 函数功能:	开始按钮按下响应函数
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CThreadTestDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxBeginThread(CountThread, this);

	CountStart();
}
