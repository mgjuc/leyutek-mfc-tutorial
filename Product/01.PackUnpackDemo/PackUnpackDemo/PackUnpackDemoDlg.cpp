/*********************************************************************************************************
* 模块名称: PackUnpackDemoDlg.cpp
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
#include "PackUnpackDemo.h"
#include "PackUnpackDemoDlg.h"
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


// CPackUnpackDemoDlg 对话框


/*********************************************************************************************************
*                                              成员函数实现
*********************************************************************************************************/
CPackUnpackDemoDlg::CPackUnpackDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PACKUNPACKDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPackUnpackDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPackUnpackDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PACK, &CPackUnpackDemoDlg::OnBnClickedButtonPack)
	ON_BN_CLICKED(IDC_BUTTON_UNPACK, &CPackUnpackDemoDlg::OnBnClickedButtonUnpack)
END_MESSAGE_MAP()


// CPackUnpackDemoDlg 消息处理程序

BOOL CPackUnpackDemoDlg::OnInitDialog()
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
	SetDlgItemText(IDC_EDIT_MODID, "12");
	SetDlgItemText(IDC_EDIT_SECID, "02");
	SetDlgItemText(IDC_EDIT_BEFORE_PACK, "00 01 6E 01 70 00");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPackUnpackDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPackUnpackDemoDlg::OnPaint()
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
HCURSOR CPackUnpackDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*********************************************************************************************************
* 函数名称: StrToData
* 函数功能: 将文本字符串转换为数据
* 输入参数: str-待转换的字符串，data-存储转换完成的数据
* 输出参数:
* 返 回 值:
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CPackUnpackDemoDlg::StrToData(CString& str, UCHAR* data)
{
	CStringArray strArr;
	SplitString(str, ' ', strArr);
	for(int i = 0; i < strArr.GetSize(); i++)
	{
		// 将字符串转换成16进制长整数，GetAt返回位于给定索引处的值
		data[i] = strtol(strArr.GetAt(i), NULL, 16);
	}
}

/*********************************************************************************************************
* 函数名称: PackToStr
* 函数功能: 将包数据转换为字符串
* 输入参数: pack-待转换的包，str-存储转换完成的字符串
* 输出参数:
* 返 回 值:
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CPackUnpackDemoDlg::PackToStr(PacketFrame pack, CString& str, int packLen)
{
	CString strTmp;
	for(int i = 0; i < packLen; i++)
	{
		strTmp.Format(_T("%02X"), pack.buffer[i]);   // 将uchar类型转换成十六位进制数
		str = str + strTmp;
		if(i < packLen - 1)
		{
			str = str + " ";  // 在原有的字符串上添加新的字符串
		}
		else
		{
			str = str + "\n";
		}
	}
}

/*********************************************************************************************************
* 函数名称: SplitString
* 函数功能: 按照给定的符号进行字符串的分割
* 输入参数: str-要拆分的字符串，split-给定的符号，strArray-拆分后的字符串数组
* 输出参数:
* 返 回 值:
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
int CPackUnpackDemoDlg::SplitString(const CString str, char split, CStringArray& strArray)
{
	strArray.RemoveAll();  // 清零
	CString strTemp = str;
	int index = 0;
	while(1)
	{
		index = strTemp.Find(split);
		if(index >= 0)
		{
			strArray.Add(strTemp.Left(index));  // CString.left从左边开始获取前index个字符
			strTemp = strTemp.Right(strTemp.GetLength() - index - 1);
		}
		else
		{
			break;
		}
	}
	strArray.Add(strTemp);

	return strArray.GetSize();
}

/*********************************************************************************************************
* 函数名称: OnBnClickedButtonPack
* 函数功能:	打包按钮按下响应函数
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CPackUnpackDemoDlg::OnBnClickedButtonPack()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd* pWnd;
	CString strBeforePack;
	CString strAfterPack;
	CString moduleId;
	CString secondId;
	PacketFrame unpack;

	pWnd = GetDlgItem(IDC_EDIT_BEFORE_PACK);
	pWnd->GetWindowText(strBeforePack);
	StrToData(strBeforePack, unpack.data);

	pWnd = GetDlgItem(IDC_EDIT_MODID);
	pWnd->GetWindowText(moduleId);
	unpack.id = strtol(moduleId, NULL, 16);

	pWnd = GetDlgItem(IDC_EDIT_SECID);
	pWnd->GetWindowText(secondId);
	unpack.id2 = strtol(secondId, NULL, 16);

	mPackUnpack.PackData(&unpack);
	PackToStr(unpack, strAfterPack, 10);

	pWnd = GetDlgItem(IDC_EDIT_AFTER_PACK);
	pWnd->SetWindowText(strAfterPack);
}

/*********************************************************************************************************
* 函数名称: OnBnClickedButtonUnpack
* 函数功能:	解包按钮按下响应函数
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CPackUnpackDemoDlg::OnBnClickedButtonUnpack()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd* pWnd;
	CString strBeforeUnpack;
	CString strAfterUnpack;
	PacketFrame pack;
	BOOL findPack;

	pWnd = GetDlgItem(IDC_EDIT_BEFORE_UNPACK);
	pWnd->GetWindowText(strBeforeUnpack);
	StrToData(strBeforeUnpack, pack.buffer);
	findPack = mPackUnpack.UnpackData(pack.buffer);

	if(findPack)
	{
		mPackUnpack.GetUnpackRslt(&pack);
		pWnd = GetDlgItem(IDC_EDIT_AFTER_UNPACK);
		PackToStr(pack, strAfterUnpack, 8);
		pWnd->SetWindowText(strAfterUnpack);
	}
}
