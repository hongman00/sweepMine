#pragma once


// CSettingDlg �Ի���

class CSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSettingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SET_MINE_MAP_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	unsigned int m_size;
public:
	unsigned int m_cnt;
public:
	afx_msg void OnEnChangeEditSize();
};
