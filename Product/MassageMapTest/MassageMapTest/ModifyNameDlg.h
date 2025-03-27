#pragma once


// CModifyNameDlg 对话框

class CModifyNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModifyNameDlg)

public:
	CModifyNameDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CModifyNameDlg();

	CString ReturnNewName();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODIFY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString mEditNewName;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
