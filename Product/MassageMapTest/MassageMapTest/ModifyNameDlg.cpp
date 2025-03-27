// ModifyNameDlg.cpp: 实现文件
//

#include "pch.h"
#include "MassageMapTest.h"
#include "ModifyNameDlg.h"
#include "afxdialogex.h"


// CModifyNameDlg 对话框

IMPLEMENT_DYNAMIC(CModifyNameDlg, CDialogEx)

CModifyNameDlg::CModifyNameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MODIFY_DIALOG, pParent)
	, mEditNewName(_T(""))
{

}

CModifyNameDlg::~CModifyNameDlg()
{
}

void CModifyNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NEWNAME, mEditNewName);
}


BEGIN_MESSAGE_MAP(CModifyNameDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CModifyNameDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CModifyNameDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CModifyNameDlg 消息处理程序

/*********************************************************************************************************
* 函数名称: OnBnClickedOk
* 函数功能:	确定按钮按下响应函数
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CModifyNameDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CDialogEx::OnOK();
}

/*********************************************************************************************************
* 函数名称: OnBnClickedCancel
* 函数功能:	取消按钮按下响应函数
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CModifyNameDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

/*********************************************************************************************************
* 函数名称: ReturnNewName
* 函数功能:	返回编辑后的姓名
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
CString CModifyNameDlg::ReturnNewName()
{
	return mEditNewName;
}