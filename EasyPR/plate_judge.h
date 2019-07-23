#pragma once
#include "feature.h"
#include "plate.hpp"

namespace easypr {

	class CPlateJudge
	{
	public:
		static CPlateJudge* instance();
		void LoadModel(std::string path);
		//使用NMS删除重叠的框
		int plateJudgeUsingNMS(const std::vector<CPlate> &c, std::vector<CPlate>&z, int maxPlates = 5);
		//通过调用SVM模型进行判别， 设置车牌分数，0 is plate, -1 is not.
		int plateSetScore(CPlate & plate);
		//将输出普通Mat转化为CPlate对象，调用plateSetScore函数求出其score
		int plateJudge(const Mat& plateMat);
		//输入一组Mat，对其Mat逐个调用上面结构的plateJudge,求出每个Mat对应的score
		int plateJudge(const std::vector<Mat> &inVec, std::vector<Mat> &resultVec);
		//同上，输入一组CPlate对象，取出它的Mat成员，然后对其逐个调用上面第一个结构的plateJudge
		int plateJudge(const std::vector<CPlate> &inVec, std::vector<CPlate> &resultVec);
	private:
		CPlateJudge();
		
		static CPlateJudge* instance_;
		svmCallback extractFeature;
		cv::Ptr<ml::SVM> svm_;
	};

}

