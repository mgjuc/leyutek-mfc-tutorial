/*********************************************************************************************************
* 模块名称: FuncPointerTestDlg.cpp
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
#include "FuncPointerTest.h"
#include "FuncPointerTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef CString(*OperationSelect)(const int, const int);
static CString Calculator(OperationSelect operaSel, const int number1, const int number2);

static CString FuncPlus(const int number1, const int number2);
static CString FuncSubtract(const int number1, const int number2);
static CString FuncMultiply(const int number1, const int number2);
static CString FuncDevide(const int number1, const int number2);

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


// CFuncPointerTestDlg 对话框

/*********************************************************************************************************
*                                              成员函数实现
*********************************************************************************************************/
CFuncPointerTestDlg::CFuncPointerTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FUNCPOINTERTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFuncPointerTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_OPERATION, mOperation);
}

BEGIN_MESSAGE_MAP(CFuncPointerTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CALCULATE, &CFuncPointerTestDlg::OnBnClickedButtonCalculate)
END_MESSAGE_MAP()


// CFuncPointerTestDlg 消息处理程序

BOOL CFuncPointerTestDlg::OnInitDialog()
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
	GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T(""));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFuncPointerTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFuncPointerTestDlg::OnPaint()
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
HCURSOR CFuncPointerTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*********************************************************************************************************
* 函数名称: FuncPlus
* 函数功能:	加运算实现函数
* 输入参数:	number1，number2
* 输出参数:	void
* 返 回 值: plusResult
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
static CString FuncPlus(const int number1, const int number2)
{
	CString plusResult;
	int result;
	result = number1 + number2;
	plusResult.Format(_T("%d"), result);
	return plusResult;
}

/*********************************************************************************************************
* 函数名称: FuncSubtract
* 函数功能:	减运算实现函数
* 输入参数:	number1，number2
* 输出参数:	void
* 返 回 值: subtractResult
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
static CString FuncSubtract(const int number1, const int number2)
{
	CString subtractResult;
	int result;
	result = number1 - number2;
	subtractResult.Format(_T("%d"), result);
	return subtractResult;
}

/*********************************************************************************************************
* 函数名称: FuncMultiply
* 函数功能:	乘运算实现函数
* 输入参数:	number1，number2
* 输出参数:	void
* 返 回 值: multiplyResult
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
static CString FuncMultiply(const int number1, const int number2)
{
	CString multiplyResult;
	int result;
	result = number1 * number2;
	multiplyResult.Format(_T("%d"), result);
	return multiplyResult;
}

/*********************************************************************************************************
* 函数名称: FuncDevide
* 函数功能:	除运算实现函数
* 输入参数:	number1，number2
* 输出参数:	void
* 返 回 值: devideResult
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
static CString FuncDevide(const int number1, const int number2)
{
	CString devideResult;
	float result;
	result = (float)number1 / (float)number2;
	devideResult.Format(_T("%.2f"), result);
	return devideResult;
}

/*********************************************************************************************************
* 函数名称: Calculator
* 函数功能:	运算函数
* 输入参数:	operaSel，number1，number2
* 输出参数:	void
* 返 回 值: operaSel(number1, number2)
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
static CString Calculator(OperationSelect operaSel, const int number1, const int number2)
{
	return operaSel(number1, number2);
}

/*********************************************************************************************************
* 函数名称: OnBnClickedButtonCalculate
* 函数功能:	计算按钮单击响应函数
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CFuncPointerTestDlg::OnBnClickedButtonCalculate()
{
	// TODO: 在此添加控件通知处理程序代码
	CString number1Str, number2Str;
	CString operateResult;
	GetDlgItemText(IDC_EDIT_NUMBER1, number1Str);
	GetDlgItemText(IDC_EDIT_NUMBER2, number2Str);

	int number1 = _ttoi(number1Str);
	int number2 = _ttoi(number2Str);

	int index = mOperation.GetCurSel();
	if (number1Str != _T("") && number2Str != _T(""))
	{
		switch (index)
		{
		case 0:
			operateResult = Calculator(FuncPlus, number1, number2);
			GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(operateResult);
			break;
		case 1:
			operateResult = Calculator(FuncSubtract, number1, number2);
			GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(operateResult);
			break;
		case 2:
			operateResult = Calculator(FuncMultiply, number1, number2);
			GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(operateResult);
			break;
		case 3:
			operateResult = Calculator(FuncDevide, number1, number2);
			GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(operateResult);
			break;
		default:
			break;
		}
	}
	
}
