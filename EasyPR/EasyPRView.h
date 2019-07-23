
// EasyPRView.h : CEasyPRView 类的接口
//

#pragma once

#include <string>
#include "EasyPR.h"
#include "config.h"
#include "EasyPRDoc.h"
#include "character.hpp"
#include "chars_identify.h"
#include "chars_segment.h"
#include "plate_locate.h"
#include "plate_judge.h"
#include <set>
#include "util.h"
#define NM_A (WM_USER + 100)
using namespace easypr;
class CEasyPRView : public CView
{
protected: // 仅从序列化创建
	CEasyPRView();
	DECLARE_DYNCREATE(CEasyPRView)

// 特性
public:
	CEasyPRDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CEasyPRView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

inline std::string getPlateColor(Color in) const {
		std::string color = "未知";
		if (BLUE == in) color = "蓝牌";
		if (YELLOW == in) color = "黄牌";
		if (WHITE == in) color = "白牌";
#ifdef OS_WINDOWS
		color = Utils::utf8_to_gbk(color.c_str());
#endif
		return color;
	}
protected:
int CEasyPRView::charsRecognise(Mat plate, std::string& plateLicense);
int CEasyPRView::charsRecognise(CPlate& plate, std::string& plateLicense);
protected:
	CEasyPRDoc* pDoc ;
	std::string filePath;
	cv::Mat m_MatImg;
	CImage src_img;
	easypr::CPlateLocate plate;
	easypr::CCharsSegment cs;
	int fileflag ;
	set<string> m_set;//集合存放车牌
	std::string license;//存放车牌字符
	vector<cv::Mat> resultVec;
	vector<cv::Mat> resultVec1;
	float maxVal;
	int nHeight;
	int nWidth;
	

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnPlatela();
};

#ifndef _DEBUG  // EasyPRView.cpp 中的调试版本
inline CEasyPRDoc* CEasyPRView::GetDocument() const
   { return reinterpret_cast<CEasyPRDoc*>(m_pDocument); }
#endif

