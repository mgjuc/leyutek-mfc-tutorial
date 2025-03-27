/*********************************************************************************************************
* 模块名称: UARTSetup.cpp
* 摘    要: 串口设置
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
#include "ParamMonitor.h"
#include "UARTSetup.h"
#include "afxdialogex.h"
#include "UART.h"
#include "System.h"


// CUARTSetup 对话框
CString arrstrPort[20];

IMPLEMENT_DYNAMIC(CUARTSetup, CDialogEx)

CUARTSetup::CUARTSetup(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_UART_SETUP, pParent)
{

}

CUARTSetup::~CUARTSetup()
{
}

void CUARTSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORT, mCtrlPort);
}


BEGIN_MESSAGE_MAP(CUARTSetup, CDialogEx)
	ON_BN_CLICKED(IDOK, &CUARTSetup::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CUARTSetup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitConfig();
	UARTInitialize(); // 初始

	CComboBox* p;
	// 取得控件的ID，设置控件相关联的成员变量
	p = (CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE);
	p->SetCurSel(mBaudRate);
	p = (CComboBox*)GetDlgItem(IDC_COMBO_PARITY);
	p->SetCurSel(mParity);

	CString postStr;

	mCtrlPort.ResetContent(); // 清空串口号commbobox里面的内容
	// 搜索串口号
	EnumUARTPort();

	postStr.Format("COM%d", mPort);
	mCtrlPort.SelectString(0, (LPCTSTR)postStr); // 搜索串口字符串

	if (mCtrlPort.GetCount() > 1) // 如果当前有端口，就将第一个端口显示在combobox上
	{
		mCtrlPort.SetCurSel(0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

/*********************************************************************************************************
* 函数名称: UARTInitialize
* 函数功能:	初始化串口
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年02月03日
* 注    意:
*********************************************************************************************************/
void CUARTSetup::UARTInitialize()
{
	// 将默认设置显示到相应的框
	mPort = gUARTConfig.nPort;
	mBaudRate = gUARTConfig.nBaudRate;
	mStopBit = gUARTConfig.nStopBit;
	mParity = gUARTConfig.nParity;
	mDataBit = gUARTConfig.nDataBit;
}

/*********************************************************************************************************
* 函数名称: EnumUARTPort
* 函数功能:	查询电脑中的注册表来得到串口
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年02月03日
* 注    意:
*********************************************************************************************************/
void CUARTSetup::EnumUARTPort()
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
	if (lRet0 != ERROR_SUCCESS)
	{
		AfxMessageBox(_T("错误：无法打开有关的hKEY"));
		return;
	}

	DWORD dwIndex = 0; // 注册表的键值
	CString StrPort;

	while (1)
	{
		LONG lStatus;
		TCHAR szName[256] = { 0 };
		UCHAR szPortName[80] = { 0 };
		DWORD dwName;
		DWORD dwSizeofPortName;
		DWORD dwType;

		dwName = sizeof(szName);
		dwSizeofPortName = sizeof(szPortName);
		// 用来枚举指定项的值
		lStatus = RegEnumValue(hKey, dwIndex++, szName, &dwName, NULL, &dwType,
			szPortName, &dwSizeofPortName);
		if ((lStatus == ERROR_SUCCESS) || (lStatus == ERROR_MORE_DATA))
		{
			CString str;
			str = szPortName;
			arrstrPort[dwIndex] = str;
		}
		else
		{
			break;
		}
	}

	// 进行串口号的排序，从小到大排序
	for (int nPortNum = 1; nPortNum <= 255; nPortNum++)
	{
		StrPort.Format("COM%d", nPortNum);
		for (int i = 1; i <= dwIndex; i++) // 遍历有没有字符串相等的，此处的最大值不能是mCtrlPort.GetCount()!!!
		{
			if (strcmp(StrPort, arrstrPort[i]) == 0)
			{
				mCtrlPort.AddString(arrstrPort[i]);
			}
		}
	}
	RegCloseKey(hKey); // 关闭注册表
}

/*********************************************************************************************************
* 函数名称: ReceiveProc
* 函数功能: 串口数据接收回调函数
* 输入参数:
* 输出参数:
* 返 回 值:
* 创建日期: 2021年02月03日
* 注    意:
*********************************************************************************************************/
int CUARTSetup::ReceiveProc(void* pData, DWORD dwLen, void* pThis)   //接收线程函数
{
    if (pThis)
    {
        return DK_SUCCESS;
    }
    else
    {
        return ERR_DK_FAILURE;
    }
}

// CUARTSetup 消息处理程序

/*********************************************************************************************************
* 函数名称: OnBnClickedOk
* 函数功能: 打开串口
* 输入参数:
* 输出参数:
* 返 回 值:
* 创建日期: 2021年02月03日
* 注    意:
*********************************************************************************************************/
void CUARTSetup::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CComboBox* p;
	p = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
	if (p->GetCurSel() >= 0)
	{
		CString PostStr;
		p->GetWindowText(PostStr);  // 得到所选择的title,存放在PostStr
		PostStr = PostStr.Right(PostStr.GetLength() - 3);   // 减去COM
		mPort = _ttoi(PostStr);
	}
	else
	{
		mPort = 1;
	}

	// 获取对应Combo框的关联变量的值，并设置下次设置的默认值
	p = (CComboBox*)GetDlgItem(IDC_COMBO_PARITY);
	mParity = p->GetCurSel();

	p = (CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE);
	mBaudRate = p->GetCurSel();

	CDialogEx::OnOK();
}

