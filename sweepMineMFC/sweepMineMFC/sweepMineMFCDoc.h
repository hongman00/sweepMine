// sweepMineMFCDoc.h : CsweepMineMFCDoc ��Ľӿ�
//


#pragma once


class CsweepMineMFCDoc : public CDocument
{
protected: // �������л�����
	CsweepMineMFCDoc();
	DECLARE_DYNCREATE(CsweepMineMFCDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CsweepMineMFCDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


