// MainFrm.h : CMainFrame ��Ľӿ�
//


#pragma once

#include "sweepmine.h"	
#include "SettingDlg.h"

class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGradePrimary();
public:
	afx_msg void OnGradeDedium();
public:
	afx_msg void OnGradeSenior();
	afx_msg void OnSetByYourself();

private:
	mineBlockSize m_mineBlockSize;
	Location m_location;
	CSettingDlg m_setMineMapDlg;
	UINT16 m_minMineMapWidth;

private:
	void SetMineInfo(UINT8 mineSize, UINT16 mineCnt, UINT16 grade);
	void SetSubMenuGray(UINT8 type);
	void CustomMineMap();
};


