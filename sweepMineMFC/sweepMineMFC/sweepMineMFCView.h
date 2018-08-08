	// sweepMineMFCView.h : CsweepMineMFCView 类的接口
//


#pragma once

#include "sweepMineMFCDoc.h"

#include "sweepmine.h"
using namespace std;


class CsweepMineMFCView : public CView
{
protected: // 仅从序列化创建
	CsweepMineMFCView();
	DECLARE_DYNCREATE(CsweepMineMFCView)

// 属性
public:
	CsweepMineMFCDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CsweepMineMFCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

private:
	vector<S_MineNode> m_mineList;
	vector<S_MineNode>::iterator pm_mineNode, pm_mineNodeTemp;

	list<mineLocation> m_mineLocationList;
	list<mineLocation>::iterator pm_mineLocation;

	BITMAP m_bitMapInfo;

	UINT8 m_isFail;
	UINT8 m_isSuccess;

	static UINT8 m_calcNumOver;
	static CFile m_debugFile;
	static debugInfo m_debugInfo;
	static HANDLE m_hMutex;

public:
	void SetMineMap(UINT8 mineMapSize = 10, UINT16 mineCnt = 10, UINT16 grade = IDB_BITMAP_PRIMACY_SQUARE);

private:
	void SetMine(UINT16 mineCnt = 10);
	UINT8 GetRandNum();
	void GetMinePosition(UINT8 *x, UINT8 *y);
	void CalcMineNum();
	UINT8 GetMineNum(UINT8 x, UINT8 y);
	void CalcMapEdge(UINT8 x, UINT8 y, UINT8 *x_max, UINT8 *x_min, UINT8 *y_max, UINT8 *y_min);
	void ShowMineMap(CDC* pDC);
	void ShowExpression(CDC *pDC = NULL);
	void ShowFailMineMap();
	void ShowWinMineMap();
	void ResetMineMap();
	void CalcClickPosition(CPoint point, pLocation location);
	void RedrawMineMap();
	UINT8 JudgeClickPosition(UINT16 x, UINT16 y);
	UINT8 JudgeIsExpression(UINT16 x, UINT16 y);
	void CalcMineBlockLeftTopPos(Location *pPos, UINT8 x, UINT8 y);
	void AutoOpenMine(UINT8 x, UINT8 y);
	UINT8 CheckEmpty(UINT8 x, UINT8 y);
	void AutoOpenEmptyMine(UINT8 x, UINT8 y);
	void OpenMine(UINT8 x, UINT8 y);
	void OpenEmpty(UINT8 x, UINT8 y);
	void ShowInfo(CDC *pDc = NULL);
	void ShowDebugInfo(const char* fmt,...);

private:
	UINT8 m_mineMapSize;
	UINT16 m_mineCnt;
	UINT32 *m_bitmapId;
	static UINT32 m_mineBitmapIdPrimacy[17];
	static UINT32 m_mineBitmapIdMedium[17];
	static UINT32 m_mineBitmapIdSenior[17];
	UINT8 m_heightOffset;
	UINT8 m_widthOffset;
	UINT16 m_steps;
	UINT32 m_time;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnInitialUpdate();
	void WriteDebugInfo(const char* fmt,...);
	static DWORD WINAPI WriteDebugInfoProc(LPVOID lpParameter);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	unsigned int m_tagCnt;
};

#ifndef _DEBUG  // sweepMineMFCView.cpp 中的调试版本
inline CsweepMineMFCDoc* CsweepMineMFCView::GetDocument() const
   { return reinterpret_cast<CsweepMineMFCDoc*>(m_pDocument); }
#endif

