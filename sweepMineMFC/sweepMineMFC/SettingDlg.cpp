// SettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "sweepMineMFC.h"
#include "SettingDlg.h"


// CSettingDlg �Ի���

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


// CSettingDlg ��Ϣ�������

void CSettingDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	if (m_cnt > 289 || m_size > 17) {
		return;
	}

	if  (m_cnt > m_size * m_size) {
		CString str;
		str.Format("������%d�����ף�%d����������̫��װ���¡�����", m_size * m_size, m_cnt);
		MessageBox(str);

		return;
	}

	OnOK();
}

void CSettingDlg::OnEnChangeEditSize()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
