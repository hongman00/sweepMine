// sweepMineMFCDoc.cpp : CsweepMineMFCDoc ���ʵ��
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


// CsweepMineMFCDoc ����/����

CsweepMineMFCDoc::CsweepMineMFCDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CsweepMineMFCDoc::~CsweepMineMFCDoc()
{
}

BOOL CsweepMineMFCDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CsweepMineMFCDoc ���л�

void CsweepMineMFCDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CsweepMineMFCDoc ���

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


// CsweepMineMFCDoc ����
