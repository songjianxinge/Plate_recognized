#include "stdafx.h"
#include <iostream>
#include <vector>
#include <opencv.hpp>
#include "plate_locate.h"
using namespace std;
using namespace easypr;
int test_plate_locate()
{
		cout << "test_plate_locate" << endl;
		const string file = "resources/image/plate_locate.jpg";
		cv::Mat src = imread(file);
		vector<cv::Mat> resultVec;
		CPlateLocate plate;

		int result = plate.plateLocate(src, resultVec);
		  if (result == 0) {
			size_t num = resultVec.size();
			for (size_t j = 0; j < num; j++) {
				cv::Mat resultMat = resultVec[j];
			    imshow("plate_locate", resultMat);
				waitKey(0);
		
			}
		  destroyWindow("plate_locate");
		
		}
		  return result;
}