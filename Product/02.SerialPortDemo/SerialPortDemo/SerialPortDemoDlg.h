/*********************************************************************************************************
* 模块名称: SerialPortDemoDlg.h
* 使用说明:
* 摘    要: 类头文件
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

/*********************************************************************************************************
*                                                包含头文件
*********************************************************************************************************/
#include "UART.h"

/*********************************************************************************************************
*                                                 类的定义
*********************************************************************************************************/
// CSerialPortDemoDlg对话框
class CSerialPortDemoDlg : public CDialogEx
{
// 构造
public:
	CSerialPortDemoDlg(CWnd* pParent = nullptr); // 标准构造函数

	CUART mUART;

	// inline内置成员函数，提高效率，inline可以省略
	int SetPort(int port) { mPort = port;         return true; };
	int SetParity(int parity) { mParity = parity;     return true; };
	int SetBaudRate(int baudRate) { mBaudRate = baudRate; return true; };
	int SetDataBits(int dataBits) { mDataBits = dataBits;   return true; };
	int SetStopBits(int stopBits) { mStopBits = stopBits;   return true; };

	int GetPort(void) { return mPort; };
	int GetParity(void) { return mParity; };
	int GetBaudRate(void) { return mBaudRate; };
	int GetDataBits(void) { return mDataBits; };
	int GetStopBits(void) { return mStopBits; };

	void EnumUARTPort();   // 扫描串口函数
	void UARTInitialize(); // 串口初始化函数

	static int ReceiveProc(void* data, DWORD dwLen, void* pThis); // 接收线程函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERIALPORTDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	int mPort;     // 串口号
	int mParity;   // 校验位
	int mBaudRate; // 波特率
	int mDataBits;  // 数据位
	int mStopBits;  // 停止位

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//绑定了串口号的ComboBox控件
	CComboBox mCtrlPort;
	CListBox mCtrlReceiveBox;
	afx_msg void OnBnClickedOpenUART();
	afx_msg void OnBnClickedCloseUART();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnCbnSelchangeComboBaudrate2();
};
