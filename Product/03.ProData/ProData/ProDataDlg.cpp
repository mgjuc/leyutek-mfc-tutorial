/*********************************************************************************************************
* 模块名称: ProDataDlg.cpp
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
#include "ProData.h"
#include "ProDataDlg.h"
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


// CProDataDlg 对话框
/*********************************************************************************************************
*                                              成员函数实现
*********************************************************************************************************/
CProDataDlg::CProDataDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PRODATA_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GRAPHICS, mCtrlWave);
	DDX_Control(pDX, IDC_EDIT_DISP, mCtrlEditDisp);
}

BEGIN_MESSAGE_MAP(CProDataDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CProDataDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_STORE, &CProDataDlg::OnBnClickedButtonStore)
	ON_BN_CLICKED(IDC_RADIO_DYNAMIC, &CProDataDlg::OnBnClickedRadioDynamic)
	ON_BN_CLICKED(IDC_RADIO_STATIC, &CProDataDlg::OnBnClickedRadioStatic)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CProDataDlg 消息处理程序

BOOL CProDataDlg::OnInitDialog()
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
	mECGEraseColor = ERASE; // 擦除画笔颜色，与窗体颜色一致
	mECGWaveColor = BLACK;  // 画波形颜色，黑色
	InitWave();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CProDataDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CProDataDlg::OnPaint()
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
HCURSOR CProDataDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*********************************************************************************************************
* 函数名称: InitWave
* 函数功能: 波形绘制前的初始化工作
* 输入参数: void
* 输出参数: 
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CProDataDlg::InitWave()
{
	CRect Rect;
	mCtrlWave.GetClientRect(Rect);
	mECGRect = *(LPCRECT)Rect;

	mECGRectWidth = Rect.Width();
	mECGRectHeight = Rect.Height();

	// 设置初始画图位置
	mECGWaveX = 0;
	mECGWaveY = 0;

	// 创建画笔（波形画笔、擦除画笔）
	mECGDrawPen.CreatePen(PS_SOLID, 1, mECGWaveColor);
	mECGErasePen.CreatePen(PS_SOLID, 1, mECGEraseColor);

	mECGDC = ::GetDC(mCtrlWave.m_hWnd); // 获取心电波形句柄
	mDrawWaveInit = true; // 设置初始化完成标志位
}

