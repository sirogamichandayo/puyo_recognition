#include "./state_main.h"

#include <vector>
#include <cassert>
#include <iostream>


bool State::isGetState(const int &mode)
{
	if (mode == get_mode::isFightEnd)
	{
		return isJudgeFightEnd();
	}

	// c++20
	if (/*[[likely]]*/ mode == get_mode::existNext_1p)
	{
		return isExistNext_1p();
	}

	if (/*[[likely]]*/ mode == get_mode::existNext_2p)
	{
		return isExistNext_2p();
	}

	////////////////////////////////////////
	LOG("No exist mode.");
	std::exit(0);
}

void State::getState(const int &mode, int &issue)
{
	// 勝敗条件
	if (mode == get_mode::battleResult)
	{
		getResult(&issue);
		return;
	}

	////////////////////////////////////////
	LOG("No exist mode.");
	std::exit(0);
}

void State::getState(const int &mode, 
		std::vector<int>::iterator first, std::vector<int>::iterator end)
{
	using namespace game;
	// ぷよとかを受け取る。
	if (mode == get_mode::allPuyo_1p)
	{
		int size = BOARD_ROWS_NO_IN_1314 * BOARD_COLS +
							 NEXT1_ROWS * NEXT1_COLS +
							 NEXT2_ROWS * NEXT2_COLS;
		if (size != std::distance(first, end))
		{
			LOG("Please pass Iterator of container of the corresponding size.\n\
					std::vector<game::BOARD_ROWS_NO_IN_1314*game::BOARD_COLS>    \n\
					is required.");
			std::exit(1);
		}

		// TODO: Waste reduction.
		auto it = first;
		getBoard_1p(it, std::step(it, BOARD_ROWS_NO_IN_1314*BOARD_COLS-1));
		std::advance(it, BOARD_ROWS_NO_IN_1314*BOARD_COLS);
		getNext_1p (it, std::step(it, NEXT1_ROWS*NEXT1_COLS-1));
		std::advance(it, NEXT1_ROWS*NEXT1_COLS);
		getNext2_1p(it, std::step(it, NEXT2_ROWS*NEXT2_COLS-1));

		// Those are code that to judge between "X" or not.
		// "X" is top of the third row;

		// When puyo don't exist under space.
		auto it_34 = step(first, 34);
		auto it_35 = step(it_34);
		if (*it_34 == color::NONE)
			*it_35 = color::NONE;

		// When don't exist color::RED in puyo_color_list.
		if (!isExistRedInColorList && *it_35 == color::RED)
			*it_35 = color::NONE;

		return;
	}

	if (mode == get_mode::allPuyo_2p)
	{
		int size = BOARD_ROWS_NO_IN_1314 * BOARD_COLS +
							 NEXT1_ROWS * NEXT1_COLS +
							 NEXT2_ROWS * NEXT2_COLS;
		if (size != std::distance(first, end))
		{
			LOG("Please pass Iterator of container of the corresponding size.\n\
					std::vector<game::BOARD_ROWS_NO_IN_1314*game::BOARD_COLS>    \n\
					is required.");
			std::exit(1);
		}

		auto it = first;
		getBoard_2p(it, std::step(it, BOARD_ROWS_NO_IN_1314*BOARD_COLS-1));
		std::advance(it, BOARD_ROWS_NO_IN_1314*BOARD_COLS);
		getNext_2p (it, std::step(it, NEXT1_ROWS*NEXT1_COLS-1));
		std::advance(it, NEXT1_ROWS*NEXT1_COLS);
		getNext2_2p(it, std::step(it, NEXT2_ROWS*NEXT2_COLS-1));

		// Those are code that to judge between "X" or not.
		// "X" is top of the third row;

		// When puyo don't exist under space.
		auto it_34 = step(first, 34);
		auto it_35 = step(it_34);
		if (*it_34 == color::NONE)
			*it_35 = color::NONE;

		// When don't exist color::RED in puyo_color_list.
		if (!isExistRedInColorList && *it_35 == color::RED)
			*it_35 = color::NONE;

		return;	
	}

	////////////////////////////////////////
	LOG("No exist mode.");
	std::exit(0);
}

////////////////////////////////////////////////////////////////////////
/* private */
////////////////////////////////////////////////////////////////////////
void State::toHDImg(cv::Mat *const img_)
{
	cv::resize(*img_, *img_, cv::Size(), 
						 (static_cast<double>(pic::HD_WIDTH)  / img_->cols),
						 (static_cast<double>(pic::HD_HEIGHT) / img_->rows));
}

void State::cutImg(cv::Mat *const img_)
{
	if (player == player::DEFAULT)
		return;
	for (const auto &[p_, rect_] : player_resize)
		if (player == p_)
		{
			*img_ = (*img_)(rect_);
			return;
		}
}

