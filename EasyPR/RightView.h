#pragma once
#include "afxwin.h"
#include "EasyPRDoc.h"


// CRightView 窗体视图

class CRightView : public CFormView
{
	DECLARE_DYNCREATE(CRightView)
public :
	static CRightView *instance();
protected:
	CRightView();           // 动态创建所使用的受保护的构造函数
	virtual ~CRightView();
	
	//CEasyPRDoc* GetDocument() const;
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RIGHTVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
	inline CEasyPRDoc* CRightView::GetDocument() const
	{
		return reinterpret_cast<CEasyPRDoc*>(m_pDocument);
	}
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CEasyPRDoc* pDoc;
	CStatic m_picL;
	static CRightView* instance_;
private:
	CStatic m_picS;
public:
	CString m_result_str;
public:
	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* /*pDC*/);

	//void setImg(CEasyPRDoc* pDoc, int id, Mat &mat);
private:

//	CStatic m_result;
	CStatic m_result;
};


