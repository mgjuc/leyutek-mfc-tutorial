/*********************************************************************************************************
* 模块名称: StoreData.h
* 摘    要: 数据存储全局函数与变量声明
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

// CStoreData 对话框

class CStoreData : public CDialogEx
{
	DECLARE_DYNAMIC(CStoreData)

public:
	CStoreData(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CStoreData();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_STOREDATA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