void State::paddingImg(const cv::Mat &img_, cv::Mat &img_pad, 
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

int State::colorNum2ForBitNum(int color)
{
	/*
		 Puyo real color has no measning.
		 So change real color number to number for bit operatorn.
		 */
	if (color == color::NONE || color == color::DIST)
		return color;
	else if (color == puyo_color_list[0])
		return 4;
	else if (color == puyo_color_list[1])
		return 5;
	else if (color == puyo_color_list[2])
		return 6;
	else if (color == puyo_color_list[3])
		return 7;

	//  Logger
	// Recognition miss.
	LOG("Recognition miss");
	return color::MISS;
}

void State::getNext_1p(std::vector<int>::iterator first,
											 std::vector<int>::iterator last)
{
	using cols = game::NEXT1_COLS;
	using rows = game::NEXT1_ROWS;
	int size = cols * rows;
	if (size != std::distance(first, last))
	{
		LOG("The distance of iterator is strange.");
		std::exit(1);
	}

	cv::Mat img_next_1p(this->img, pic::next2_2p);
	std::vector<cv::Mat> img_next_1p_vec(size);
	splitImage(&img_next_1p, cols, rows, 
	           img_next_1p_vec.begin(), img_next_1p_vec.end());

	for (int i = 0; i < size; ++i, ++first)
		*first = getColor(img_next_2p_vec[i]);
}

void State::getNext2_1p(std::vector<int>::iterator first, 
											  std::vector<int>::iterator last)
{
	using cols = game::NEXT2_COLS;
	using rows = game::NEXT2_ROWS;
	int size = cols * rows;
	if (size != std::distance(first, last))
	{
		LOG("The distance of iterator is strange.");
		std::exit(1);
	}

	cv::Mat img_next_2p(this->img, pic::next2_2p);
	std::vector<cv::Mat> img_next_2p_vec(size);
	splitImage(&img_next_2p, cols, rows,
						 img_next_2p_vec.begin(), img_next_2p_vec.end());
	
	for (int i = 0; i < size; ++i, ++first)
		*first = getColor(img_next_2p_vec[i]);
}

void State::getBoard_1p(std::vector<int>::iterator first,
												std::vector<int>::iterator last)
{
	using cols = game::BOARD_COLS;
	using rows = game::BOARD_ROWS_NO_IN_1314;
	int size = cols * rows;
	if (size != std::distance(first, last))
	{
		LOG("The distance of iterator is strange.");
		std::exit(1);
	}

	cv::Mat img_board_1p(this->img, pic::board_1p);
	std::vector<cv::Mat> img_board_1p_vec(size);
	splitImage(&img_board_1p, cols, rows,
						img_board_1p_vec.begin(), img_board_1p_vec.end());

	for (int i = 0; i < size; ++i, ++first)
		*first = getColor(img_board_1p_vec[i]);
}

void State::getNext_2p(std::vector<int>::iterator first,
											std::vector<int>::iterator last)
{
	using cols = game::NEXT2_COLS;
	using rows = game::NEXT2_ROWS;
	int size = cols * rows;
	if (size != std::distance(first, last))
	{
		LOG("The distance of iterator is strange.");
		std::exit(1);
	}

	cv::Mat img_next_2p(this->img, pic::next_2p);
	std::vector<cv::Mat> img_next_2p_vec(size);
	splitImage(&img_next_2p, cols, rows, 
						img_next_2p_vec.begin(), img_next_2p_vec.end());

	for (int i = 0; i < size; ++i, ++first)
		*first = getColor(img_next_2p_vec[i]);
}

void State::getNext2_2p(std::vector<int>::iterator first,
												std::vector<int>::iterator last)
{
	using cols = game::NEXT2_COLS;
	using rows = game::NEXT2_ROWS;
	int size = cols * rows;
	if (size != std::distance(first, last))
	{
		LOG("The distance of iterator is strange.");
		std::exit(1);
	}

	cv::Mat img_next2_2p(this->img, pic::next2_2p);
	std::vector<cv::Mat> img_next2_2p_vec(size);
	splitImage(&img_next2_2p, cols, rows, 
						img_next2_2p_vec.begin(), img_next2_2p_vec.end());

	for (int i = 0; i < size; ++i, ++first)
		*first = getColor(img_next2_2p_vec[i]);
}

void State::getBoard_2p(std::vector<int>::iterator first,
												std::vector<int>::iterator last)
{
	using cols = game::BOARD_COLS;
	using rows = game::BOARD_ROWS;
	int size = cols * rows;
	if (size != std::distance(first, last))
	{
		LOG("The distance of iterator is strange.");
		std::exit(1);
	}	

	cv::Mat img_board_2p(this->img, pic::next2_2p);
	std::vector<cv::Mat> img_board_2p_vec(size);
	splitImage(&img_board_2p, cols, rows, 
						img_board_2p_vec.begin(), img_board_2p_vec.end());

	for (int i = 0; i < size; ++i, ++first)
		*first = getColor(img_board_2p_vec[i]);
}

int State::getColor(const cv::Mat &img)
{
	int color = toGetPuyoColorPerPiece(img);
	auto it = std::find(puyo_color_list.begin(), puyo_color_list.end(), color);
	
	if (!initColorList && it == puyo_color_list.end()) {
		puyo_color_list.push_back(color);

		if (puyo_color_list.size() == color::PUYO_COLOR_NUM)
		{
			initColorList = true;

			auto it = std::find(puyo_color_list.begin(), puyo_color_list.end(), color::RED);
			if (it != puyo_color_list.end())
				isExistRedInColorList = true;
		}

		
	}
	return colorNum2ForBitNum(color);
}

bool State::isExistNext_1p()
{
	cv::Mat is_next_img_1p(this->img, pic::is_next_1p);

	return (color::NONE != toGetPuyoColorPerPiece(is_next_img_1p));
}

bool State::isExistNext_2p()
{
	cv::Mat is_next_img_2p(this->img, pic::is_next_2p);
	return (color::NONE != toGetPuyoColorPerPiece(is_next_img_2p));
}

void State::splitImage(const cv::Mat *const image, 
											const int &&col_num, const int &&row_num, 
											std::vector<cv::Mat>::iterator first, 
											std::vector<cv::Mat>::iterator last)
{
	int size = col_num * row_num;
	if (size != std::distance(first, last))
	{
		LOG("The distance of iterator is strange.");
		std::exit(1);
	}

	int cols = image->cols;
	int rows = image->rows;

	int splitCols = cols / col_num;
	int splitRows = rows / row_num;

	std::vector<cv::Rect> p(col_num * row_num);

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
		int cols_ = c * splitCols;
		int index_cols = c * row_num;
		for (int r = 0; r < row_num; ++r)
		{
			int rows_ = (row_num - (r + 1)) * splitRows;
			cv::Rect rec = cv::Rect(cols_, rows_, splitCols, splitRows);
			p[index_cols + r] = rec;
		}
	}

	for (int i = 0; i < size; ++i, ++first)
	{
		cv::Mat cropped(*image, rec_);
		*first = cropped;
	}
}

