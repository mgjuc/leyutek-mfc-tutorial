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
#include "UART.h"
#include "ColorStatic.h"
#include "PackUnpack.h"

// CParamMonitorDlg 对话框
class CParamMonitorDlg : public CDialogEx
{
// 构造
public:
	CParamMonitorDlg(CWnd* pParent = nullptr);	// 标准构造函数

	CPackUnpack mPackUnpack;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARAM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;
	UINT  mTimerID;       //调用timeSetEvent返回的ID值

	CFont mNewFont;
	CFont mNewFontLabel;
	COLORREF mCrNIBP;
	COLORREF mCrTemp;
	COLORREF mCrSPO2;
	COLORREF mCrResp;
	COLORREF mCrECG1;
	COLORREF mCrECG2;
	COLORREF mCrBackground;
	void Initialize(void);  //初始化

	//SPO2
	int  mSPO2WaveX;        //X坐标
	int  mSPO2WaveY;        //Y坐标
	RECT mSPO2Rect;         //在该矩形区域内画图
	int  mSPO2RectWidth;    //矩形区域宽度
	int  mSPO2RectHeight;   //矩形区域高度
	CPen mSPO2DrawPen;      //SPO2波形画笔
	CPen mSPO2ErasePen;     //SPO2擦除画笔  
	HDC  mSPO2DC;           //设备描述表句柄

	bool  mDrawWaveInit;    //波形初始化标志位

	void InitWave(void);          //波形初始化
	void CloseWave(void);         //关闭波形

	void ChangeUARTPort(void);    //串口号有改变，重新设置
	//显示参数
	void DisplayParams(void);
	//NIBP显示函数
	void DisplayCuffPressure();

	void CreateMediaTimer(void);  //产生、删除多媒体时钟
	void DeleteMediaTimer(void);

	//生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnSecondTimer(WPARAM, LPARAM);  //手动添加
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
	afx_msg void OnUARTSet();
	afx_msg void OnDestroy();
	afx_msg void OnStnClickedStaticNIBP();

	afx_msg LRESULT DrawSPO2Wave(WPARAM, LPARAM);
};
