
// EasyPRView.cpp : CEasyPRView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "EasyPR.h"
#endif

#include "core_func.h"
#include "EasyPRDoc.h"
#include "EasyPRView.h"
#include "RightView.h"
#include <opencv.hpp>
#include "config.h"
#include <algorithm>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace easypr;

// CEasyPRView

IMPLEMENT_DYNCREATE(CEasyPRView, CView)

BEGIN_MESSAGE_MAP(CEasyPRView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &CEasyPRView::OnFileOpen)
	ON_COMMAND(ID_PLATELA, &CEasyPRView::OnPlatela)
	
END_MESSAGE_MAP()

// CEasyPRView 构造/析构

CEasyPRView::CEasyPRView()
{
	// TODO: 在此处添加构造代码
	fileflag = 0;
}

CEasyPRView::~CEasyPRView()
{
}

BOOL CEasyPRView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CEasyPRView 绘制

void CEasyPRView::OnDraw(CDC* pDC)
{
	CEasyPRDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect rt;
	GetClientRect(&rt);
	nHeight = rt.Height();
	nWidth = rt.Width();
	if (!src_img.IsNull()) {

		int fixed_width = std::min(src_img.GetWidth(), nWidth);
		int fixed_height = std::min(src_img.GetHeight(), nHeight);

		double ratio_w = fixed_width / (double)src_img.GetWidth();
		double ratio_h = fixed_height / (double)src_img.GetHeight();
		double ratio = std::min(ratio_w, ratio_h);

		int show_width = (int)(ratio * src_img.GetWidth());
		int show_height = (int)(ratio * src_img.GetHeight());
		int offsetx = (nWidth - show_width) / 2;
		int offsety = (nHeight - show_height) / 2;
		::SetStretchBltMode(pDC->GetSafeHdc(), COLORONCOLOR);

		src_img.StretchBlt(pDC->GetSafeHdc(),
			offsetx, offsety,
			show_width, show_height, 0, 0, src_img.GetWidth(), src_img.GetHeight(),
			SRCCOPY);
	}
	//pDoc->SetPath(filePath);
	// &img = pDoc->src_image;
	/*CEasyPRDoc::MatToCImage(src_img, src_img);
	src_img.Draw(pDC->GetSafeHdc(), 0, 0, cimage.GetWidth(), cimage.GetHeight(),
	0, 0, cimage.GetWidth(), cimage.GetHeight());*/
	// TODO: 在此处为本机数据添加绘制代码
}


// CEasyPRView 打印

BOOL CEasyPRView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CEasyPRView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CEasyPRView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CEasyPRView 诊断

#ifdef _DEBUG
void CEasyPRView::AssertValid() const
{
	CView::AssertValid();
}

void CEasyPRView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEasyPRDoc* CEasyPRView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEasyPRDoc)));
	return (CEasyPRDoc*)m_pDocument;
}
#endif //_DEBUG


// CEasyPRView 消息处理程序


void CEasyPRView::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(TRUE, _T(".bmp"), _T("*.bmp"), OFN_HIDEREADONLY |
		OFN_OVERWRITEPROMPT, _T("位图文件(*.bmp)|*.bmp|JPEG文件(*.jpg)|*.jpg||"));
		if (dlg.DoModal() == IDOK)
		{
			if (fileflag == 1)
			{
				src_img.Destroy();
			}
			filePath = CT2A(dlg.GetPathName().GetBuffer());
			m_MatImg = cv::imread(filePath);
			src_img.Load(dlg.GetPathName());
			fileflag = 1;
			Invalidate();

		}
}