//  TODO: include constant(pic, )
// Judge between fight or not.
bool State::isJudgeFightEnd()
{
	HSV hsv;
	int yellow = 0;
	int green = 0;
	int other = 0;
	cv::Mat finish(this->img, pic::finish);
	cv::Mat resize_img;
	cv::resize(resize_img, finish, cv::Size(), 0.1, 0.1);

#pragma omp parallel for
	int rows = resize_img.rows;
	int cols = resize_img.cols;
	for (int y = 0; y < rows; y++)
	{
		cv::Vec3b *p = &resize_img.at<cv::Vec3b>(y, 0);
		for (int x = 0; x < cols; x++, p++)
		{
			hsv((int)(*p)[0]*2, (int)(*p)[1], (int)(*p)[2]);
			/*
		p	hsv.h = (int)(*p)[0] * 2; // H : 0 ~ 180 in Opencv. Change 0 ~ 360
			hsv.s = (int)(*p)[1];
			hsv.v = (int)(*p)[2];
			*/

			switch (hsv.toReadGreenYellow())
			{
			case color::YELLOW:
				yellow++;
				break;
			case color::GREEN:
				green++;
				break;
			case color::OTHER:
				other++;
				break;
			}
		}
	}
	/*
	cout << "####################" << endl;

	cout << "yellow : " << yellow << endl;
	cout << "green  : " << green  << endl;
	cout << "other  : " << other  << endl;
	*/
	if (((yellow > 1000) && (green > 500)) && (other < 500))
		return true; // finish.
	return false;	// not finish.
}

