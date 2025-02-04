#include <cassert>

#include "./image_processing.h"
#include "./screen_shot.h"

namespace img_p =  image_processing;

void img_p::imgAroundCutRate(const cv::Mat &img_input, cv::Mat *const img_output, 
							 const float &x_rate, const float &y_rate, 
							 const float &w_rate, const float &h_rate)

{
	assert(x_rate + w_rate <= 1.0);
	assert(y_rate + h_rate <= 1.0);
	

	int cols = img_input.cols;
	int rows = img_input.rows;

	int x = static_cast<double>(cols * x_rate);
	int y = static_cast<double>(rows * y_rate);
	int width = static_cast<double>(cols * w_rate);
	int height = static_cast<double>(rows * h_rate);

	*img_output = img_input(cv::Rect(x, y, width, height));
}								

void img_p::img2Hist(const cv::Mat &img_, cv::MatND *const hist_output)
{
	int h_bins = 90; int s_bins = 128;
	int histSize[] = {h_bins, s_bins};

	float h_ranges[] = {0, 180};
	float s_ranges[] = {0, 256};

	const float* ranges[] = {h_ranges, s_ranges};

	// We use HSV images, In this case H, S are used.
	int channels[] = {0, 1};

	cv::calcHist(&img_, 1, channels, cv::Mat(), *hist_output, 2, 
								histSize, ranges, true, false);
	cv::normalize(*hist_output, *hist_output, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());
}

void img_p::splitImage(const cv::Mat &image, 
					   const int &col_num, const int &row_num, 
					   std::vector<cv::Mat> *const image_vec)
{
	size_t size = col_num * row_num;
	// initialize.
	assert(image_vec->size() == size);

	int cols = image.cols;
	int rows = image.rows;

	int split_cols = cols / col_num;
	int split_rows = rows / row_num;

	std::vector<cv::Rect> crop_rect_vec(size);

	/* Order of cutting.
	+=========+
	|4|8|12|16|
	|3|7|11|15|
	|2|6|10|14|
	|1|5|9 |13|
	+=========+
	*/
	for (int c = 0; c < col_num; ++c)
	{
		int cols_ = c * split_cols;
		int index_cols = c * row_num;
		for (int r = 0; r < row_num; ++r)
		{
			int rows_ = (row_num - (r + 1)) * split_rows;
			cv::Rect rec = cv::Rect(cols_, rows_, split_cols, split_rows);
			crop_rect_vec[index_cols + r] = rec;
		}
	}


	for (size_t i = 0; i < size; ++i)
	{
		cv::Mat cropped(image, crop_rect_vec[i]);
		(*image_vec)[i] = cropped;
	}	
}											

void img_p::toHDImg(cv::Mat *const img_)
{
	cv::resize(*img_, *img_, cv::Size(), 
						(static_cast<double>(pic::HD_WIDTH)  / img_->cols),
						(static_cast<double>(pic::HD_HEIGHT) / img_->rows));
}

void img_p::toHDImg(const cv::Mat &img_, cv::Mat *const img_output)
{
	cv::resize(img_, *img_output, cv::Size(), 
			   (static_cast<double>(pic::HD_WIDTH)  / img_.cols),
			   (static_cast<double>(pic::HD_HEIGHT) / img_.rows));
}
					
void img_p::sharpningKernel4(const cv::Mat &img_input, cv::Mat *const img_output)
{
	cv::filter2D(img_input, *img_output, img_input.depth(), img_p::sparpning_kernel_4_filter);
}

void img_p::sharpningKernel9(const cv::Mat &img_input, cv::Mat *const img_output)
{
	cv::filter2D(img_input, *img_output, img_input.depth(), img_p::sparpning_kernel_8_filter);
}

void img_p::opening(const cv::Mat &img_input, cv::Mat *const img_output, const cv::Mat &kernel, int iterations)
{
	cv::morphologyEx(img_input, *img_output, cv::MORPH_OPEN, kernel, cv::Point(-1, -1), iterations);
}

void img_p::closing(const cv::Mat &img_input, cv::Mat *const img_output, const cv::Mat &kernel, int iterations)
{
	cv::morphologyEx(img_input, *img_output, cv::MORPH_CLOSE, kernel, cv::Point(-1, -1), iterations);
}
