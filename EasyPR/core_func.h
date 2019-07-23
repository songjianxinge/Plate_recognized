#pragma once
#include <opencv2/opencv.hpp>
#include "character.hpp"
#include "plate.hpp"
#include <iostream>
using namespace cv;
using namespace std;


namespace easypr {
	Mat colorMatch(const Mat& src, Mat& match, const Color r, const bool adaptive_minsv);
	bool plateColorJudge(const Mat& src, const Color r, const bool adaptive_minsv, float &percent);
	bool bFindLeftRightBound(Mat &bound, int & posLeft, int & posRight);
	bool bFindLeftRightBound2(Mat &bound, int & posLeft, int & posRight);
	bool bFindLeftRightBound3(Mat &bound, int & posLeft, int & posRight);
	//清楚车牌上的柳丁
	bool clearLiuDing(Mat &img);
	void clearLiuDingOnly(Mat & img);
	void clearLiuDing(Mat mask, int &top, int& bottom);

	Mat histeq(Mat in);
	Rect GetCenterRect(Mat &in);
	Mat CutTheRect(Mat &in, Rect &rect);
	int ThresholdOtsu(Mat mat);

	Mat ProjectedHistogram(Mat img, int t, int threshold = 20);

	Mat showHistogram(const Mat &hist);

	Mat preprocessChar(Mat in, int char_size);

	Rect GetChineseRect(const Rect rectSpe);

	bool verifyCharSizes(Rect t);
	bool verifyPlateSizes(Rect r);
	bool verifyRotatedPlateSizes(RotatedRect m, bool showDebug = false);
	//非极大值御制
	void NMStoCharacter(std::vector<CCharacter> &inVec,double overlap);
	//绘制矩形角度
	void rotatedRectangle(InputOutputArray img, RotatedRect rect,
		const Scalar & color, int thickness = 1, 
		int lineType = LINE_8, int shift = 0);
	//ostu region
	void spatial_ostu(InputArray _src, int grid_x, int grid_y, Color type = BLUE);
	Color getPlateType(const Mat &src, const bool adaptive_minsv);
	//Scale to small image 
	Mat scaleImg(const Mat & image, const Size &maxSize, double &scale_rat);

	//Scale back RoatedRect 
	RotatedRect scaleBackRect(const RotatedRect &rr, const float scale_rat);


	//! use verify size to first generate char candidates
	void mserCharMatch(const Mat &src, std::vector<Mat> &match, std::vector<CPlate>& out_plateVec_blue, std::vector<CPlate>& out_plateVec_yellow,
		bool usePlateMser, std::vector<RotatedRect>& out_plateRRect_blue, std::vector<RotatedRect>& out_plateRRect_yellow, int index = 0, bool showDebug = false);

	// computer the insert over union about two rrect
	bool computeIOU(const RotatedRect& rrect1, const RotatedRect& rrect2, const int width, const int height, const float thresh, float& result);
	float computeIOU(const RotatedRect& rrect1, const RotatedRect& rrect2, const int width, const int height);

	bool computeIOU(const Rect& rect1, const Rect& rect2, const float thresh, float& result);
	float computeIOU(const Rect& rect1, const Rect& rect2);

	/*
		convert form mser point to image.
		The function creaeted first
	*/
	Mat adaptive_image_from_points(const std::vector<Point>& points,
		const Rect& rect, const Size& size, const Scalar& backgroundColor = Scalar(0, 0, 0),
		const Scalar& forgroundColor = Scalar(255, 255, 255), bool gray = true);

	// Calculate a rect have same length and width and remains the center
	Rect adaptive_charrect_from_rect(const Rect& rect, int maxwidth, int maxheight, bool useExtendHeight = false);

	// calc safe rect
	bool calcSafeRect(const RotatedRect& roi_rect, const Mat& src,
		Rect_<float>& safeBoundRect);
	bool calcSafeRect(const RotatedRect &roi_rect, const int width, const int height,
		Rect_<float> &safeBoundRect);

	// uniform resize all the image to same size for the next process
	Mat uniformResize(const Mat &result, float& scale);

	// uniform resize all the plates to same size for the next process
	Mat uniformResizePlates(const Mat &result, float& scale);

	// show detect results
	void showDectectResults(const Mat& img, const std::vector<CPlate> &plateVec, size_t num);

	// show the results
	Mat showResult(const Mat &result, int img_index = 0);

	// enlarge the char rect
	Rect rectEnlarge(const Rect& src, const int mat_width, const int mat_height);
	Rect rectFit(const Rect &src, const int mat_width, const int mat_height);

	// write images to temp folder
	void writeTempImage(const Mat& outImg, const string path, int index = 0);

	// remove small hor lines in the plate
	bool judegMDOratio2(const Mat &image, const Rect &rect, std::vector<Point> &contour, Mat &result, const float thresh = 1.f,
		bool useExtendHeight = false);

	// clear top and bottom borders
	void clearBorder(const Mat &img, Rect& cropRect);

	// non-maxumun surpresion for id array
	template<typename T>
	void NMSforId(const vector<T> &arr, vector<int> & index)
	{
		//prepare
		int size = (int)arr.size();
		index.resize(size);
		for (int j = 0; j < size; j++)
		{
			index.at(j) = 0;
		}
		//nms
		int i = 1;
		while (i < size - 1)
		{
			if (arr.at(i) > arr.at(i + 1))
			{
				if (arr.at(i) >= arr.at(i - 1))
					index.at(i) = 1;
			}
			else
			{
				while (i < size - 1 && arr.at(i) <= arr.at(i + 1))
					i = i + 1;
				if (i < size - 1)
					index.at(i) = 1;
			}
		}

	}




}


