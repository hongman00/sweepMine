// sweepMineMFCView.cpp : CsweepMineMFCView 类的实现
//

#include "stdafx.h"
#include "sweepMineMFC.h"

#include "sweepMineMFCDoc.h"
#include "sweepMineMFCView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT8 CsweepMineMFCView::m_calcNumOver = MINE_FALSE;
CFile CsweepMineMFCView::m_debugFile;
debugInfo CsweepMineMFCView::m_debugInfo;
HANDLE CsweepMineMFCView::m_hMutex;

// CsweepMineMFCView

IMPLEMENT_DYNCREATE(CsweepMineMFCView, CView)

BEGIN_MESSAGE_MAP(CsweepMineMFCView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

UINT32 CsweepMineMFCView::m_mineBitmapIdPrimacy[17] = {IDB_BITMAP_PRIMACY_OPEN, IDB_BITMAP_PRIMACY1, IDB_BITMAP_PRIMACY2, IDB_BITMAP_PRIMACY3, IDB_BITMAP_PRIMACY4, IDB_BITMAP_PRIMACY5, IDB_BITMAP_PRIMACY6, IDB_BITMAP_PRIMACY7, IDB_BITMAP_PRIMACY8, IDB_BITMAP_PRIMACY_BOOM, IDB_BITMAP_PRIMACY_BOOMED, IDB_BITMAP_PRIMACY_TRIANGLE, IDB_BITMAP_PRIMACY_QUESTION, IDB_BITMAP_PRIMACY_WIN, IDB_BITMAP_PRIMACY_LOSE, IDB_BITMAP_PRIMACY_PROCESS, IDB_BITMAP_PRIMACY_SQUARE};
UINT32 CsweepMineMFCView::m_mineBitmapIdMedium[17] = {IDB_BITMAP_MEDIUM_OPEN, IDB_BITMAP_MEDIUM1, IDB_BITMAP_MEDIUM2, IDB_BITMAP_MEDIUM3, IDB_BITMAP_MEDIUM4, IDB_BITMAP_MEDIUM5, IDB_BITMAP_MEDIUM6, IDB_BITMAP_MEDIUM7, IDB_BITMAP_MEDIUM8, IDB_BITMAP_MEDIUM_BOOM, IDB_BITMAP_MEDIUM_BOOMED, IDB_BITMAP_MEDIUM_TRIANGLE, IDB_BITMAP_MEDIUM_QUESTION, IDB_BITMAP_MEDIUM_WIN, IDB_BITMAP_MEDIUM_LOSE, IDB_BITMAP_MEDIUM_PROCESS, IDB_BITMAP_MEDIUM_SQUARE};
UINT32 CsweepMineMFCView::m_mineBitmapIdSenior[17] = {IDB_BITMAP_SENIOR_OPEN, IDB_BITMAP_SENIOR1, IDB_BITMAP_SENIOR2, IDB_BITMAP_SENIOR3, IDB_BITMAP_SENIOR4, IDB_BITMAP_SENIOR5, IDB_BITMAP_SENIOR6, IDB_BITMAP_SENIOR7, IDB_BITMAP_SENIOR8, IDB_BITMAP_SENIOR_BOOM, IDB_BITMAP_SENIOR_BOOMED, IDB_BITMAP_SENIOR_TRIANGLE, IDB_BITMAP_SENIOR_QUESTION, IDB_BITMAP_SENIOR_WIN, IDB_BITMAP_SENIOR_LOSE, IDB_BITMAP_SENIOR_PROCESS, IDB_BITMAP_SENIOR_SQUARE};

// CsweepMineMFCView 构造/析构

CsweepMineMFCView::CsweepMineMFCView()
	: m_tagCnt(0)
{
	// TODO: 在此处添加构造代码
	m_mineMapSize = GRADE_PRIMACY_MAP_SIZE;
	m_mineCnt = GRADE_PRIMACY_MINE_COUNT;
	m_bitmapId = m_mineBitmapIdPrimacy;

	m_isFail = MINE_FALSE;
	m_isSuccess = MINE_FALSE;

	m_widthOffset = 0;
	m_heightOffset = 0;
	m_steps = 0;
	m_time = 0;
}

CsweepMineMFCView::~CsweepMineMFCView()
{
	ResetMineMap();
}

BOOL CsweepMineMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CsweepMineMFCView 绘制

void CsweepMineMFCView::OnDraw(CDC* pDC)
{
	CsweepMineMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	ShowMineMap(pDC);
}

void CsweepMineMFCView::ResetMineMap() {
	m_mineList.clear();
	m_mineLocationList.clear();

	m_mineList.reserve(m_mineMapSize * m_mineMapSize);

	m_isFail = MINE_FALSE;
	m_isSuccess = MINE_FALSE;
	m_steps = 0;
	m_time = 0;
	m_tagCnt = 0;

	srand((long)time(NULL));
}

void CsweepMineMFCView::CalcClickPosition(CPoint point, pLocation location) {
	location->x = (UINT16)((point.x - m_widthOffset) / m_bitMapInfo.bmWidth);
	location->y = (UINT16)((point.y - m_heightOffset) / m_bitMapInfo.bmHeight);
}

void CsweepMineMFCView::CalcMineBlockLeftTopPos(pLocation pPos, UINT8 x, UINT8 y) {
	pPos->x = (UINT16)(m_widthOffset + x * m_bitMapInfo.bmWidth);
	pPos->y = (UINT16)(m_heightOffset + y * m_bitMapInfo.bmHeight);
}

void CsweepMineMFCView::SetMineMap(UINT8 mineMapSize, UINT16 mineCnt, UINT16 grade) {
	m_mineCnt = mineCnt;
	m_mineMapSize = mineMapSize;

	if (IDB_BITMAP_PRIMACY_SQUARE == grade) {
		m_bitmapId = m_mineBitmapIdPrimacy;
	} 
	else if (IDB_BITMAP_MEDIUM_SQUARE == grade) {
		m_bitmapId = m_mineBitmapIdMedium;
	}
	else if (IDB_BITMAP_SENIOR_SQUARE == grade) {
		m_bitmapId = m_mineBitmapIdSenior;
	}

	CBitmap bitmap;
	bitmap.LoadBitmap(grade);
	bitmap.GetBitmap(&m_bitMapInfo);

	CRect rect;
	GetClientRect(&rect);
	m_widthOffset = (UINT8)((rect.right - rect.left - m_mineMapSize * m_bitMapInfo.bmWidth) / 2);
	m_heightOffset = (UINT8)(40 + m_bitMapInfo.bmHeight + 5);

	ResetMineMap();

	for (int i = 0; i < m_mineMapSize; i ++) {
		for (int j = 0; j < m_mineMapSize; j ++) {
			pS_MineNode pNode = new S_MineNode;
			pNode->x = j;
			pNode->y = i;
			pNode->isMine = MINE_FALSE;
			pNode->type = MINE_NUM_0;
			pNode->isMineNumCalc = MINE_FALSE;
			pNode->isFlag = MINE_FALSE;
			pNode->isQuestion = MINE_FALSE;
			pNode->isChosen = MINE_FALSE;

			m_mineList.push_back(*pNode);
		}
	}

	SetMine(m_mineCnt);
	CalcMineNum();
	RedrawMineMap();
}

UINT8 CsweepMineMFCView::GetRandNum() {
	return rand() % m_mineMapSize;
}

void CsweepMineMFCView::GetMinePosition(UINT8 *x, UINT8 *y) {
	while (TRUE) {
		int flag = 0;
		*x = GetRandNum();
		*y = GetRandNum();

		for (pm_mineLocation = m_mineLocationList.begin(); pm_mineLocation != m_mineLocationList.end(); pm_mineLocation ++) {
			if (*x == pm_mineLocation->x && *y == pm_mineLocation->y) {
				flag = 1;
				break;
			}
		}

		if (1 != flag) {
			break;
		}
	}

	pmineLocation pNode = new mineLocation;
	pNode->x = *x;
	pNode->y = *y;

	m_mineLocationList.push_back(*pNode);
}

void CsweepMineMFCView::SetMine(UINT16 mineCnt) {
	UINT8 x = 0, y = 0;

	for (int i = 0; i < mineCnt; i ++) {
		GetMinePosition(&x, &y);

		pm_mineNode = m_mineList.begin() + y * m_mineMapSize + x;
		pm_mineNode->isMine = MINE_TRUE;
		pm_mineNode->type = MINE_BOOM;
	}
}

UINT8 CsweepMineMFCView::GetMineNum(UINT8 x, UINT8 y) {
	UINT8 mineCnt = 0;
	UINT8 x_max = 0, x_min = 0, y_max = 0, y_min = 0;
	UINT16 offset = 0;

	CalcMapEdge(x, y, &x_max, &x_min, &y_max, &y_min);
	for (int i = y_min; i <= y_max; i ++) {
		offset += i * m_mineMapSize;
		offset += x_min;
		for (pm_mineNode = m_mineList.begin() + offset; pm_mineNode <= m_mineList.begin() + (offset + (x_max - x_min)); pm_mineNode ++) {
			if (MINE_TRUE == pm_mineNode->isMine) {
				mineCnt ++;
			}
		}
		offset = 0;
	}

	return mineCnt;
}

void CsweepMineMFCView::CalcMineNum() {
	UINT8 x_max = 0, x_min = 0, y_max = 0, y_min = 0;

	for (pm_mineLocation = m_mineLocationList.begin(); pm_mineLocation != m_mineLocationList.end(); pm_mineLocation ++) {
		CalcMapEdge(pm_mineLocation->x, pm_mineLocation->y, &x_max, &x_min, &y_max, &y_min);
		for (int i = y_min; i <= y_max; i ++) {
			for (int j = x_min; j <= x_max; j ++) {
				pm_mineNodeTemp = m_mineList.begin() + i * m_mineMapSize + j;
				if ((j == pm_mineLocation->x && i == pm_mineLocation->y) || (MINE_TRUE == pm_mineNodeTemp->isMineNumCalc) || (MINE_TRUE == pm_mineNodeTemp->isMine)) {
 					continue;
				}
				pm_mineNodeTemp->type = GetMineNum(j, i);
				pm_mineNodeTemp->isMineNumCalc = MINE_TRUE;
			}
		}
	}
}

void CsweepMineMFCView::CalcMapEdge(UINT8 x, UINT8 y, UINT8 *x_max, UINT8 *x_min, UINT8 *y_max, UINT8 *y_min) {
	*x_max = x + 1;
	*x_min = x - 1;
	*y_max = y + 1;
	*y_min = y - 1;

	if (0 == x) {
		*x_min = 0;
	}

	if (m_mineMapSize - 1 == x) {
		*x_max = m_mineMapSize - 1;
	}

	if (0 == y) {
		*y_min = 0;
	}

	if (m_mineMapSize - 1 == y) {
		*y_max = m_mineMapSize - 1;
	}
}

void CsweepMineMFCView::ShowExpression(CDC *pDC) {
	CDC *pDc = pDC;

	if (NULL == pDc) {
		pDc = GetDC();
	}

	CDC dc;
	dc.CreateCompatibleDC(pDC);

	CBitmap expression;
	UINT16 idx = 0;
	if (MINE_TRUE == m_isFail) {
		idx = MINE_LOSE;
	}
	else if (MINE_TRUE == m_isSuccess) {
		idx = MINE_WIN;
	}
	else {
		idx = MINE_PROCESS;
	}
	expression.LoadBitmap(m_bitmapId[idx]);
	dc.SelectObject(&expression);

	UINT16 l_x = (UINT16)(m_widthOffset + m_mineMapSize * m_bitMapInfo.bmWidth / 2 - 10);
	UINT16 l_y = 20;

	pDc->BitBlt(l_x, l_y, m_bitMapInfo.bmWidth, m_bitMapInfo.bmHeight, &dc, 0, 0, SRCCOPY);
	if (NULL == pDC) {
		pDc->DeleteDC();
	}
}

void CsweepMineMFCView::ShowMineMap(CDC* pDC) {

	ShowExpression(pDC);
	ShowInfo(pDC);

	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap bmp;

	Location pos;
	for (pm_mineNode = m_mineList.begin(); pm_mineNode != m_mineList.end(); pm_mineNode ++) {	
		CalcMineBlockLeftTopPos(&pos, pm_mineNode->x, pm_mineNode->y);
		if (MINE_FALSE == pm_mineNode->isChosen) {
			bmp.DeleteObject();
			bmp.LoadBitmap(m_bitmapId[MINE_CLOSED]);

			dc.SelectObject(&bmp);
		}
		else {
			bmp.DeleteObject();
			bmp.LoadBitmap(m_bitmapId[pm_mineNode->type]);

			dc.SelectObject(&bmp);
		}
		pDC->BitBlt(pos.x, pos.y, m_bitMapInfo.bmWidth, m_bitMapInfo.bmHeight, &dc, 0, 0, SRCCOPY);
	}
}

void CsweepMineMFCView::ShowFailMineMap() {
	ShowExpression();

	CDC dc, *pDC = NULL;
	pDC = GetDC();
	dc.CreateCompatibleDC(pDC);
	CBitmap bmp;

	Location pos;
	for (pm_mineNode = m_mineList.begin(); pm_mineNode != m_mineList.end(); pm_mineNode ++) {
		if (MINE_FALSE == pm_mineNode->isChosen) {
			bmp.DeleteObject();
			bmp.LoadBitmap(m_bitmapId[pm_mineNode->type]);

			dc.SelectObject(&bmp);
			CalcMineBlockLeftTopPos(&pos, pm_mineNode->x, pm_mineNode->y);
			pDC->BitBlt(pos.x, pos.y, m_bitMapInfo.bmWidth, m_bitMapInfo.bmHeight, &dc, 0, 0, SRCCOPY);

			pm_mineNode->isChosen = MINE_TRUE;
		}
	}
	pDC->DeleteDC();
}

void CsweepMineMFCView::ShowWinMineMap() {
	ShowExpression();

	CDC dc, *pDC = NULL;
	pDC = GetDC();
	dc.CreateCompatibleDC(pDC);
	CBitmap bmp;

	Location pos;
	for (pm_mineLocation = m_mineLocationList.begin(); pm_mineLocation != m_mineLocationList.end(); pm_mineLocation ++) {
		pm_mineNode = m_mineList.begin() + pm_mineLocation->y * m_mineMapSize + pm_mineLocation->x;
		bmp.DeleteObject();
		bmp.LoadBitmap(m_bitmapId[MINE_TRIANGLE]);

		dc.SelectObject(&bmp);
		CalcMineBlockLeftTopPos(&pos, pm_mineNode->x, pm_mineNode->y);
		pDC->BitBlt(pos.x, pos.y, m_bitMapInfo.bmWidth, m_bitMapInfo.bmHeight, &dc, 0, 0, SRCCOPY);

		pm_mineNode->isChosen = MINE_TRUE;
	}
	pDC->DeleteDC();
}

UINT8 CsweepMineMFCView::JudgeClickPosition(UINT16 x, UINT16 y) {
	if (x < m_widthOffset || x > (m_widthOffset + m_mineMapSize * m_bitMapInfo.bmWidth) || y < m_heightOffset || y > (m_heightOffset + m_mineMapSize * m_bitMapInfo.bmHeight)) {
		return MINE_FALSE;
	}

	return MINE_TRUE;
}

UINT8 CsweepMineMFCView::JudgeIsExpression(UINT16 x, UINT16 y) {
	UINT16 l_x = (UINT16)(m_widthOffset + m_mineMapSize * m_bitMapInfo.bmWidth / 2 - 10);
	UINT16 l_y = 20;

	if (x < l_x || x > l_x + m_bitMapInfo.bmWidth || y < l_y || y > l_y + m_bitMapInfo.bmHeight) {
		return MINE_FALSE;
	}

	return MINE_TRUE;
}

void CsweepMineMFCView::ShowInfo(CDC *pDc) {
	CDC *pDC = pDc;

	if (NULL == pDC) {
		pDC = GetDC();
	}

	CDC dc;
	CFont font;
	font.CreatePointFont(100, "华文行楷", NULL);
	CFont *pOldFont = pDC->SelectObject(&font);
	COLORREF clr = pDC->SetTextColor(125);
	CString str;
	str.Format("步数：%d     雷数：%d     标记数：%d    时间：%d:%d:%d  ", m_steps, m_mineCnt, m_tagCnt, m_time / 3600, m_time % 3600 / 60, m_time % 3600 % 60);
	pDC->TextOut(m_bitMapInfo.bmWidth, m_heightOffset - 15, str);
	pDC->SelectObject(pOldFont);

	if (NULL == pDc) {
		pDC->DeleteDC();
	}
}


// CsweepMineMFCView 诊断

#ifdef _DEBUG
void CsweepMineMFCView::AssertValid() const
{
	CView::AssertValid();
}

void CsweepMineMFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CsweepMineMFCDoc* CsweepMineMFCView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CsweepMineMFCDoc)));
	return (CsweepMineMFCDoc*)m_pDocument;
}
#endif //_DEBUG

