// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "sweepMineMFC.h"

#include "MainFrm.h"
#include "sweepMineMFCView.h"
#include "sweepMineMFCDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_GRADE_PRIMARY, &CMainFrame::OnGradePrimary)
	ON_COMMAND(ID_GRADE_DEDIUM, &CMainFrame::OnGradeDedium)
	ON_COMMAND(ID_GRADE_SENIOR, &CMainFrame::OnGradeSenior)
	ON_COMMAND(ID_SET_BY_YOURSELF, &CMainFrame::OnSetByYourself)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	CBitmap bitmap;
	BITMAP bitMapInfo;

	bitmap.LoadBitmap(IDB_BITMAP_PRIMACY_SQUARE);
	bitmap.GetBitmap(&bitMapInfo);

	m_mineBlockSize.w = (UINT8)bitMapInfo.bmWidth;
	m_mineBlockSize.h = (UINT8)bitMapInfo.bmHeight;

	m_minMineMapWidth = 400;

}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
// 	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
// 		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
// 		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
// 	{
// 		TRACE0("未能创建工具栏\n");
// 		return -1;      // 未能创建
// 	}
// 
// 	if (!m_wndStatusBar.Create(this) ||
// 		!m_wndStatusBar.SetIndicators(indicators,
// 		  sizeof(indicators)/sizeof(UINT)))
// 	{
// 		TRACE0("未能创建状态栏\n");
// 		return -1;      // 未能创建
// 	}

	// TODO: 如果不需要工具栏可停靠，则删除这三行
