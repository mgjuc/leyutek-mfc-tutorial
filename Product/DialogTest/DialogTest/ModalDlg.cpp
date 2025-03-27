// ModalDlg.cpp: 实现文件
//

#include "pch.h"
#include "DialogTest.h"
#include "ModalDlg.h"
#include "afxdialogex.h"


// CModalDlg 对话框

IMPLEMENT_DYNAMIC(CModalDlg, CDialogEx)

CModalDlg::CModalDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MODELESS_DIALOG, pParent)
{

}

CModalDlg::~CModalDlg()
{
}

void CModalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CModalDlg, CDialogEx)
END_MESSAGE_MAP()


// CModalDlg 消息处理程序
