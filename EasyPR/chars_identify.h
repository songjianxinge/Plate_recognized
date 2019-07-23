#pragma once
#include <opencv.hpp>
#include <memory>
#include "character.hpp"
#include "feature.h"
#include "kv.h"
namespace easypr{
	class CharsIdentify
	{
	public:
		static CharsIdentify* instance();
		//通过神经网络模型Ann进行识别
		int classify(cv::Mat f, float& maxVal, bool isChinses = false, bool isAlphabet = false);
		//输入车牌字符的特征向量，送入ann_
		void classify(cv::Mat featureRows, std::vector<int>& out_maxIndexs,
			std::vector<float>& out_maxVals, std::vector<bool> isChineseVec);
		void classify(std::vector<CCharacter>& charVec);

		void classifyChinese(std::vector<CCharacter>& charVec);
		void classifyChineseGray(std::vector<CCharacter>& charVec);

		std::pair<std::string, std::string> identify(cv::Mat input, bool isChinese = false, bool isAlphabet = false);
		int identify(std::vector<cv::Mat> inputs, std::vector<std::pair<std::string, std::string>>& outputs,
			std::vector<bool> isChineseVec);

		std::pair<std::string, std::string> identifyChinese(cv::Mat input, float& result, bool& isChinese);
		std::pair<std::string, std::string> identifyChineseGray(cv::Mat input, float& result, bool& isChinese);

		bool isCharacter(cv::Mat input, std::string& label, float& maxVal, bool isChinese = false);

		void LoadModel(std::string path);
		void LoadChineseModel(std::string path);
		void LoadGrayChANN(std::string path);
		void LoadChineseMapping(std::string path);

	private:
		CharsIdentify();
		annCallback extractFeature;
		static CharsIdentify* instance_;

		// binary character classifer
		cv::Ptr<cv::ml::ANN_MLP> ann_;

		// binary character classifer, only for chinese
		cv::Ptr<cv::ml::ANN_MLP> annChinese_;

		// gray classifer, only for chinese
		cv::Ptr<cv::ml::ANN_MLP> annGray_;

		// used for chinese mapping
		std::shared_ptr<Kv> kv_;

		cv::Ptr<ml::SVM> svm_;
	};
}


