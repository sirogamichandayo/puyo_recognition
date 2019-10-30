#include "./image_processing.h"
#include "./screen_shot.h"

namespace img_p =  image_processing;

void img_p::paddingImg(const cv::Mat &img_, cv::Mat &img_pad, 
                const float &x_rate, const float &y_rate, 
								const float &w_rate, const float &h_rate)
{
	if (x_rate + w_rate > 1.0 ||
			y_rate + h_rate > 1.0)
	{
		LOG("Out of range");
		std::exit(0);
	}

	int cols = img_.cols;
	int rows = img_.rows;

	int x = cols * x_rate;
	int y = rows * y_rate;
	int width = cols * w_rate;
	int height = rows * h_rate;
	cv::Rect rect = cv::Rect(x, y, width, height);

	img_pad = img_(rect);
}								

void img_p::img2Hist(const cv::Mat &img_, cv::MatND *const hist_)
{
	cv::Mat image;
	img_p::paddingImg(img_, image, 0.1, 0.1, 0.8, 0.8);
	// When judgint the color::YELLOW or backgraund "zenkasi", 
	// sensitive to surrounding background.
	// So I make padding size if bigger.

	// cv::resize(image, image, cv::Size(), 0.2, 0.2);
	
	int h_bins = 90; int s_bins = 128;
	int histSize[] = {h_bins, s_bins};

	float h_ranges[] = {0, 180};
	float s_ranges[] = {0, 256};

	const float* ranges[] = {h_ranges, s_ranges};

	// We use HSV images, In this case H, S are used.
	int channels[] = {0, 1};

	cv::calcHist(&image, 1, channels, cv::Mat(), *hist_, 2, 
								histSize, ranges, true, false);
	cv::normalize(*hist_, *hist_, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());
}

void img_p::splitImage(const cv::Mat &image, 
											const int &col_num, const int &row_num, 
											std::vector<cv::Mat> *const image_vec)
{
	int size = col_num * row_num;
	int cols = image.cols;
	int rows = image.rows;

	int split_cols = cols / col_num;
	int split_rows = rows / row_num;

	std::vector<cv::Rect> crop_vec(size);

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
			crop_vec[index_cols + r] = rec;
		}
	}

	// initialize.(memory waste!!)
#if false
	if (image_vec->size() != size)
	{
		std::vector<cv::Mat> ().swap(*image_vec);
		image_vec->resize(size);
	}
	else
	{
		image_vec->clear();
	}
#endif	

	for (int i = 0; i < size; ++i)
	{
		cv::Mat cropped(image, crop_vec[i]);
		(*image_vec)[i] = cropped;
	}	
}											

void img_p::toHDImg(cv::Mat *const img_)
{
	cv::resize(*img_, *img_, cv::Size(), 
						(static_cast<double>(pic::HD_WIDTH)  / img_->cols),
						(static_cast<double>(pic::HD_HEIGHT) / img_->rows));
}