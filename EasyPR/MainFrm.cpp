
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "EasyPR.h"

#include "MainFrm.h"

#include <opencv2/imgproc/imgproc_c.h>
#include <iostream>
#include <vector>
#include <opencv.hpp>
#include "plate_judge.h"
#include "chars_segment.h"
#include "chars_identify.h"
#include "plate_locate.h"
#include "LifeView.h"
#include "EasyPRView.h"
#include "RightView.h"
using namespace std;
using namespace easypr;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(NM_A, OnMyChange)
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
	//// TODO: 在此添加成员初始化代码

	/*cout << "test_plate_locate" << endl;
	const string file = "resources/image/plate_lop.bmp";
	cv::Mat src = imread(file);
	vector<cv::Mat> resultVec;
	vector<cv::Mat> resultVec1;
	float maxVal;
	CPlateLocate plate;
	plate.setDebug(1);
	plate.setLifemode(true);
	CCharsSegment cs;
	int result = plate.plateLocate(src, resultVec);
	std::string license;
	if (result != 0) return;
	for (size_t j = 0; j < resultVec.size(); j++)
	{
		imshow("t" + j, resultVec[j]);
	}
	for (size_t j = 0; j < resultVec.size(); j++) {
		cv::Mat resultMat = resultVec[j];
		int score = CPlateJudge::instance()->plateJudge(resultVec[j]);
		if (score == 0) {
				int result_seg = cs.charsSegment(resultMat,resultVec1);
				if (result_seg == 0)
				{
					for (size_t block_i = 0; block_i < resultVec1.size(); block_i++)
					{
						auto block = resultVec1[block_i];
						if (block_i == 0)
						{
							auto character = CharsIdentify::instance()->identify(block, true);
							license.append(character.second);
						}
						else
						{
							auto character = CharsIdentify::instance()->identify(block, false);
							license.append(character.second);
						}
					}
				}
			license.c_str();
			CString str(license.c_str());
			AfxMessageBox(str);
			}
			waitKey(0);
		}
	destroyWindow("plate_locate");
*/
	
	//return result;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 如果不需要可停靠工具栏，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

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




BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类
	//拆成一行两列
	m_spliter.CreateStatic(this, 1, 2);
	//左侧和右侧具体的显示内容
	m_spliter.CreateView(0, 0, RUNTIME_CLASS(CEasyPRView), CSize(600, 500), pContext);
	m_spliter.CreateView(0, 1, RUNTIME_CLASS(CRightView), CSize(200, 500), pContext);

	return TRUE;
}
LRESULT CMainFrame::OnMyChange(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NM_A)
	{
		Invalidate();
		//AfxMessageBox(_T("t"));
	}

	return 0;
}