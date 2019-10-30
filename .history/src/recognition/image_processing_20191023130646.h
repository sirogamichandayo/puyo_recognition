#ifndef _IMAGE_PROCESSING_H_
#define _IMAGE_PROCESSING_H_

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

namespace image_processing
{
	extern void paddingImg(const cv::Mat &img_, cv::Mat &img_pad, 
                      const float &x_rate, const float &y_rate, 
											const float &w_rate, const float &h_rate);
					
	extern void img2Hist(const cv::Mat &img_, cv::MatND *const hist_);

	extern void splitImage(const cv::Mat &image, 
											const int &col_num, const int &row_num, 
											std::vector<cv::Mat> *const image_vec);
	extern void toHDImg(cv::Mat *const img_);

	template<class elemType> // RGB, HSV
	extern void img2ElemPerPixelForVec(const cv::Mat &img_, 
																std::vector<elemType> &elem_vec)
	{
		int rows = img_.rows;
		int cols = img_.cols;
		elem_vec.resize(rows*cols);
		#pragma omp paralle for
		for (int y = 0; y < rows; ++y)
		{
			auto *p = &img_.at<cv::Vec3b>(y, 0);
			for (int x = 0; x < cols; ++x, ++p)
			{
				elem_vec[y*cols + x]((int)(*p)[0]*2, (int)(*p)[1], (int)(*p)[2]);
			}
		}
	}
}


#endif