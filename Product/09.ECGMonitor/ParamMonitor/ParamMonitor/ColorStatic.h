/*********************************************************************************************************
* ģ������: ColorStatic.h
* ժ    Ҫ: 
* ��ǰ�汾: 1.0.0
* ��    ��: Leyutek(COPYRIGHT 2018 - 2021 Leyutek. All rights reserved.)
* �������: 2021��01��22��
* ��    ��:
* ע    ��: none
**********************************************************************************************************
* ȡ���汾:
* ��    ��:
* �������:
* �޸�����:
* �޸��ļ�:
*********************************************************************************************************/
#pragma once

/*********************************************************************************************************
*                                              ����ͷ�ļ�
*********************************************************************************************************/
#include <afxwin.h>
#include "Color.h"

class CColorStatic :
    public CStatic
{
public:
    CColorStatic();
    virtual ~CColorStatic();

	void SetTextColor(COLORREF crColor); // ���������ı�����ɫ��
	void SetBkColor(COLORREF crColor);   // ���������ı���BackGround��ɫ��

	void SetTextFont(LOGFONT* lf);
	void VCenterText();
	void HCenterText();

	void SetColor(COLORREF color);

protected:
	CFont mFont;          // �û����������
	bool mUseSysFont;     // �ж��Ƿ�ʹ��Ĭ�����塣
	bool mHCentered;      // �ж��ı��Ƿ�Ӧ��HΪ���ġ�
	bool mVCentered;      // �ж��ı��Ƿ�Ӧ��VΪ���ġ�

	CBrush   mBkgnd;      // Holds Brush Color for the Static Text
	COLORREF mBkColor;    // Holds the Background Color for the Text
	COLORREF mTextColor;  // Holds the Color for the Text

	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()

};

