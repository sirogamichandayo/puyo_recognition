#ifndef _IMAGE_PROCESSING_H_
#define _IMAGE_PROCESSING_H_

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

namespace image_processing
{
	extern void imgAroundCutRate(const cv::Mat &img_input, cv::Mat *const img_output, 
								 const float &x_rate, const float &y_rate, 
								 const float &w_rate, const float &h_rate);
					
	extern void img2Hist(const cv::Mat &img_, cv::MatND *const hist_);

	extern void splitImage(const cv::Mat &image, 
						   const int &col_num, const int &row_num, 
						   std::vector<cv::Mat> *const image_vec);
	extern void toHDImg(cv::Mat *const img_);
	extern void toHDImg(const cv::Mat &img_, cv::Mat *const img_output);
}


#endif