void Result_Print(set<string> _set)
{
	for (set<string>::iterator it = _set.begin(); it != _set.end(); it++)
	{
		string z = *it;
		CString str(z.c_str());
		CRightView::instance()->m_result_str = str;
		AfxMessageBox(str);
	}

}
Mat mymerge(vector<Mat> &v)
{
	
	Size bigsize(v[0].cols * v.size(), v[0].rows );//合并后图片size

	vector<Mat>temp(v.size());

	Mat mergefinal;
	mergefinal.create(bigsize, CV_MAKETYPE(v[0].depth(), 1));
	mergefinal = Scalar::all(0);

	for (int i = 0; i < 7; i++)
	{
		if (i<7)temp[i] = mergefinal(Rect(i*v[0].cols, 0, v[0].cols, v[0].rows));
		else temp[i] = mergefinal(Rect((i - 10)*v[0].cols, v[0].rows, v[0].cols, v[0].rows));
		v[i].copyTo(temp[i]); //copy图片到对应位置
	}
	return mergefinal;
}

int CEasyPRView::charsRecognise(Mat plate, std::string& plateLicense)
{
	std::vector<Mat> matChars;
	int result =cs.charsSegment(plate, matChars);
	if (result == 0) {
		int num = matChars.size();
		for (int j = 0; j < num; j++)
		{
			Mat charMat = matChars.at(j);
			bool isChinses = false;
			float maxVal = 0;
			if (j == 0) {
				bool judge = true;
				isChinses = true;
				auto character = CharsIdentify::instance()->identifyChinese(charMat, maxVal, judge);
				plateLicense.append(character.second);
			}
			else {
				isChinses = false;
				auto character = CharsIdentify::instance()->identify(charMat, isChinses);
				plateLicense.append(character.second);
			}
		}

	}
	if (plateLicense.size() < 7) {
		return -1;
	}

	return result;
}


int CEasyPRView::charsRecognise(CPlate& plate, std::string& plateLicense) {
	std::vector<Mat> matChars;
	std::vector<Mat> grayChars;
	Mat plateMat = plate.getPlateMat();
	if (0) writeTempImage(plateMat, "plateMat/plate");
	Color color;
	if (plate.getPlateLocateType() == CMSER) {
		color = plate.getPlateColor();
	}
	else {
		int w = plateMat.cols;
		int h = plateMat.rows;
		Mat tmpMat = plateMat(Rect_<double>(w * 0.1, h * 0.1, w * 0.8, h * 0.8));
		color = getPlateType(tmpMat, true);
	}

	int result = cs.charsSegmentUsingOSTU(plateMat, matChars, grayChars, color);

	if (result == 0) {
		int num = matChars.size();
		for (int j = 0; j < num; j++)
		{
			Mat charMat = matChars.at(j);
			Mat grayChar = grayChars.at(j);
			if (color != Color::BLUE)
				grayChar = 255 - grayChar;

			bool isChinses = false;
			std::pair<std::string, std::string> character;
			float maxVal;
			if (0 == j) {
				isChinses = true;
				bool judge = true;
				character = CharsIdentify::instance()->identifyChineseGray(grayChar, maxVal, judge);
				plateLicense.append(character.second);

				// set plate chinese mat and str
				plate.setChineseMat(grayChar);
				plate.setChineseKey(character.first);
				if (0) writeTempImage(grayChar, "char_data/" + character.first + "/chars_");
			}
			else if (1 == j) {
				isChinses = false;
				bool isAbc = true;
				character = CharsIdentify::instance()->identify(charMat, isChinses, isAbc);
				plateLicense.append(character.second);
			}
			else {
				isChinses = false;
				SHOW_IMAGE(charMat, 0);
				character = CharsIdentify::instance()->identify(charMat, isChinses);
				plateLicense.append(character.second);
			}

			CCharacter charResult;
			charResult.setCharacterMat(charMat);
			charResult.setCharacterGrayMat(grayChar);
			if (isChinses)
				charResult.setCharacterStr(character.first);
			else
				charResult.setCharacterStr(character.second);

			plate.addReutCharacter(charResult);
		}
		if (plateLicense.size() < 7) {
			return -1;
		}
	}

	return result;
}

