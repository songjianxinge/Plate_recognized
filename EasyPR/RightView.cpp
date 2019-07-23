// RightView.cpp : 实现文件


#include "stdafx.h"
#include "EasyPR.h"
#include "RightView.h"
#include "EasyPRView.h"

// CRightView

IMPLEMENT_DYNCREATE(CRightView, CFormView)

CRightView::CRightView()
	: CFormView(IDD_RIGHTVIEW)
{

}
CRightView* CRightView::instance_ = nullptr;
CRightView* CRightView::instance()
{
	if (!instance_)
		instance_ = new CRightView;
	return instance_;
}

CRightView::~CRightView()
{
}

void CRightView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOCATE, m_picL);
	DDX_Control(pDX, IDC_SEGMENT, m_picS);
	//  DDX_Text(pDX, IDC_RESULT, m_result);
	DDX_Control(pDX, IDC_RESULT, m_result);
}

BEGIN_MESSAGE_MAP(CRightView, CFormView)
	//ON_WM_CREATE()
	//ON_MESSAGE(NM_A,OnMyChange)
END_MESSAGE_MAP()


// CRightView 诊断

#ifdef _DEBUG
void CRightView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CRightView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRightView 消息处理程序





void CRightView::OnInitialUpdate()
{
	 pDoc = GetDocument();
	if (!pDoc)
		return;
	if (pDoc->locatePicture.empty())
		return;
	CDC *pDC = GetDlgItem(IDC_LOCATE)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CFormView::OnInitialUpdate();
	CRect rect;
	GetDlgItem(IDC_LOCATE)->GetClientRect(&rect);
	CImage c;
	SetRect(rect, rect.left, rect.top, rect.right, rect.bottom);
	pDoc->MatToCImage(pDoc->locatePicture,c);
	c.Draw(pDC->GetSafeHdc(), 0, 0, c.GetWidth(), c.GetHeight(),
		0, 0, c.GetWidth(), c.GetHeight());
	m_result.SetWindowTextW(_T("dd"));
	// TODO: 在此添加专用代码和/或调用基类
}


void CRightView::OnDraw(CDC* pDC)
{
	// TODO: 在此添加专用代码和/或调用基类
	pDoc = GetDocument();
	if (!pDoc)
		return;
	if (pDoc->locatePicture.empty())
		return;
	if (pDoc->charsegment.empty())
		return;
	HDC hDC = GetDlgItem(IDC_LOCATE)->GetDC()->GetSafeHdc();
	CRect rect;
	GetDlgItem(IDC_LOCATE)->GetClientRect(&rect);
	CImage c;
	pDoc->MatToCImage(pDoc->locatePicture, c);
	c.Draw(hDC, 0, 0, rect.Width(), rect.Height(),
		0, 0, c.GetWidth(), c.GetHeight());

	HDC hDC2 = GetDlgItem(IDC_SEGMENT)->GetDC()->GetSafeHdc();
	CRect rects;
	GetDlgItem(IDC_SEGMENT)->GetClientRect(&rects);
	pDoc->MatToCImage(pDoc->charsegment, c);
	c.Draw(hDC2, 0, 0, rects.Width(), rects.Height(),
		0, 0, c.GetWidth(), c.GetHeight());
	if(m_result_str != "")
		AfxMessageBox(m_result_str);
}

