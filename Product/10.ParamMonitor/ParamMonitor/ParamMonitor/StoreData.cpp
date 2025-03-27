/*********************************************************************************************************
* 模块名称: StoreData.cpp
* 摘    要: 数据存储
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

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "pch.h"
#include "ParamMonitor.h"
#include "StoreData.h"
#include "afxdialogex.h"
#include "PackUnpack.h"

// CStoreData 对话框

IMPLEMENT_DYNAMIC(CStoreData, CDialogEx)

CStoreData::CStoreData(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_STOREDATA, pParent)
{

}

CStoreData::~CStoreData()
{
}

void CStoreData::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStoreData, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CStoreData::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDOK, &CStoreData::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CStoreData::OnBnClickedCancel)
END_MESSAGE_MAP()

// CStoreData 消息处理程序

BOOL CStoreData::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (!gSaveDir.IsEmpty())
	{
		CWnd* pWnd;
		pWnd = GetDlgItem(IDC_EDIT_SAVE_DIR);
		pWnd->SetWindowText(gSaveDir); // 显示文件路径
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

/*********************************************************************************************************
* 函数名称: OnBnClickedButtonOpen
* 函数功能: 打开按钮监听函数
* 输入参数:
* 输出参数:
* 返 回 值:
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CStoreData::OnBnClickedButtonOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd* pWnd;
	char szFilters[] =
		"CSV file(*.csv)\0*.csv\0"\
		"C++ source file(*.h;*,hpp;*.cpp)\0*.h;*.hpp;*.cpp\0"\
		"Text File(*.txt)\0*.txt\0"\
		"All Typle(*.*)\0*.*\0" \
		"Lua source file(*.lua)\0*.lua\0"\
		"\0";

	CFileDialog fileDlg(FALSE, "csv", _T("Test"));
	fileDlg.m_ofn.lpstrTitle = "Save File";
	fileDlg.m_ofn.lpstrFilter = szFilters;
	if (IDOK == fileDlg.DoModal())
	{
		pWnd = GetDlgItem(IDC_EDIT_SAVE_DIR);
		pWnd->SetWindowText(fileDlg.GetPathName()); // 显示文件路径
		gSaveDir = fileDlg.GetPathName();

	}
}

/*********************************************************************************************************
* 函数名称: OnBnClickedOk
* 函数功能: 确定按钮监听函数
* 输入参数:
* 输出参数:
* 返 回 值:
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CStoreData::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	gSaveFlag = true;
	CDialogEx::OnOK();
}

/*********************************************************************************************************
* 函数名称: OnBnClickedCancel
* 函数功能: 取消按钮监听函数
* 输入参数:
* 输出参数:
* 返 回 值:
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CStoreData::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