void CsweepMineMFCView::RedrawMineMap() {
	Invalidate();
	UpdateWindow();
}


// CsweepMineMFCView 消息处理程序

void CsweepMineMFCView::OpenMine(UINT8 x, UINT8 y) {
	CDC dc, *pDc = NULL;
	pDc = GetDC();
	dc.CreateCompatibleDC(pDc);
	CBitmap bmp;
	vector<S_MineNode>::iterator pTemp;

	pTemp = m_mineList.begin() + y * m_mineMapSize + x;
	if (MINE_TRUE == pTemp->isFlag || MINE_TRUE == pTemp->isQuestion) {
		return;
	}
	pTemp->isChosen = MINE_TRUE;
	m_steps++;

	bmp.LoadBitmap(m_bitmapId[pTemp->type]);
	dc.SelectObject(&bmp);

	Location pos;
	CalcMineBlockLeftTopPos(&pos, x, y);

	pDc->BitBlt(pos.x, pos.y, m_bitMapInfo.bmWidth, m_bitMapInfo.bmHeight, &dc, 0, 0, SRCCOPY);
	pDc->DeleteDC();
	ShowInfo();

	if (m_mineCnt == m_mineMapSize * m_mineMapSize - m_steps) {
		m_isSuccess = MINE_TRUE;
		ShowWinMineMap();
	}
}

