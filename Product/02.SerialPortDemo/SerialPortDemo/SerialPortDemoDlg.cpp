/*********************************************************************************************************
* 模块名称: SerialPortDemo.cpp
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
#include "SerialPortDemo.h"
#include "SerialPortDemoDlg.h"
#include "afxdialogex.h"
#include "System.h"
#include "UART.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/
CString arrStrPort[20];

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

// CSerialPortDemoDlg 对话框
/*********************************************************************************************************
*                                              成员函数实现
*********************************************************************************************************/
CSerialPortDemoDlg::CSerialPortDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERIALPORTDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialPortDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORT, mCtrlPort);
	DDX_Control(pDX, IDC_LIST_RECEIVE, mCtrlReceiveBox);
}

BEGIN_MESSAGE_MAP(CSerialPortDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_UART, &CSerialPortDemoDlg::OnBnClickedOpenUART)
	ON_BN_CLICKED(IDC_CLOSE_UART, &CSerialPortDemoDlg::OnBnClickedCloseUART)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CSerialPortDemoDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CSerialPortDemoDlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// CSerialPortDemoDlg 消息处理程序

BOOL CSerialPortDemoDlg::OnInitDialog()
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
	UARTInitialize(); // 成员函数的实现在下面的代码中,界面中所有组合框的初始化
	CComboBox* p;

	// 取得控件的ID，设置控件相关联的成员变量
	p = (CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE);
	p->SetCurSel(mBaudRate);
	p = (CComboBox*)GetDlgItem(IDC_COMBO_PARITY);
	p->SetCurSel(mParity);
	p = (CComboBox*)GetDlgItem(IDC_COMBO_DATABITS);
	p->SetCurSel(mDataBits);
	p = (CComboBox*)GetDlgItem(IDC_COMBO_STOPBITS);
	p->SetCurSel(mStopBits);

	//初始化串口号
	CString portStr;

	mCtrlPort.ResetContent(); // 清空串口号ComboBox里面的内容

	// 搜索串口号
	EnumUARTPort();

	portStr.Format(_T("COM%d"), mPort);
	mCtrlPort.SelectString(0, (LPCTSTR)portStr); // 搜索串口字符串

	if(mCtrlPort.GetCount() > 1) // 如果当前有端口，就将第一个端口显示在ComboBox上
	{
		mCtrlPort.SetCurSel(0);
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSerialPortDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSerialPortDemoDlg::OnPaint()
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
HCURSOR CSerialPortDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*********************************************************************************************************
* 函数名称: EnumUARTPort
* 函数功能:	查询计算机中的注册表来得到串口
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CSerialPortDemoDlg::EnumUARTPort()
{
	HKEY hKey; // 注册表，在注册表编辑器中注册表项是用控制键来显示或者编辑的，使得控制键找到和编辑信息项组更容易
	LPCTSTR data_Set = _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"); // 串口在注册表中的路径

	//打开一个指定的注册表键
	//参数一：需要打开的主键的名称，HKEY_LOCAL_MACHINE-保存了所有与计算机有关的配置信息
	//参数二：需要打开的子键的名称
	//参数三：保留，设为0
	//参数四：安全访问标记，也就是权限
	//参数五：得到的将要打开键的句柄
	LONG lRet0 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_Set, 0, KEY_READ, &hKey);
	if(lRet0 != ERROR_SUCCESS)
	{
		AfxMessageBox(_T("错误：无法打开有关的hKEY"));
		return;
	}

	DWORD dwIndex = 0; // 注册表的键值
	CString strPort;

	while(1)
	{
		LONG lStatus;
		TCHAR szName[256] = { 0 };
		TCHAR szPortName[20] = { 0 };
		DWORD dwName;
		DWORD dwSizeofPortName;
		DWORD dwType;

		dwName = sizeof(szName);
		dwSizeofPortName = sizeof(szPortName);
		// 用来枚举指定项的值
		lStatus = RegEnumValue(hKey, dwIndex++, szName, &dwName, NULL, &dwType,
			(LPBYTE)szPortName, &dwSizeofPortName);
		if((lStatus == ERROR_SUCCESS) || (lStatus == ERROR_MORE_DATA))
		{
			CString str;
			str = szPortName;
			arrStrPort[dwIndex] = str;
		}
		else
		{
			break;
		}
	}

	// 进行串口号的排序，从小到大排序
	for(int portNum = 1; portNum <= 255; portNum++)
	{
		strPort.Format(_T("COM%d"), portNum);
		for(int i = 1; i <= dwIndex; i++) // 遍历有没有字符串相等的，此处的最大值不能是mCtrlPort.GetCount()!!!
		{
			if(lstrcmp(strPort, arrStrPort[i]) == 0)
			{
				mCtrlPort.AddString(arrStrPort[i]);
			}
		}
	}
	RegCloseKey(hKey); // 关闭注册表
}

