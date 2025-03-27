// ModelessDlg.cpp: 实现文件
//

#include "pch.h"
#include "DialogTest.h"
#include "ModelessDlg.h"
#include "afxdialogex.h"

// CModelessDlg 对话框

IMPLEMENT_DYNAMIC(CModelessDlg, CDialogEx)

CModelessDlg::CModelessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MODELESS_DIALOG, pParent)
{

}

CModelessDlg::~CModelessDlg()
{
}

void CModelessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CModelessDlg, CDialogEx)
END_MESSAGE_MAP()


// CModelessDlg 消息处理程序