UINT8 CsweepMineMFCView::CheckEmpty(UINT8 x, UINT8 y) {
	UINT8 x_max = 0, x_min = 0, y_max = 0, y_min = 0;
	UINT16 offset = 0;
	vector<S_MineNode>::iterator pTemp;

	CalcMapEdge(x, y, &x_max, &x_min, &y_max, &y_min);
	for (int i = y_min; i <= y_max; i ++) {
		offset += i * m_mineMapSize;
		offset += x_min;
		for (pTemp = m_mineList.begin() + offset; pTemp <= m_mineList.begin() + (offset + (x_max - x_min)); pTemp ++) {
			if (MINE_NUM_0 != pTemp->type) {
				return MINE_FALSE;
			}
		}
		offset = 0;
	}

	return MINE_TRUE;
}

void CsweepMineMFCView::OpenEmpty(UINT8 x, UINT8 y) {
	UINT8 x_max = 0, x_min = 0, y_max = 0, y_min = 0;
	UINT16 offset = 0;
	vector<S_MineNode>::iterator pTemp;

	CalcMapEdge(x, y, &x_max, &x_min, &y_max, &y_min);
	for (int i = y_min; i <= y_max; i ++) {
		offset += i * m_mineMapSize;
		offset += x_min;
		for (pTemp = m_mineList.begin() + offset; pTemp <= m_mineList.begin() + (offset + (x_max - x_min)); pTemp ++) {
			if (MINE_FALSE == pTemp->isChosen) {
				AutoOpenEmptyMine(pTemp->x, pTemp->y);
			}
		}
		offset = 0;
	}
}

