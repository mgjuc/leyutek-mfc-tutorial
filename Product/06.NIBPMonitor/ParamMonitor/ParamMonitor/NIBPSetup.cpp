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
	DDX_Control(pDX, IDC_COMBO_MODE, mCtrlMode);
}


BEGIN_MESSAGE_MAP(CNIBPSetup, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNIBPSetup::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CNIBPSetup::OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CNIBPSetup 消息处理程序

BOOL CNIBPSetup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	mCtrlMode.SetCurSel(0); // 设置下拉默认值为首个参数

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


/*********************************************************************************************************
* 函数名称: OnBnClickedOk
* 函数功能: 开始测量按钮单击响应函数
* 输入参数: void
* 输出参数: void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CNIBPSetup::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

/*********************************************************************************************************
* 函数名称: OnBnClickedCancel
* 函数功能: 停止测量按钮单击响应函数
* 输入参数: void
* 输出参数: void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CNIBPSetup::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

/*********************************************************************************************************
* 函数名称: OnClose
* 函数功能: 关闭窗口
* 输入参数: void
* 输出参数: void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CNIBPSetup::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int ret = 6;
	EndDialog(ret);
}
