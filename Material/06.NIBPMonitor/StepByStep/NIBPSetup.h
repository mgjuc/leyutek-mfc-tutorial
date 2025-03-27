/*********************************************************************************************************
* 模块名称: NIBPSetup.h
* 摘    要: 
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

// CNIBPSetup 对话框

class CNIBPSetup : public CDialogEx
{
	DECLARE_DYNAMIC(CNIBPSetup)

public:
	CNIBPSetup(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CNIBPSetup();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_NIBP_SET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