// void CsweepMineMFCView::AutoOpenEmptyMine(UINT8 x, UINT8 y) {
// 	OpenMine(x, y);
// 	if (MINE_TRUE == CheckEmpty(x, y)) {
// 		OpenEmpty(x, y);
// 	}
// }

void CsweepMineMFCView::AutoOpenEmptyMine(UINT8 x, UINT8 y) {
	OpenMine(x, y);

	UINT8 x_max = 0, x_min = 0, y_max = 0, y_min = 0;
	UINT16 offset = 0;
	vector<S_MineNode>::iterator pTemp;

	pTemp = m_mineList.begin() + y * m_mineMapSize + x;
	if (pTemp->type >= MINE_NUM_1 && pTemp->type <= MINE_NUM_8) {
		return;
	}

	CalcMapEdge(x, y, &x_max, &x_min, &y_max, &y_min);
	for (int i = y_min; i <= y_max; i ++) {
		offset += i * m_mineMapSize;
		offset += x_min;
		for (pTemp = m_mineList.begin() + offset; pTemp <= m_mineList.begin() + (offset + (x_max - x_min)); pTemp ++) {
			if (MINE_FALSE == pTemp->isChosen && (MINE_NUM_0 <= pTemp->type && pTemp->type <= MINE_NUM_8)) {
				AutoOpenEmptyMine(pTemp->x, pTemp->y);
			}
		}
		offset = 0;
	}
}