void CEasyPRView::OnPlatela()
{
	// TODO: 在此添加命令处理程序代码
	//const string file = filePath;
	pDoc = GetDocument();
	cv::Mat src = m_MatImg.clone();
	float scale = 1.f;
	Mat img = uniformResize(src,scale);
	//imshow("img", img);
	std::vector<CPlate> plateV;
	int index = 0;
	//int result = plate.plateLocate(src, resultVec);
	int result = plate.plateLocate(src, plateV,index);
	plate.setMaxPlates(2);
	for (vector<Rect_<float>> ::iterator it = plate.BoundRect.begin(); it != plate.BoundRect.end(); it++)
	{
		rectangle(m_MatImg, *it, Scalar(255, 0, 0), 3, 8);
	}
	pDoc->MatToCImage(m_MatImg, src_img);
	Invalidate();
	if (result != 0) return ;
	size_t num_p = plateV.size();
	for (size_t j = 0; j < num_p; j++)
	{
		CPlate& item = plateV.at(j);
		Mat plateM = item.getPlateMat();
		SHOW_IMAGE(plateM, 0);
		//scale the rect to src
		item.setPlateScale(scale);
		RotatedRect rect = item.getPlatePos();
		item.setPlatePos(scaleBackRect(rect, 1.f / scale));

		//get plate color
		Color color = item.getPlateColor();
		if (color == UNKNOWN)
		{
			color = getPlateType(plateM, true);
			item.setPlateColor(color);
		}
		std::string plateColor = getPlateColor(color);
		//imshow("plateM", plateM);
		int result_seg = cs.charsSegment(plateM, resultVec1);
		if (result_seg == 0 )
		{
			pDoc->locatePicture = plateM.clone();
			pDoc->charsegment = mymerge(resultVec1).clone();
			::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_A, (WPARAM)NM_A, (LPARAM)0);
			for (size_t block_i = 0; block_i < resultVec1.size() && resultVec1.size()<8; block_i++)
			{
				auto block = resultVec1[block_i];
				if (block_i == 0)
				{
					auto character = easypr::CharsIdentify::instance()->identify(block, true);
					license.append(character.second);
				}
				else
				{
					auto character = easypr::CharsIdentify::instance()->identify(block, false);
					license.append(character.second);
				}
			}	
			if (license == "")
				continue;
			m_set.insert(license);
			resultVec1.clear();
		}
		license.clear();

	}


	//size_t num = resultVec.size();
	//for (size_t j = 0; j < num; j++) {
	//	cv::Mat resultMat = resultVec[j];
	//	int score = CPlateJudge::instance()->plateJudge(resultVec[j]);
	//	if (score != 0)
	//	{
	//		if(m_set.size() != 0)
	//			Result_Print(m_set);
	//		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_A, (WPARAM)NM_A, (LPARAM)0);
	//		license.clear();
	//		resultVec.clear();
	//		resultVec1.clear();
	//		m_set.clear();
	//		return ;
	//	}
	//		
	//	int result_seg = cs.charsSegment(resultMat, resultVec1);
	//	if (result_seg == 0 )
	//	{
	//		//imshow("resultMat" + j, resultMat);
	//		pDoc->locatePicture = resultMat.clone();
	//		//pDoc->charsegment= mymerge(resultVec1).clone();
	//		pDoc->charsegment = mymerge(resultVec1).clone();
	//		//imshow("char",pDoc->charsegment);
	//		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_A, (WPARAM)NM_A, (LPARAM)0);
	//		for (size_t block_i = 0; block_i < resultVec1.size() && resultVec1.size()<8; block_i++)
	//		{
	//			auto block = resultVec1[block_i];
	//			//imshow("c" + block_i, resultVec1[block_i]);
	//			if (block_i == 0)
	//			{
	//				auto character = easypr::CharsIdentify::instance()->identify(block, true);
	//				license.append(character.second);
	//			}
	//			else
	//			{
	//				auto character = easypr::CharsIdentify::instance()->identify(block, false);
	//				license.append(character.second);
	//			}
	//		}	
	//		if (license == "")
	//			continue;
	//		m_set.insert(license);
	//		resultVec1.clear();
	//	}
	//	license.clear();
	//}
	Result_Print(m_set);
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_A, (WPARAM)NM_A, (LPARAM)0);
	resultVec.clear();
	m_set.clear();
	destroyWindow("plate_locate");
}
