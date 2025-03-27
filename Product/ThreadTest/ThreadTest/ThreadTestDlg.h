/*********************************************************************************************************
* 模块名称: ThreadTestDlg.h
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

/*********************************************************************************************************
*                                                 类的定义
*********************************************************************************************************/
// CThreadTestDlg 对话框
class CThreadTestDlg : public CDialogEx
{
// 构造
public:
	CThreadTestDlg(CWnd* pParent = nullptr);	// 标准构造函数

	static UINT CountThread(void* param);
	void CountStart();
	void CountOver();

	CFont mNewFont;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THREADTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStart();
};
