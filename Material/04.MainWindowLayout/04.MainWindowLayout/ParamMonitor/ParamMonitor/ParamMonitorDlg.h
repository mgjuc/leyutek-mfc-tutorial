/*********************************************************************************************************
* 模块名称: ParamMonitorDlg.h
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

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "ColorStatic.h"

// CParamMonitorDlg 对话框
class CParamMonitorDlg : public CDialogEx
{
// 构造
public:
	CParamMonitorDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARAM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	//生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CColorStatic mCtrlWaveECG1;
	CColorStatic mCtrlWaveECG2;
	CColorStatic mCtrlWaveSPO2;
	CColorStatic mCtrlWaveResp;

	CColorStatic mCtrlParamRr;
	CString mStrParamRr;

	CColorStatic mCtrlParamT1;
	CString mStrParamT1;
	CColorStatic mCtrlParamT2;
	CString mStrParamT2;
	CString mStrTempSensor1;
	CString mStrTempSensor2;

	CColorStatic mCtrlNIBPCuf;
	CString mStrNIBPCuf;
	CColorStatic mCtrlParamNm;
	CString mStrParamNm;
	CColorStatic mCtrlParamNs;
	CString mStrParamNs;
	CColorStatic mCtrlParamNd;
	CString mStrParamNd;
	CColorStatic mCtrlParamNpr;
	CString mStrParamNPr;

	CColorStatic mCtrlParamSPO2;
	CString mStrParamSPO2;
	CColorStatic mCtrlParamSpr;
	CString mStrParamSPr;
	CString mStrSPO2Sensor;
	CString mStrSPO2Finger;

	CColorStatic mCtrlRA;
	CColorStatic mCtrlLA;
	CColorStatic mCtrlLL;
	CColorStatic mCtrlV;
	CColorStatic mCtrlHR;
	CString mStrHR;
};
