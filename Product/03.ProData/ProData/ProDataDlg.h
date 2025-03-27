/*********************************************************************************************************
* 模块名称: ProDataDlg.h
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
#include <vector>

/*********************************************************************************************************
*                                                  宏定义
*********************************************************************************************************/
#define BLACK      RGB(0, 0, 0)
#define ERASE      RGB(240, 240, 240)
#define ECGMAXVALUE 4096
#define NIDDRAW    100

/*********************************************************************************************************
*                                                 类的定义
*********************************************************************************************************/
// CProDataDlg 对话框
class CProDataDlg : public CDialogEx
{
// 构造
public:
	CProDataDlg(CWnd* pParent = nullptr); // 标准构造函数

	std::vector<int> mFileData; // 存储从文件读取到的数据
	int  mECGWaveX; // X坐标
	int  mECGWaveY; // Y坐标
	RECT mECGRect; // 在该矩形区域内画图
	int  mECGRectWidth; // 矩形区域宽度
	int  mECGRectHeight; // 矩形区域高度
	CPen mECGDrawPen; // ECG波形画笔
	CPen mECGErasePen; // ECG擦除画笔  
	HDC  mECGDC; // 设备描述表句柄
	COLORREF mECGWaveColor; // 波形线条颜色
	COLORREF mECGEraseColor; // 擦除区域背景色
	bool mDrawWaveInit; // 波形初始化标志位

	void InitWave(void); // 波形初始化
	void CloseWave(void); // 关闭波形
	void DrawECGWave(); // 画静态图
	void DrawDynamicECGWave(); // 画动态图

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRODATA_DIALOG };
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
	CStatic mCtrlWave;
	CEdit mCtrlEditDisp;
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonStore();
	afx_msg void OnBnClickedRadioDynamic();
	afx_msg void OnBnClickedRadioStatic();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL DestroyWindow();
};

