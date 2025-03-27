/*********************************************************************************************************
* 模块名称: ColorStatic.h
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
#include <afxwin.h>
#include "Color.h"

class CColorStatic :
    public CStatic
{
public:
    CColorStatic();
    virtual ~CColorStatic();

	void SetTextColor(COLORREF crColor); // 用于设置文本的颜色。
	void SetBkColor(COLORREF crColor);   // 用于设置文本的BackGround颜色。

	void SetTextFont(LOGFONT* lf);
	void VCenterText();
	void HCenterText();

	void SetColor(COLORREF color);

protected:
	CFont mFont;          // 用户定义的字体
	bool mUseSysFont;     // 判断是否使用默认字体。
	bool mHCentered;      // 判断文本是否应以H为中心。
	bool mVCentered;      // 判断文本是否应以V为中心。

	CBrush   mBkgnd;      // Holds Brush Color for the Static Text
	COLORREF mBkColor;    // Holds the Background Color for the Text
	COLORREF mTextColor;  // Holds the Color for the Text

	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()

};

