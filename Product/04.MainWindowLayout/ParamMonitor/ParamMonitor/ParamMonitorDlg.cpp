﻿/*********************************************************************************************************
* 模块名称: ParamMonitorDlg.cpp
* 摘    要: 主界面实现
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
#include "ParamMonitor.h"
#include "ParamMonitorDlg.h"
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
public:
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

// CParamMonitorDlg 对话框

CParamMonitorDlg::CParamMonitorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PARAM_DIALOG, pParent)
	, mStrParamT1(_T("---"))
	, mStrParamT2(_T("---"))
	, mStrTempSensor1(_T("T1脱落"))
	, mStrTempSensor2(_T("T2脱落"))
	, mStrNIBPCuf(_T("---"))
	, mStrParamNd(_T("---"))
	, mStrParamNm(_T("---"))
	, mStrParamNPr(_T("---"))
	, mStrParamNs(_T("---"))
	, mStrParamSPO2(_T(""))
	, mStrParamSPr(_T(""))
	, mStrSPO2Sensor(_T(""))
	, mStrSPO2Finger(_T(""))
	, mStrParamRr(_T("---"))
	, mStrHR(_T("---"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	mCrBackground = BLACK; //黑色
	mCrECG1 = LIGHTGREEN;  //亮绿色
	mCrECG2 = LIGHTGREEN;  //亮绿色
	mCrSPO2 = CYAN;        //青色
	mCrResp = YELLOW;      //黄色
	mCrTemp = BLUE;        //品红色
	mCrNIBP = MAGENTA;     //红色
}

void CParamMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_WAVE1, mCtrlWaveECG1);
	DDX_Control(pDX, IDC_STATIC_WAVE2, mCtrlWaveECG2);
	DDX_Control(pDX, IDC_STATIC_WAVE_SPO2, mCtrlWaveSPO2);
	DDX_Control(pDX, IDC_STATIC_WAVE_RESP, mCtrlWaveResp);

	DDX_Control(pDX, IDC_STATIC_PARAM_RR, mCtrlParamRr);
	DDX_Text(pDX, IDC_STATIC_PARAM_RR, mStrParamRr);

	DDX_Control(pDX, IDC_STATIC_PARAM_T1, mCtrlParamT1);
	DDX_Text(pDX, IDC_STATIC_PARAM_T1, mStrParamT1);
	DDX_Control(pDX, IDC_STATIC_PARAM_T2, mCtrlParamT2);
	DDX_Text(pDX, IDC_STATIC_PARAM_T2, mStrParamT2);
	DDX_Text(pDX, IDC_STATIC_TEMP_SENSOR1, mStrTempSensor1);
	DDX_Text(pDX, IDC_STATIC_TEMP_SENSOR2, mStrTempSensor2);

	DDX_Control(pDX, IDC_STATIC_NIBP_CUF, mCtrlNIBPCuf);
	DDX_Text(pDX, IDC_STATIC_NIBP_CUF, mStrNIBPCuf);
	DDX_Control(pDX, IDC_STATIC_PARAM_NM, mCtrlParamNm);
	DDX_Text(pDX, IDC_STATIC_PARAM_NM, mStrParamNm);
	DDX_Control(pDX, IDC_STATIC_PARAM_NS, mCtrlParamNs);
	DDX_Text(pDX, IDC_STATIC_PARAM_NS, mStrParamNs);
	DDX_Control(pDX, IDC_STATIC_PARAM_ND, mCtrlParamNd);
	DDX_Text(pDX, IDC_STATIC_PARAM_ND, mStrParamNd);
	DDX_Control(pDX, IDC_STATIC_PARAM_NPR, mCtrlParamNpr);
	DDX_Text(pDX, IDC_STATIC_PARAM_NPR, mStrParamNPr);

	DDX_Control(pDX, IDC_STATIC_PARAM_SPO2, mCtrlParamSPO2);
	DDX_Text(pDX, IDC_STATIC_PARAM_SPO2, mStrParamSPO2);
	DDX_Control(pDX, IDC_STATIC_PARAM_SPR, mCtrlParamSpr);
	DDX_Text(pDX, IDC_STATIC_PARAM_SPR, mStrParamSPr);
	DDX_Text(pDX, IDC_STATIC_PARAM_SENSOR, mStrSPO2Sensor);
	DDX_Text(pDX, IDC_STATIC_PARAM_FINGER, mStrSPO2Finger);

	DDX_Control(pDX, IDC_STATIC_RA, mCtrlRA);
	DDX_Control(pDX, IDC_STATIC_LA, mCtrlLA);
	DDX_Control(pDX, IDC_STATIC_LL, mCtrlLL);
	DDX_Control(pDX, IDC_STATIC_V, mCtrlV);
	DDX_Control(pDX, IDC_STATIC_HR, mCtrlHR);
	DDX_Text(pDX, IDC_STATIC_HR, mStrHR);
}

BEGIN_MESSAGE_MAP(CParamMonitorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()

// CParamMonitorDlg 消息处理程序

BOOL CParamMonitorDlg::OnInitDialog()
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
	Initialize();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CParamMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CParamMonitorDlg::OnPaint()
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
//显示
HCURSOR CParamMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*********************************************************************************************************
* 函数名称: Initialize
* 函数功能: 设置所有组合框初始选择
* 输入参数:
* 输出参数:
* 返 回 值:
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CParamMonitorDlg::Initialize()
{
	// 设置字体大小 
	mNewFont.CreateFont(20,       // 1字体大小为20号 
		0,                        // 2nWidth 
		0,                        // 3nEscapement 
		0,                        // 4nOrientation 
		FW_BOLD,                  // 5nWeight  在0到1000之间指定字体的权值，如400表示标准体，
								  //   700表示黑（粗）体，如果此值为0，则使用缺省的权值。
		FALSE,                    // 6TRUE-斜体 
		FALSE,                    // 7bUnderline  TRUE，则字体增加下划线
		0,                        // 8cStrikeOut TRUE，则字体增加删除线
		ANSI_CHARSET,             // 9nCharSet  指定字符集
		OUT_DEFAULT_PRECIS,       // 10nOutPrecision  输出精度
		CLIP_DEFAULT_PRECIS,      // 11nClipPrecision 
		DEFAULT_QUALITY,          // 12nQuality 
		DEFAULT_PITCH | FF_SWISS, // 13nPitchAndFamily  
		_T("宋体"));              // 14lpszFac

	mNewFontLabel.CreateFont(18,  // 1字体大小为18号 
		0,                        // 2nWidth 
		0,                        // 3nEscapement 
		0,                        // 4nOrientation 
		FW_THIN,                  // 5nWeight  在0到1000之间指定字体的权值，如400表示标准体，
								  //   700表示黑（粗）体，如果此值为0，则使用缺省的权值。
		FALSE,                    // 6TRUE-斜体 
		FALSE,                    // 7bUnderline  TRUE，则字体增加下划线
		0,                        // 8cStrikeOut TRUE，则字体增加删除线
		ANSI_CHARSET,             // 9nCharSet  指定字符集
		OUT_DEFAULT_PRECIS,       // 10nOutPrecision  输出精度
		CLIP_DEFAULT_PRECIS,      // 11nClipPrecision 
		DEFAULT_QUALITY,          // 12nQuality 
		DEFAULT_PITCH | FF_SWISS, // 13nPitchAndFamily  
		_T("宋体"));              // 14lpszFac

	GetDlgItem(IDC_STATIC_PARAM_T1)->SetFont(&mNewFont);      //设置体温值的字体大小
	GetDlgItem(IDC_STATIC_PARAM_T2)->SetFont(&mNewFont);
	GetDlgItem(IDC_STATIC_TEMP_LABEL1)->SetFont(&mNewFontLabel);
	GetDlgItem(IDC_STATIC_TEMP_LABEL2)->SetFont(&mNewFontLabel);
	GetDlgItem(IDC_STATIC_TEMP_LABEL3)->SetFont(&mNewFontLabel);
	GetDlgItem(IDC_STATIC_TEMP_LABEL4)->SetFont(&mNewFontLabel);
	GetDlgItem(IDC_STATIC_TEMP_SENSOR1)->SetFont(&mNewFontLabel);
	GetDlgItem(IDC_STATIC_TEMP_SENSOR2)->SetFont(&mNewFontLabel);

	GetDlgItem(IDC_STATIC_PARAM_SPO2)->SetFont(&mNewFont);    //设置血氧值得字体大小
	GetDlgItem(IDC_STATIC_PARAM_SPR)->SetFont(&mNewFont);
	GetDlgItem(IDC_STATIC_PARAM_FINGER)->SetFont(&mNewFontLabel);
	GetDlgItem(IDC_STATIC_PARAM_SENSOR)->SetFont(&mNewFontLabel);
	GetDlgItem(IDC_STATIC_SPO2_LABEL1)->SetFont(&mNewFontLabel);
	GetDlgItem(IDC_STATIC_SPO2_LABEL2)->SetFont(&mNewFontLabel);
	GetDlgItem(IDC_STATIC_SPO2_LABEL3)->SetFont(&mNewFontLabel);
	GetDlgItem(IDC_STATIC_SPO2_LABEL4)->SetFont(&mNewFontLabel);
	GetDlgItem(IDC_STATIC_SPO2_LABEL5)->SetFont(&mNewFontLabel);

	GetDlgItem(IDC_STATIC_PARAM_NS)->SetFont(&mNewFont);      //设置无创压的字体大小
	GetDlgItem(IDC_STATIC_PARAM_ND)->SetFont(&mNewFont);
	GetDlgItem(IDC_STATIC_PARAM_NM)->SetFont(&mNewFont);
	GetDlgItem(IDC_STATIC_PARAM_NPR)->SetFont(&mNewFont);
	GetDlgItem(IDC_STATIC_NIBP_CUF)->SetFont(&mNewFont);
	GetDlgItem(IDC_STATIC_NIBP_LABEL1)->SetFont(&mNewFontLabel);
	GetDlgItem(IDC_STATIC_NIBP_LABEL2)->SetFont(&mNewFontLabel);
	GetDlgItem(IDC_STATIC_NIBP_LABEL3)->SetFont(&mNewFontLabel);
	GetDlgItem(IDC_STATIC_NIBP_LABEL4)->SetFont(&mNewFontLabel);
	GetDlgItem(IDC_STATIC_NIBP_LABEL5)->SetFont(&mNewFontLabel);

	GetDlgItem(IDC_STATIC_PARAM_RR)->SetFont(&mNewFont);      //设置呼吸字体大小
	GetDlgItem(IDC_STATIC_RESP_LABEL1)->SetFont(&mNewFontLabel);
	GetDlgItem(IDC_STATIC_RESP_LABEL2)->SetFont(&mNewFontLabel);

	GetDlgItem(IDC_STATIC_HR)->SetFont(&mNewFont);            //设置心电字体大小
	GetDlgItem(IDC_STATIC_ECG_LABEL1)->SetFont(&mNewFontLabel);
	GetDlgItem(IDC_STATIC_ECG_LABEL2)->SetFont(&mNewFontLabel);

	// 设置字体颜色
	mCtrlParamT1.SetTextColor(mCrTemp);      //设置体温1的颜色为品红色
	mCtrlParamT2.SetTextColor(mCrTemp);

	mCtrlParamSPO2.SetTextColor(mCrSPO2);    //设置血氧字体颜色
	mCtrlParamSpr.SetTextColor(mCrSPO2);

	mCtrlParamNs.SetTextColor(mCrNIBP);      //设置无创压字体颜色
	mCtrlParamNd.SetTextColor(mCrNIBP);
	mCtrlParamNm.SetTextColor(mCrNIBP);
	mCtrlParamNpr.SetTextColor(mCrNIBP);
	mCtrlNIBPCuf.SetTextColor(mCrNIBP);

	mCtrlParamRr.SetTextColor(mCrResp);      //设置呼吸字体颜色

	mCtrlHR.SetTextColor(mCrECG1);           //设置心电字体颜色

	mCtrlWaveECG1.SetBkColor(mCrBackground); //设置心电波形1画图的背景色
	mCtrlWaveECG2.SetBkColor(mCrBackground); //设置心电波形2画图的背景色
	mCtrlWaveSPO2.SetBkColor(mCrBackground); //设置SPO2波形绘图背景颜色 
	mCtrlWaveResp.SetBkColor(mCrBackground); //设置RESP波形绘图背景颜色
}
