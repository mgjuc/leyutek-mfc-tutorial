﻿/*********************************************************************************************************
* 模块名称: ProData.h
* 使用说明:
* 摘    要: 应用程序的主头文件
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

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "resource.h"		// 主符号

/*********************************************************************************************************
*                                               类的定义
*********************************************************************************************************/
// CProDataApp:
// 有关此类的实现，请参阅 ProData.cpp
//

class CProDataApp : public CWinApp
{
public:
	CProDataApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CProDataApp theApp;