void State::getResult(int *const result)
{
	HSV hsv;
	int count_white_1p = 0;
	int count_white_2p = 0;
	int count_blue_1p = 0;
	int count_blue_2p = 0;
	int count_red_1p = 0;
	int count_red_2p = 0;
	int other = 0;

#pragma omp parallel for
	// 1p
	cv::Mat end_1p(this->img, pic::result_1p);
	cv::Mat resize_img;
	cv::resize(resize_img, end_1p, cv::Size(), 0.1, 0.1);
	int rows = resize_img.rows;
	int cols = resize_img.cols;
	for (int y = 0; y < rows; ++y)
	{
		cv::Vec3b *p = &resize_img.at<cv::Vec3b>(y, 0);
		for (int x = 0; x < cols; ++x, ++p)
		{
			hsv((int)(*p)[0]*2, (int)(*p)[1], (int)(*p)[2]);
			/*
			hsv.h = (int)(*p)[0] * 2; // H : 0 ~ 180 in Opencv. Change 0 ~ 360
			hsv.s = (int)(*p)[1];
			hsv.v = (int)(*p)[2];
			*/
			switch (hsv.toReadBlueRed())
			{
			case color::WHITE:
				++count_white_1p;
				break;
			case color::BLUE:
				++count_blue_1p;
				break;
			case color::RED:
				++count_red_1p;
				break;
			default:
				++other;
				break;
			}
		}
	}
// 2p
#pragma omp parallel for
	cv::Mat end_2p(img, pic::result_2p);
	cv::resize(resize_img, end_2p, cv::Size(), 0.1, 0.1);
	rows = resize_img.rows;
	cols = resize_img.cols;
	for (int y = 0; y < rows; ++y)
	{
		cv::Vec3b *p = end_2p.ptr<cv::Vec3b>(y, 0);
		for (int x = 0; x < cols; ++x, ++p)
		{
			hsv((int)(*p)[0]*2, (int)(*p)[1], (int)(*p)[2]);
			// cout << hsv.h << ", "<< hsv.s << ", " << hsv.v << endl; debug
			switch (hsv.toReadBlueRed())
			{
			case color::WHITE:
				++count_white_2p;
				break;
			case color::BLUE:
				++count_blue_2p;
				break;
			case color::RED:
				++count_red_2p;
				break;
			default:
				++other;
				break;
			}
		}
	}

	// debug
	// cout << "1p (<=)" << endl;
	// cout << "WHITE : " << count_white_1p << endl;
	// cout << "BLUE  : " << count_blue_1p << endl;
	// cout << "RED   : " << count_red_1p  << endl;
	// cout << "2p (=>)" << endl;
	// cout << "WHITE : " << count_white_2p << endl;
	// cout << "BLUE  : " << count_blue_2p << endl;
	// cout << "RED   : " << count_red_2p  << endl;
	// imageShow(end_1p);
	// imageShow(end_2p);
	if ((70 < count_white_1p && 70 < count_white_2p) && other < 25)
	{
		// 1p win.
		if ((count_blue_1p < 20 && 60 < count_red_1p) && (count_red_2p < 20 && 60 < count_blue_2p))
			*result = judge::WIN;
		else if ((count_red_1p < 20 && 60 < count_blue_1p) && (count_blue_2p < 20 && 60 < count_red_2p))
			*result = judge::LOSE;
		else if ((count_red_1p < 20 && 60 < count_blue_1p) && (count_red_2p < 20 && 60 < count_blue_2p)) // drow.
			*result = judge::DROW;
	}
	*result = judge::NONE;
}


int State::toGetPuyoColorPerPiece(const cv::Mat &image)
{
	// for yellow puyo.
	cv::Mat image_padding;
	// for exist next
	// paddingImg(image, image_padding, 0.1, 0.35, 0.85, 0.65);

	paddingImg(image, image_padding, 0.1, 0.1, 0.8, 0.8);
	
	// Receive image(piece puyo), and judge puyo color
	float size = 0.1;
	HSV hsv;

	std::map<int, int>
			color_pixel_dict = {
					{color::NONE, 0}, 
					{color::DIST, 0},
					{color::RED, 0},
					{color::BLUE, 0},
					{color::YELLOW, 0},
					{color::GREEN, 0},
					{color::PURPLE, 0}};
	cv::Mat resize_image;
	cv::resize(image_padding, resize_image, cv::Size(), size, size);

	int rows = resize_image.rows;
	int cols = resize_image.cols;
#pragma omp parallel for
	for (int y = 0; y < rows; ++y)
	{
		cv::Vec3b *p = &resize_image.at<cv::Vec3b>(y, 0);
		for (int x = 0; x < cols; ++x, ++p)
		{
			hsv((int)(*p)[0]*2, (int)(*p)[1], (int)(*p)[2]);
			/*
			hsv.h = (int)(*p)[0] * 2; H : 0~180 in opencv.
			hsv.s = (int)(*p)[1];
			hsv.v = (int)(*p)[2];
			*/

			++color_pixel_dict[hsv.toGetPixelPuyoColor()];
		}
	}
	// for is_exist_next
	// color_pixel_dict[color::NONE]*=2.9;

	/* debug
	saveColorAndImg(color_pixel_dict.begin(), color_pixel_dict.end(), image_padding);
	showForDebug(image_padding);
	*/
	
	std::pair<int, int> max_color = *std::max_element
		(color_pixel_dict.begin(), color_pixel_dict.end(),
    [](const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
			return p1.second < p2.second; 
		}
	);
	return max_color.first;
}