void CsweepMineMFCView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (MINE_TRUE == m_isFail || MINE_TRUE == m_isSuccess) {
		if (MINE_TRUE == JudgeIsExpression((UINT16)point.x, (UINT16)point.y)) {
			SetMineMap(m_mineMapSize, m_mineCnt, m_bitmapId[MINE_CLOSED]);
			RedrawMineMap();
		}
		return;
	}

	if (MINE_FALSE == JudgeClickPosition((UINT16)point.x, (UINT16)point.y)) {
		return;
	}

	pLocation pLClickLocation = new Location;
	CalcClickPosition(point, pLClickLocation);

	pm_mineNode = m_mineList.begin() + pLClickLocation->y * m_mineMapSize + pLClickLocation->x;

	if (MINE_TRUE == pm_mineNode->isFlag || MINE_TRUE == pm_mineNode->isQuestion) {
		return;
	}

	if (MINE_FALSE == pm_mineNode->isChosen) {
		if (MINE_NUM_0 == pm_mineNode->type) {
			AutoOpenEmptyMine(pm_mineNode->x, pm_mineNode->y);
		}
		else {
			m_steps ++;
			ShowInfo();
			pm_mineNode->isChosen = MINE_TRUE;
			if (MINE_TRUE == pm_mineNode->isMine) {
				m_isFail = MINE_TRUE;
				pm_mineNode->type = MINE_BOOMED;
			}
			else {
				if (m_mineCnt == m_mineMapSize * m_mineMapSize - m_steps) {
					m_isSuccess = MINE_TRUE;
				}
			}

			CDC dc, *pDc = NULL;
			pDc = GetDC();
			dc.CreateCompatibleDC(pDc);
			CBitmap bmp;

			bmp.LoadBitmap(m_bitmapId[pm_mineNode->type]);
			dc.SelectObject(&bmp);

			Location pos;
			CalcMineBlockLeftTopPos(&pos, pm_mineNode->x, pm_mineNode->y);

			pDc->BitBlt(pos.x, pos.y, m_bitMapInfo.bmWidth, m_bitMapInfo.bmHeight, &dc, 0, 0, SRCCOPY);
			pDc->DeleteDC();
		}
	}

	if (MINE_TRUE == m_isSuccess) {
		ShowWinMineMap();
	}

	if (MINE_TRUE == m_isFail) {
		ShowFailMineMap();
	}

	CView::OnLButtonDown(nFlags, point);
}

