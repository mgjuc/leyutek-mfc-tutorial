/*********************************************************************************************************
* 模块名称: NIBPSetup.cpp
* 摘    要: NIBPSetup.cpp实现文件
* 当前版本: 1.0.0
* 作    者: Leyutek(COPYRIGHT 2018 - 2021 Leyutek. All rights reserved.)
* 完成日期: 2021年01月22日
* 内    容:
* 注    意:
**********************************************************************************************************
* 取代版本:
* 作    者:
* 完成日期:
* 修改内容:
* 修改文件:
*********************************************************************************************************/

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "pch.h"
#include "ParamMonitor.h"
#include "NIBPSetup.h"
#include "afxdialogex.h"

// CNIBPSetup 对话框

IMPLEMENT_DYNAMIC(CNIBPSetup, CDialogEx)

CNIBPSetup::CNIBPSetup(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_NIBP_SET, pParent)
{

}

CNIBPSetup::~CNIBPSetup()
{
}

void CNIBPSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNIBPSetup, CDialogEx)
END_MESSAGE_MAP()

// CNIBPSetup 消息处理程序

BOOL CNIBPSetup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