// 	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
// 	EnableDocking(CBRS_ALIGN_ANY);
// 	DockControlBar(&m_wndToolBar);

	SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);

	SetMenu(NULL);

	CMenu m_sweepMineMenu;
	m_sweepMineMenu.LoadMenu(IDR_MENU_SWEEPMINE);
	SetMenu(&m_sweepMineMenu);
	m_sweepMineMenu.Detach();

	m_bAutoMenuEnable = FALSE;

	SetSubMenuGray(SUBMENUGRAY_PRIMACY);

	RECT rect;
	GetClientRect(&rect);

	int x = m_mineBlockSize.w * (GRADE_PRIMACY_MAP_SIZE + 2) + 15;
	int y = m_mineBlockSize.h * (GRADE_PRIMACY_MAP_SIZE + 4) + 15;

	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);

	m_location.x = (UINT16)((cx - (rect.right - rect.left)) / 2);
	m_location.y = (UINT16)((cy - (rect.bottom - rect.top)) / 3);

	SetWindowPos(&wndTop, m_location.x, m_location.y, x, y, SWP_NOACTIVATE);

	SetTimer(1, 1000, NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.cx = 30 * (GRADE_PRIMACY_MAP_SIZE + 2) + 15;
	cs.cy = 30 * (GRADE_PRIMACY_MAP_SIZE + 4) + 15;

	cs.style &= ~FWS_ADDTOTITLE;
	cs.style &= ~WS_THICKFRAME;
	cs.lpszName = _T("sweepMine");

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::SetSubMenuGray(UINT8 type) {
	if (SUBMENUGRAY_ALL == type) {
		GetMenu()->GetSubMenu(0)->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		GetMenu()->GetSubMenu(0)->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		GetMenu()->GetSubMenu(0)->EnableMenuItem(2, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	}
	else if (SUBMENUGRAY_PRIMACY == type) {
		GetMenu()->GetSubMenu(0)->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		GetMenu()->GetSubMenu(0)->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
		GetMenu()->GetSubMenu(0)->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
	}
	else if (SUBMENUGRAY_MEDIUM == type) {
		GetMenu()->GetSubMenu(0)->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
		GetMenu()->GetSubMenu(0)->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		GetMenu()->GetSubMenu(0)->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
	}
	else if (SUBMENUGRAY_SENIOR == type){
		GetMenu()->GetSubMenu(0)->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
		GetMenu()->GetSubMenu(0)->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
		GetMenu()->GetSubMenu(0)->EnableMenuItem(2, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	}
	else if (SUBMENUGRAY_NULL == type) {
		GetMenu()->GetSubMenu(0)->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
		GetMenu()->GetSubMenu(0)->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
		GetMenu()->GetSubMenu(0)->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
	}
}

void CMainFrame::OnGradePrimary()
{
	// TODO: 在此添加命令处理程序代码

	int x = m_mineBlockSize.w * (GRADE_PRIMACY_MAP_SIZE + 2) + 15;
	x = x > m_minMineMapWidth ? x : m_minMineMapWidth;
	int y = m_mineBlockSize.h * (GRADE_PRIMACY_MAP_SIZE + 4) + 15;

	SetSubMenuGray(SUBMENUGRAY_PRIMACY);
	SetWindowPos(&wndTop, m_location.x, m_location.y, x, y, SWP_NOACTIVATE);
	SetMineInfo(GRADE_PRIMACY_MAP_SIZE, GRADE_PRIMACY_MINE_COUNT, IDB_BITMAP_PRIMACY_SQUARE);
}

void CMainFrame::OnGradeDedium()
{
	// TODO: 在此添加命令处理程序代码
	CBitmap bitmap;
	BITMAP bitMapInfo;

	bitmap.LoadBitmap(IDB_BITMAP_MEDIUM_SQUARE);
	bitmap.GetBitmap(&bitMapInfo);

	int x = bitMapInfo.bmWidth * (GRADE_MEDIUM_MAP_SIZE + 2) + 15;
	x = x > m_minMineMapWidth ? x : m_minMineMapWidth;
	int y = bitMapInfo.bmHeight * (GRADE_MEDIUM_MAP_SIZE + 5) + 15;

	SetSubMenuGray(SUBMENUGRAY_MEDIUM);
	SetWindowPos(&wndTop, m_location.x, m_location.y, x, y, SWP_NOACTIVATE);
	SetMineInfo(GRADE_MEDIUM_MAP_SIZE, GRADE_MEDIUM_MINE_COUNT, IDB_BITMAP_MEDIUM_SQUARE);
}

void CMainFrame::OnGradeSenior()
{
	// TODO: 在此添加命令处理程序代码
	CBitmap bitmap;
	BITMAP bitMapInfo;

	bitmap.LoadBitmap(IDB_BITMAP_SENIOR_SQUARE);
	bitmap.GetBitmap(&bitMapInfo);

	int x = bitMapInfo.bmWidth * (GRADE_SENIOR_MAP_SIZE + 2) + 15;
	x = x > m_minMineMapWidth ? x : m_minMineMapWidth;
	int y = bitMapInfo.bmHeight * (GRADE_SENIOR_MAP_SIZE + 6) + 25;

	SetSubMenuGray(SUBMENUGRAY_SENIOR);
	SetWindowPos(&wndTop, m_location.x, m_location.y, x, y, SWP_NOACTIVATE);
	SetMineInfo(GRADE_SENIOR_MAP_SIZE, GRADE_SENIOR_MINE_COUNT, IDB_BITMAP_SENIOR_SQUARE);
}

void CMainFrame::CustomMineMap()
{
	// TODO: 在此添加命令处理程序代码
	CBitmap bitmap;
	BITMAP bitMapInfo;

	bitmap.LoadBitmap(IDB_BITMAP_PRIMACY_SQUARE);
	bitmap.GetBitmap(&bitMapInfo);

	SetSubMenuGray(SUBMENUGRAY_NULL);
	int x = bitMapInfo.bmWidth * (m_setMineMapDlg.m_size + 2) + 15;
	x = x > m_minMineMapWidth ? x : m_minMineMapWidth;
	int y = bitMapInfo.bmHeight * (m_setMineMapDlg.m_size + 4) + 15;

	SetWindowPos(&wndTop, m_location.x, m_location.y, x, y, SWP_NOACTIVATE);
	SetMineInfo(m_setMineMapDlg.m_size, m_setMineMapDlg.m_cnt, IDB_BITMAP_PRIMACY_SQUARE);
}

void CMainFrame::OnSetByYourself()
{
	// TODO: 在此添加命令处理程序代码

	if (IDOK == m_setMineMapDlg.DoModal()) {
		CustomMineMap();
	}
}

void CMainFrame::SetMineInfo(UINT8 mineSize, UINT16 mineCnt, UINT16 grade) 
{
	CsweepMineMFCView *pView = (CsweepMineMFCView *)GetActiveView();

	pView->SetMineMap(mineSize, mineCnt, grade);
}
