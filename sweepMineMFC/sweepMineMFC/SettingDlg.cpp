// SettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "sweepMineMFC.h"
#include "SettingDlg.h"


// CSettingDlg 对话框

IMPLEMENT_DYNAMIC(CSettingDlg, CDialog)

CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingDlg::IDD, pParent),
	m_size(0),
	m_cnt(0)
{

}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SIZE, m_size);
	DDV_MinMaxUInt(pDX, m_size, 1, 17);
	DDX_Text(pDX, IDC_EDIT_CNT, m_cnt);
	DDV_MinMaxUInt(pDX, m_cnt, 0, 289);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSettingDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_SIZE, &CSettingDlg::OnEnChangeEditSize)
END_MESSAGE_MAP()


// CSettingDlg 消息处理程序

void CSettingDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (m_cnt > 289 || m_size > 17) {
		return;
	}

	if  (m_cnt > m_size * m_size) {
		CString str;
		str.Format("雷区：%d，地雷：%d，哈哈，雷太多装不下。。。", m_size * m_size, m_cnt);
		MessageBox(str);

		return;
	}

	OnOK();
}

void CSettingDlg::OnEnChangeEditSize()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