void CsweepMineMFCView::AutoOpenMine(UINT8 x, UINT8 y) {
	UINT8 x_max = 0, x_min = 0, y_max = 0, y_min = 0;
	UINT16 offset = 0;

	CalcMapEdge(x, y, &x_max, &x_min, &y_max, &y_min);
	for (int i = y_min; i <= y_max; i ++) {
		offset += i * m_mineMapSize;
		offset += x_min;
		for (pm_mineNodeTemp = m_mineList.begin() + offset; pm_mineNodeTemp <= m_mineList.begin() + (offset + (x_max - x_min)); pm_mineNodeTemp ++) {
			if (MINE_FALSE == pm_mineNodeTemp->isChosen && MINE_FALSE == pm_mineNodeTemp->isFlag && MINE_FALSE == pm_mineNodeTemp->isQuestion) {
				if (MINE_NUM_0 == pm_mineNodeTemp->type) {
					AutoOpenEmptyMine(pm_mineNodeTemp->x, pm_mineNodeTemp->y);
				}
				else {
					pm_mineNodeTemp->isChosen = MINE_TRUE;
					m_steps ++;
					ShowInfo();
					if (MINE_TRUE == pm_mineNodeTemp->isMine) {
						m_isFail = MINE_TRUE;
						pm_mineNodeTemp->type = MINE_BOOMED;
					}
					else {
						if (m_steps == m_mineMapSize * m_mineMapSize - m_mineCnt) {
							m_isSuccess = MINE_TRUE;
						}
					}

					CDC dc, *pDc = NULL;
					pDc = GetDC();
					dc.CreateCompatibleDC(pDc);
					CBitmap bmp;

					bmp.LoadBitmap(m_bitmapId[pm_mineNodeTemp->type]);
					dc.SelectObject(&bmp);

					Location pos;
					CalcMineBlockLeftTopPos(&pos, pm_mineNodeTemp->x, pm_mineNodeTemp->y);

					pDc->BitBlt(pos.x, pos.y, m_bitMapInfo.bmWidth, m_bitMapInfo.bmHeight, &dc, 0, 0, SRCCOPY);
					pDc->DeleteDC();
				}
			}
		}
		offset = 0;
	}

	if (MINE_TRUE == m_isSuccess) {
		ShowWinMineMap();
	}

	if (MINE_TRUE == m_isFail) {
		ShowFailMineMap();
	}
}