/*********************************************************************************************************
* 函数名称: CloseWave
* 函数功能: 关闭波形显示，释放DC
* 输入参数: void
* 输出参数:
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CProDataDlg::CloseWave(void)
{
	mDrawWaveInit = false;
	::ReleaseDC(mCtrlWave.m_hWnd, mECGDC); // 释放DC
}


/*********************************************************************************************************
* 函数名称: DrawECGWave
* 函数功能: 静态波形显示
* 输入参数: void
* 输出参数:
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CProDataDlg::DrawECGWave()
{
	if(mDrawWaveInit == FALSE || mFileData.size() == 0)
	{
		return;
	}

	CDC* pDC = CDC::FromHandle(mECGDC); // 创建并附加一个临时的CDC对象

	// 整个界面先擦除一遍
	pDC->SelectObject(&mECGErasePen); // SelectObject选择擦除画笔对象
	for(int i = mECGRect.left; i < mECGRect.right; i++)
	{
		pDC->MoveTo(i, mECGRect.top);
		pDC->LineTo(i, mECGRect.bottom);      // 从上往下擦除
	}

	// 画波形
	pDC->SelectObject(&mECGDrawPen);        // SelectObject选择波形画笔对象
	pDC->MoveTo(mECGWaveX, mECGWaveY);  // 画线起始点设置

	for(int i = 0; i < mFileData.size(); i++)
	{
		mECGWaveX++;
		mECGWaveY = mECGRect.bottom - (mFileData[mECGWaveX * 3] - 1500) * mECGRectHeight / (ECGMAXVALUE / 2);
		pDC->LineTo(mECGWaveX, mECGWaveY); // 画线
		if(mECGWaveX >= mECGRect.right)
		{
			break; // 超出x轴右边则停止画
		}
	}
}

/*********************************************************************************************************
* 函数名称: DrawDynamicECGWave
* 函数功能: 动态波形显示
* 输入参数: void
* 输出参数:
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CProDataDlg::DrawDynamicECGWave()
{
	if(mDrawWaveInit == FALSE || mFileData.size() == 0)
	{
		return;
	}

	CDC* pDC = CDC::FromHandle(mECGDC); // 创建并附加一个临时的CDC对象
	pDC->SelectObject(&mECGDrawPen); // SelectObject选择画笔对象
	pDC->MoveTo(mECGWaveX, mECGWaveY); // 画线起始点设置

	mECGWaveX++;
	if(mECGWaveX * 3 >= mFileData.size()) // 根据不同波形数据调整取点间隔
	{
		mECGWaveY = mECGRect.bottom;
	}
	else
	{
		mECGWaveY = mECGRect.bottom - (mFileData[mECGWaveX * 3] - 1500) * mECGRectHeight / (ECGMAXVALUE / 2);
	}
	pDC->LineTo(mECGWaveX, mECGWaveY); // 画线

	// 擦除波形
	pDC->SelectObject(&mECGErasePen);
	int nEraseX = mECGWaveX + 10;
	if(mECGRect.right <= nEraseX) // 擦除X位置超过右边界
	{
		nEraseX = 10 - (mECGRect.right - mECGWaveX); // 擦除最左边
	}
	pDC->MoveTo(nEraseX, mECGRect.top); // 起始点移动该位置
	pDC->LineTo(nEraseX, mECGRect.bottom); // 从上往下擦除

	if(mECGWaveX >= mECGRect.right)
	{
		mECGWaveX = 0; // 超出右边界，再从左边开始
	}
}

/*********************************************************************************************************
* 函数名称: OnBnClickedButtonLoad
* 函数功能: 单击加载数据按钮，将数据加载到IDC_EDIT_DISP框
* 输入参数: void
* 输出参数:
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CProDataDlg::OnBnClickedButtonLoad()
{
	// TODO: 在此添加控件通知处理程序代码
	char szFilters[] =
		"CSV file(*.csv)\0*.csv\0"\
		"C++ source file(*.h;*,hpp;*.cpp)\0*.h;*.hpp;*.cpp\0"\
		"Text File(*.txt)\0*.txt\0"\
		"All Typle(*.*)\0*.*\0" \
		"Lua source file(*.lua)\0*.lua\0"\
		"\0";

	CFileDialog openDlg(TRUE);
	openDlg.m_ofn.lpstrTitle = _T("Open File");
	openDlg.m_ofn.lpstrFilter = (LPCSTR)szFilters;

	if(IDOK == openDlg.DoModal())
	{
		CStdioFile file;

		CFileException e;
		// 构造文件，同时增加异常处理
		if(!file.Open(openDlg.GetPathName(), CFile::modeRead, &e))
		{
			CString strErr;
			strErr.Format(_T("File could not be opened %d\n"), e.m_cause);
			MessageBox(strErr);
			return;
		}

		// 读取文件内容
		CString str = NULL; // 存储每行读取到的字符串
		CString strTotal = NULL; // 把所有字符串拼接起来

		while(file.ReadString(str))
		{
			mFileData.push_back(_ttoi(_T(str))); // 每一行数据存储进vector
			strTotal += str + " ";
		}
		// 显示文件内容
		mCtrlEditDisp.SetWindowTextA(strTotal);

		file.Close();
	}
}

/*********************************************************************************************************
* 函数名称: OnBnClickedButtonStore
* 函数功能: 单击存储数据按钮，将数据保存到文件中
* 输入参数: void
* 输出参数:
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CProDataDlg::OnBnClickedButtonStore()
{
	// TODO: 在此添加控件通知处理程序代码
	char szFilters[] =
		"CSV file(*.csv)\0*.csv\0"\
		"C++ source file(*.h;*,hpp;*.cpp)\0*.h;*.hpp;*.cpp\0"\
		"Text File(*.txt)\0*.txt\0"\
		"All Typle(*.*)\0*.*\0" \
		"Lua source file(*.lua)\0*.lua\0"\
		"\0";

	CFileDialog fileDlg(FALSE, "csv", _T("Test"));
	fileDlg.m_ofn.lpstrTitle = "Save File";
	fileDlg.m_ofn.lpstrFilter = szFilters;

	if(IDOK == fileDlg.DoModal())
	{
		CFile file(fileDlg.GetPathName(), CFile::modeCreate | CFile::modeReadWrite);

		char tmp[8];
		//写入文件内容
		for(int i = 0; i < mFileData.size(); i++)
		{
			_itoa_s(mFileData[i], tmp, 10); // int类型转为字符串
			file.Write(tmp, sizeof(CString) * 1);
			file.Write("\n", sizeof(CString) * 1); // 换行，按列存储 
		}
		//立即写入，不缓冲
		file.Flush();
		//文件操作结束关闭
		file.Close();
	}
}

/*********************************************************************************************************
* 函数名称: OnBnClickedRadioDynamic
* 函数功能: 切换到动态图像显示
* 输入参数: void
* 输出参数:
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CProDataDlg::OnBnClickedRadioDynamic()
{
	// TODO: 在此添加控件通知处理程序代码
	mECGWaveX = 0;// 画图横坐标从0开始
	SetTimer(NIDDRAW, 10, NULL); // 启动ID为NIDDRAW的定时器，定时时间为10毫秒
}

/*********************************************************************************************************
* 函数名称: OnBnClickedRadioStatic
* 函数功能: 切换到静态图像显示
* 输入参数: void
* 输出参数:
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CProDataDlg::OnBnClickedRadioStatic()
{
	// TODO: 在此添加控件通知处理程序代码
	mECGWaveX = 0;
	KillTimer(NIDDRAW); // 关闭定时器
	DrawECGWave(); //调用静态画图函数
}

/*********************************************************************************************************
* 函数名称: OnTimer
* 函数功能: 设置定时器功能
* 输入参数: nIDEvent-定时器ID
* 输出参数:
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CProDataDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DrawDynamicECGWave(); // 调用动态画图函数
	CDialogEx::OnTimer(nIDEvent);
}

/*********************************************************************************************************
* 函数名称: DestroyWindow
* 函数功能: 删除窗口对象
* 输入参数: void
* 输出参数:
* 返 回 值: BOOL
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
BOOL CProDataDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	mFileData.clear(); // 释放存储数据
	KillTimer(NIDDRAW); // 关闭定时器，不能放在析构函数中
	return CDialogEx::DestroyWindow();
}