/*********************************************************************************************************
* 函数名称: ReceiveProc
* 函数功能:	由于在关闭程序时，窗口（dialog和它的控件）先于CUART对象结构函数前关闭，所以此时
*           接收线程仍然在工作
* 输入参数: data-接收的数据地址，dwLen-接收数据的长度，pThis-当前的指针
* 输出参数:	void
* 返 回 值: true-合法，false-非法
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
int CSerialPortDemoDlg::ReceiveProc(void* data, DWORD dwLen, void* pThis)
{
	CString str, strTmp;
	if(pThis)
	{
		uchar* p = (uchar*)data; // 先转化为uchar类型，以便在后面转化数据类型做准备
		for(int i = 0; i < dwLen; i++)
		{
			uchar tmp = *p++; // 指针+1后读数据
			strTmp.Format(_T("%x "), tmp); // 将ntmp转化为CString类型,以16进制数显示
			str = str + strTmp;
		}

		// 把一次收到的数据放到列表框中
		((CSerialPortDemoDlg*)pThis)->mCtrlReceiveBox.AddString(str);
		return DK_SUCCESS;
	}
	else
	{
		return ERR_DK_FAILURE;
	}
}

/*********************************************************************************************************
* 函数名称: UARTInitialize
* 函数功能:	初始化串口
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CSerialPortDemoDlg::UARTInitialize()
{
	InitConfig();
	mUART.OpenPort(gUARTConfig.port);
	mUART.ConfigPort(gUARTConfig.baudRate, gUARTConfig.parity, \
		gUARTConfig.dataBits, gUARTConfig.stopBits);

	// 将默认设置显示到对应的下拉框
	mPort = gUARTConfig.port;
	mBaudRate = gUARTConfig.baudRate;
	mStopBits = gUARTConfig.stopBits;
	mParity = gUARTConfig.parity;
	mDataBits = gUARTConfig.dataBits;
}

/*********************************************************************************************************
* 函数名称: OnBnClickedOpenUART
* 函数功能:	打开串口
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CSerialPortDemoDlg::OnBnClickedOpenUART()
{
	// TODO: 在此添加控件通知处理程序代码
	CComboBox* p;
	p = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
	if(p->GetCurSel() >= 0)
	{
		CString portStr;
		p->GetWindowText(portStr); // 得到所选择的title,存放在portStr
		portStr = portStr.Right(portStr.GetLength() - 3); // 去掉COM字符
		mPort = _ttoi(portStr); // CString转int
	}
	else
	{
		mPort = 1;
	}

	// 获取对应ComboBox框的关联变量的值，并设置下次设置的默认值
	//因为语言问题，导到Combo里data设置的中文会乱码 https://www.cnblogs.com/stronghorse/p/16096271.html
	//WIN11关闭语言调级选项以unicode显示其它语言，能显示正常
	p = (CComboBox*)GetDlgItem(IDC_COMBO_PARITY);
	mParity = p->GetCurSel();

	p = (CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE);
	mBaudRate = p->GetCurSel();

	p = (CComboBox*)GetDlgItem(IDC_COMBO_DATABITS);
	mDataBits = p->GetCurSel();

	p = (CComboBox*)GetDlgItem(IDC_COMBO_STOPBITS);
	mStopBits = p->GetCurSel();

	if(!mUART.IsOpen())
	{
		mUART.OpenPort(mPort);
		mUART.RegisterCallFunc(ReceiveProc, this); // 调用接收线程
		mUART.ConfigPort(mBaudRate, mParity, mDataBits, mStopBits);
	}
}

/*********************************************************************************************************
* 函数名称: OnBnClickedCloseUART
* 函数功能:	关闭串口
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CSerialPortDemoDlg::OnBnClickedCloseUART()
{
	// TODO: 在此添加控件通知处理程序代码
	if(mUART.IsOpen()) // 判断串口是否打开了
	{
		mUART.ClosePort();
	}
}

/*********************************************************************************************************
* 函数名称: OnBnClickedButtonClear
* 函数功能:	清空接收数据
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CSerialPortDemoDlg::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
	mCtrlReceiveBox.ResetContent(); // 清空内容
}

/*********************************************************************************************************
* 函数名称: OnBnClickedButtonSend
* 函数功能:	下发串口命令
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CSerialPortDemoDlg::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	CStringArray strArr;
	int len;
	uchar tmp;

	GetDlgItem(IDC_EDIT_SEND)->GetWindowText(str); // 得到发送框的字符串
	SplitString(str, ' ', strArr); // 将一串字符按空格分开
	len = strArr.GetSize(); // 得到分开字符串的个数
	uchar* data = new uchar[len]; // 动态分配空间，记得用完后要delete释放空间

	for(int i = 0; i < len; i++)
	{
		//CT2A宏： Change Text to ASCII
		tmp = (uchar)(strtol(CT2A(strArr.GetAt(i)), NULL, 16)); // 将字符串转化为16进制的数
		*(data + i) = tmp;
	}

	mUART.SendData(data, len); // 发送一串数据

	delete[]data; // 释放内存
}