void CsweepMineMFCView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (MINE_FALSE == JudgeClickPosition((UINT16)point.x, (UINT16)point.y) || MINE_TRUE == m_isFail || MINE_TRUE == m_isSuccess) {
		return;
	}

	pLocation pLClickLocation = new Location;
	CalcClickPosition(point, pLClickLocation);

	pm_mineNode = m_mineList.begin() + pLClickLocation->y * m_mineMapSize + pLClickLocation->x;
	if (MINE_TRUE == pm_mineNode->isChosen) {
		AutoOpenMine(pm_mineNode->x, pm_mineNode->y);
	}


	CView::OnLButtonDblClk(nFlags, point);
}

void CsweepMineMFCView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (MINE_TRUE == m_isFail || MINE_TRUE == m_isSuccess) { 
		return;
	}

	if (MINE_FALSE == JudgeClickPosition((UINT16)point.x, (UINT16)point.y)) {
		return;
	}

	pLocation pRClickLocation = new Location;
	CalcClickPosition(point, pRClickLocation);


	pm_mineNode = m_mineList.begin() + pRClickLocation->y * m_mineMapSize + pRClickLocation->x;
	
	if (MINE_FALSE == pm_mineNode->isChosen) {
		UINT8 type = 0;
		if (MINE_FALSE == pm_mineNode->isQuestion && MINE_FALSE == pm_mineNode->isFlag) {
			type = MINE_QUESTION;
			pm_mineNode->isQuestion = MINE_TRUE;
		}
		else if (MINE_TRUE == pm_mineNode->isQuestion){
			type = MINE_TRIANGLE;
			pm_mineNode->isFlag = MINE_TRUE;
			pm_mineNode->isQuestion = MINE_FALSE;
			m_tagCnt++;
			ShowInfo();
		}
		else {
			type = MINE_CLOSED;
			pm_mineNode->isFlag = MINE_FALSE;
			m_tagCnt--;
			ShowInfo();
		}

		CDC dc, *pDc = NULL;
		pDc = GetDC();
		dc.CreateCompatibleDC(pDc);
		CBitmap bmp;

		bmp.LoadBitmap(m_bitmapId[type]);
		dc.SelectObject(&bmp);

		Location pos;
		CalcMineBlockLeftTopPos(&pos, pm_mineNode->x, pm_mineNode->y);

		pDc->BitBlt(pos.x, pos.y, m_bitMapInfo.bmWidth, m_bitMapInfo.bmHeight, &dc, 0, 0, SRCCOPY);
		pDc->DeleteDC();
	}

	CView::OnRButtonDown(nFlags, point);
}

