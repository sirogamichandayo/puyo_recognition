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

void State::getState(const int &mode, std::vector<int> &field)
{
	// ぷよとかを受け取る。
	if (mode == get_mode::allPuyo_1p)
	{
		int size = 14 * 6 + 2 + 2;
		initializeField(&size, &field);

		std::vector<int> board(14*6);
		std::vector<int> next1(2);
		std::vector<int> next2(2);
		getBoard_1p(&board);
		getNext_1p(&next1);
		getNext2_1p(&next2);
		auto begin = field.begin();
		
		std::move(board.begin(), board.end(), begin);
		std::move(next1.begin(), next1.end(), begin+=board.size());
		std::move(next2.begin(), next2.end(), begin+=next1.size());

		return;
	}

	if (mode == get_mode::allPuyo_2p)
	{
		int size = 14 * 6 + 2;
		initializeField(&size, &field);
		
		std::vector<int> board(14*6);
		std::vector<int> next1(2);
		std::vector<int> next2(2);
		getBoard_2p(&board);
		getNext_2p(&next1);
		getNext2_2p(&next2);
		auto begin = field.begin();

		std::move(board.begin(), board.end(), begin);
		std::move(next1.begin(), next1.end(), begin+=board.size());
		std::move(next2.begin(), next2.end(), begin+=next1.size());

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
	if (color == puyo_color_list[0])
		return 4;
	if (color == puyo_color_list[1])
		return 5;
	if (color == puyo_color_list[2])
		return 6;
	if (color == puyo_color_list[3])
		return 7;

	//  Logger
	// Recognition miss.
	LOG("Recognition miss");
	return color::NONE;
}

void State::getNext_1p(std::vector<int> *field)
{
	int col_num = 1;
	int row_num = 2;
	if (!field->size())
		field->resize(col_num * row_num);

	cv::Mat img_next_1p(this->img, pic::next2_2p);
	std::vector<cv::Mat> img_next_1p_vec;
	splitImage(&img_next_1p, &col_num, &row_num, &img_next_1p_vec);
	// TODO
	int size = img_next_1p_vec.size();
	for (int i = 0; i < size; ++i)
		(*field)[i] = getColor(img_next_1p_vec[i]);
}

void State::getNext2_1p(std::vector<int> *field)
{
	int col_num = 1;
	int row_num = 2;
	if (!field->size())
		field->resize(col_num * row_num);

	// cut image
	cv::Mat img_next_2p(this->img, pic::next2_2p);
	std::vector<cv::Mat> img_next_2p_vec;
	splitImage(&img_next_2p, &col_num, &row_num, &img_next_2p_vec);
	// TODO
	int size = img_next_2p_vec.size();
	for (int i = 0; i < size; ++i)
		(*field)[i] = getColor(img_next_2p_vec[i]);
}

void State::getBoard_1p(std::vector<int> *field)
{
	int col_num = game::BOARD_COLS;
	int row_num = game::BOARD_ROWS_NO_IN_1314;
	if (!field->size())
		field->resize(col_num * row_num);

	cv::Mat img_board_1p(this->img, pic::board_1p);
	std::vector<cv::Mat> img_board_1p_vec;
	splitImage(&img_board_1p, &col_num, &row_num, &img_board_1p_vec);
	// TODO
	int size = img_board_1p_vec.size();
	for (int i = 0; i < size; ++i)
		(*field)[i] = getColor(img_board_1p_vec[i]);
}

void State::getNext_2p(std::vector<int> *field)
{
	int col_num = 1;
	int row_num = 2;
	if (!field->size())
		field->resize(col_num * row_num);

	cv::Mat img_next_2p(this->img, pic::next_2p);
	std::vector<cv::Mat> img_next_2p_vec;
	splitImage(&img_next_2p, &col_num, &row_num, &img_next_2p_vec);
	// TODO
	int size = img_next_2p_vec.size();
	for (int i = 0; i < size; ++i)
		(*field)[i] = getColor(img_next_2p_vec[i]);
}

void State::getNext2_2p(std::vector<int> *field)
{
	int col_num = 1;
	int row_num = 2;
	if (!field->size())
		field->resize(col_num * row_num);

	cv::Mat img_next2_2p(this->img, pic::next2_2p);
	std::vector<cv::Mat> img_next2_2p_vec;
	splitImage(&img_next2_2p, &col_num, &row_num, &img_next2_2p_vec);
	// TODO
	int size = img_next2_2p_vec.size();
	for (int i = 0; i < size; ++i)
		(*field)[i] = getColor(img_next2_2p_vec[i]);
}

void State::getBoard_2p(std::vector<int> *field)
{
	int col_num = 6;
	int row_num = 14;
	if (!field->size())
		field->resize(col_num * row_num);

	cv::Mat img_board_2p(this->img, pic::next2_2p);
	std::vector<cv::Mat> img_board_2p_vec;
	splitImage(&img_board_2p, &col_num, &row_num, &img_board_2p_vec);
	int size = img_board_2p_vec.size();
	for (int i = 0; i < size; ++i)
		(*field)[i] = getColor(img_board_2p_vec[i]);
}

int State::getColor(const cv::Mat &img)
{
	int color = toGetPuyoColorPerPiece(img);
	auto it = std::find(puyo_color_list.begin(), puyo_color_list.end(), color);
	// Just behind you, but this is becouse of speed
	if (!initColorList || it == puyo_color_list.end()) {
		puyo_color_list.push_back(color);
		if (puyo_color_list.size() == 4)
			initColorList = true;
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

void State::splitImage(const cv::Mat *const image, const int *const col_num, const int *const row_num, std::vector<cv::Mat> *const image_vec)
{

	int cols = image->cols;
	int rows = image->rows;

	int splitCols = cols / *col_num;
	int splitRows = rows / *row_num;

	std::vector<cv::Rect> p(*col_num * *row_num);

	/* Order of cutting.
	+=========+
	|4|8|12|16|
	|3|7|11|15|
	|2|6|10|14|
	|1|5|9 |13|
	+=========+
	*/
	for (int c = 0; c < *col_num; ++c)
	{
		int cols_ = c * splitCols;
		int index_cols = c * *row_num;
		for (int r = 0; r < *row_num; ++r)
		{
			int rows_ = (*row_num - (r + 1)) * splitRows;
			cv::Rect rec = cv::Rect(cols_, rows_, splitCols, splitRows);
			p[index_cols + r] = rec;
		}
	}

	// initialize.
	int size = *col_num * *row_num;
	initializeField(&size, image_vec);

	int index = 0;
	for (const auto &rec_ : p)
	{
		cv::Mat cropped(*image, rec_);
		(*image_vec)[index] = cropped;
		++index;
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

	saveColorAndImg(color_pixel_dict.begin(), color_pixel_dict.end(), image_padding);
	showForDebug(image_padding);
	
	std::pair<int, int> max_color = *std::max_element
		(color_pixel_dict.begin(), color_pixel_dict.end(),
    [](const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
			return p1.second < p2.second; 
		}
	);
	return max_color.first;
}
