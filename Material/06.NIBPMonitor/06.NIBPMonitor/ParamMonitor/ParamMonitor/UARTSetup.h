/*********************************************************************************************************
* 模块名称: UARTSetup.h
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
#pragma once

// CUARTSetup 对话框

class CUARTSetup : public CDialogEx
{
	DECLARE_DYNAMIC(CUARTSetup)

public:
	CUARTSetup(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CUARTSetup();

	// inline内置成员函数，提高效率，inline可以省略
	int SetPort(int port) { mPort = port;         return true; };
	int SetParity(int parity) { mParity = parity;     return true; };
	int SetBaudRate(int baudrate) { mBaudRate = baudrate; return true; };
	int SetDataBit(int databit) { mDataBit = databit;   return true; };
	int SetStopBit(int stopbit) { mStopBit = stopbit;   return true; };

	int GetPort(void) { return mPort; };
	int GetParity(void) { return mParity; };
	int GetBaudRate(void) { return mBaudRate; };
	int GetDataBit(void) { return mDataBit; };
	int GetStopBit(void) { return mStopBit; };

	void EnumUARTPort();   // 扫描串口函数
	void UARTInitialize(); // 串口初始化函数
	static int ReceiveProc(void* pData, DWORD dwLen, void* pThis); // 接收线程函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_UART_SETUP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	int   mPort;     // 串口号
	int   mParity;   // 校验方式
	int   mBaudRate; // 波特率
	int   mDataBit;  // 数据位
	int   mStopBit;  // 停止位

public:
	afx_msg void OnBnClickedOk();
	CComboBox mCtrlPort;
	virtual BOOL OnInitDialog();
};