void CsweepMineMFCView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (MINE_FALSE == m_isFail && MINE_FALSE == m_isSuccess) {
		m_time ++;
		ShowInfo();
	}

	CView::OnTimer(nIDEvent);
}

DWORD WINAPI CsweepMineMFCView::WriteDebugInfoProc(LPVOID lpParameter) {
#ifdef GET_DEBUG_INFO
	while (TRUE) {
		if (m_debugInfo.info.GetLength() > 10240 || MINE_TRUE == m_calcNumOver) {
			WaitForSingleObject(m_hMutex, INFINITE);
			m_debugFile.Write(m_debugInfo.info.GetBuffer(), m_debugInfo.info.GetLength());
			m_debugInfo.info.Empty();
			ReleaseMutex(m_hMutex);

			if (MINE_TRUE == m_calcNumOver) {
				m_debugFile.Flush();
				break;
			}
		}
	}
#endif

	return 0;
}

void CsweepMineMFCView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类

	SetMineMap();

	SetTimer(1, 1000, NULL);

#ifdef GET_DEBUG_INFO
	m_debugFile.Open("debugInfo.txt", CFile::modeCreate | CFile::modeWrite);

	HANDLE hDebugThread;
	hDebugThread = CreateThread(NULL, 0, WriteDebugInfoProc, NULL, 0, NULL);
	CloseHandle(hDebugThread);

	m_hMutex = CreateMutex(NULL, FALSE, NULL);
#endif
}

void CsweepMineMFCView::ShowDebugInfo(const char* fmt,...) {
	char s[1024] = {0};

	va_list ap; 
	va_start(ap, fmt);      
	vsprintf(s,fmt,ap);  
	va_end(ap);  

	MessageBox(s);
}

void CsweepMineMFCView::WriteDebugInfo(const char* fmt,...) {
#ifdef GET_DEBUG_INFO
	char s[1024] = {0};

	va_list ap; 
	va_start(ap, fmt);      
	vsprintf(s,fmt,ap);  
	va_end(ap);  

	WaitForSingleObject(m_hMutex, INFINITE);
	m_debugInfo.info += s;
	ReleaseMutex(m_hMutex);
#endif
}
