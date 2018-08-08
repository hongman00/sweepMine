// sweepMineMFCDoc.cpp : CsweepMineMFCDoc 类的实现
//

#include "stdafx.h"
#include "sweepMineMFC.h"

#include "sweepMineMFCDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CsweepMineMFCDoc

IMPLEMENT_DYNCREATE(CsweepMineMFCDoc, CDocument)

BEGIN_MESSAGE_MAP(CsweepMineMFCDoc, CDocument)
END_MESSAGE_MAP()


// CsweepMineMFCDoc 构造/析构

CsweepMineMFCDoc::CsweepMineMFCDoc()
{
	// TODO: 在此添加一次性构造代码

}

CsweepMineMFCDoc::~CsweepMineMFCDoc()
{
}

BOOL CsweepMineMFCDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CsweepMineMFCDoc 序列化

void CsweepMineMFCDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CsweepMineMFCDoc 诊断

#ifdef _DEBUG
void CsweepMineMFCDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CsweepMineMFCDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CsweepMineMFCDoc 命令
