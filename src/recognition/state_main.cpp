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

#if __cplusplus > 201703L
	if (mode == get_mode::existNext_1p) [[likely]]
	{
		return isExistNext_1p();
	}

	if (mode == get_mode::existNext_2p) [[likely]]
	{
		return isExistNext_2p();
	}
#endif

#if __cplusplus <= 201703L
	if (mode == get_mode::existNext_1p)
	{
		return isExistNext_1p();
	}

	if (mode == get_mode::existNext_2p)
	{
		return isExistNext_2p();
	}

#endif	

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

void State::getState(const int &mode, std::vector<int> &field, bool isColorNum)
{
	if (mode == get_mode::allPuyo_1p)
	{
		int size =  game::BOARD_COLS * game::BOARD_ROWS_NO_IN_1314 +
								game::NEXT1_COLS * game::NEXT1_ROWS +
								game::NEXT2_COLS * game::NEXT2_ROWS;
		initializeField(&size, &field);

		std::vector<int> board(game::BOARD_COLS*game::BOARD_ROWS_NO_IN_1314);
		std::vector<int> next1(game::NEXT1_COLS*game::NEXT1_ROWS);
		std::vector<int> next2(game::NEXT2_COLS*game::NEXT2_ROWS);

		getPuyoColorSet(&board, game::BOARD_COLS, game::BOARD_ROWS_NO_IN_1314,
										pic::board_1p, "board_1p");
		getPuyoColorSet(&next1, game::NEXT1_COLS, game::NEXT1_ROWS,
										pic::next_1p, "next1_1p");
		getPuyoColorSet(&next2, game::NEXT2_COLS, game::NEXT2_ROWS,
										pic::next2_1p, "next2_1p");
				
		auto begin = field.begin();
		std::move(board.begin(), board.end(), begin);
		std::move(next1.begin(), next1.end(), begin+=board.size());
		std::move(next2.begin(), next2.end(), begin+=next1.size());

	}
	else if (mode == get_mode::allPuyo_2p)
	{
	  int size =  game::BOARD_COLS * game::BOARD_ROWS_NO_IN_1314 +
								game::NEXT1_COLS * game::NEXT1_ROWS +
								game::NEXT2_COLS * game::NEXT2_ROWS;
		initializeField(&size, &field);
		
		std::vector<int> board(game::BOARD_COLS*game::BOARD_ROWS_NO_IN_1314);
		std::vector<int> next1(game::NEXT1_COLS*game::NEXT2_ROWS);
		std::vector<int> next2(game::NEXT2_COLS*game::NEXT2_ROWS);
		getPuyoColorSet(&board, game::BOARD_COLS, game::BOARD_ROWS_NO_IN_1314,
										pic::board_2p);
		getPuyoColorSet(&next1, game::NEXT1_COLS, game::NEXT1_ROWS,
										pic::next_2p);
		getPuyoColorSet(&next2, game::NEXT2_COLS, game::NEXT2_ROWS,
									  pic::next2_2p);
		
		auto begin = field.begin();
		std::move(board.begin(), board.end(), begin);
		std::move(next1.begin(), next1.end(), begin+=board.size());
		std::move(next2.begin(), next2.end(), begin+=next1.size());

	}
	 else 
	{
	////////////////////////////////////////
	LOG("No exist mode.");
	std::exit(0);
	}

	if (isColorNum)
		bitNum2ColorNumForVec(&field);
}

void State::bitNum2ColorNumForVec(std::vector<int> *const field)
{
	if (!initColorList)
	{
		LOG("Not initialized initColorList in State Class.");
		std::exit(1);
	}
	for (auto& elem : *field)
	{
		elem = bitNum2ColorNum(elem);
	}
}

void State::colorNum2bitNumForVec(std::vector<int> *const field)
{
	if (!initColorList)
	{
		LOG("Not initialized initColorList in State Class.");
		std::exit(1);
	}
	for (auto elem : *field)
	{
		elem = colorNum2ForBitNum(elem);
	}
}

void State::colorNum2ColorStringForVec(const std::vector<int> &field_int, 
																	std::vector<std::string> *const field_str)
{
	int size = field_int.size();
	initializeField(&size, field_str);
	for (int i = 0; i < size; ++i)
		colorNum2ColorString(field_int[i], &((*field_str)[i]));
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
	// TODO: change range for to find key.
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

void State::img2Hist(const cv::Mat &img_, cv::MatND *const hist_)
{
	cv::Mat image;
	paddingImg(img_, image, 0.1, 0.1, 0.8, 0.8);
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

int State::colorNum2ForBitNum(const int &color)
{
	/*
		 Puyo real color has no measning.
		 So change real color number to number for bit operatorn.
		 */
	if (color == color::NONE || color == color::DIST)
		return color;
	else if (color == puyo_color_list[0])
		return 0b100;
	else if (color == puyo_color_list[1])
		return 0b101;
	else if (color == puyo_color_list[2])
		return 0b110;
	else if (color == puyo_color_list[3])
		return 0b111;

	//  Logger
	// Recognition miss.
	LOG("Recognition miss");
	return color::MISS;
}

int State::bitNum2ColorNum(const int &color)
{
	if (color == color::NONE || color == color::DIST)
		return color;
	else if (color == 0b100)
		return puyo_color_list[0];
	else if (color == 0b101)
		return puyo_color_list[1];
	else if (color == 0b110)
		return puyo_color_list[2];
	else if (color == 0b111)
		return puyo_color_list[3];
	
	LOG("Change miss bit number to color number.");
	return color::MISS;
}

void State::colorNum2ColorString(const int &color, std::string *const str)
{
	switch(color)
	{
	case color::NONE:
		*str = "NONE";
		break;
	case color::DIST:
		*str = "DIST";
		break;
	case color::WALL:
		*str = "WALL";
		break;
	case color::IRON:
		*str = "IRON";
		break;
	case color::RED:
		*str = "RED";
		break;
	case color::BLUE:
		*str = "BLUE";
		break;
	case color::YELLOW:
		*str = "YELLOW";
		break;
	case color::GREEN:
		*str = "GREEN";
		break;
	case color::PURPLE:
		*str = "PURPLE";
		break;
	case color::WHITE:
		*str = "WHITE";
		break;
	case color::OTHER:
		*str = "OTHER";
		break;
	default:
		*str = "MISS";
		break;
	}
}

void State::getPuyoColorSet(std::vector<int> *const field, 
												const int& cols, const int& rows,
												const cv::Rect &target_rect,
												const std::string &dir_path)
{
	int size = cols * rows;
	if (size != field->size())
	{
		LOG("The size of \"field\" is strange.");
		std::exit(1);
	}

	cv::Mat img_(this->img, target_rect);
	std::vector<cv::Mat> img_split_vec(size);
	splitImage(img_, cols, rows, &img_split_vec);
	if (dir_path != "")
	{
		//////////////////////////////
		// DEBUG
		
		// save puyo image per piece
		std::map<std::string, cv::Mat> img_split_vec_for_debug;
		for (int i = 0; i < size; ++i)
		{
			std::string file_path_split = "puyo" + std::to_string(i); // for debug
			img_split_vec_for_debug.insert(std::pair<std::string, cv::Mat>(file_path_split, img_split_vec[i]));
		}
		debug::saveImg(img_split_vec_for_debug.begin(), img_split_vec_for_debug.end(), dir_path, true);
		/////////////////////////////////
	}

	for (int i = 0; i < size; ++i)
		(*field)[i] = getColor(img_split_vec[i]);

	if (size != game::BOARD_COLS * game::BOARD_ROWS_NO_IN_1314)
		return;

	complementPuyoColorSet(field, img_split_vec, size);
}	

void State::complementPuyoColorSet(std::vector<int> *const field, 
																		const std::vector<cv::Mat> &img_split_vec, 
																		const int &size)
	{
		// Recognition adjustment of board.

	// don't think about floating the color other than color::NONE.
	for (int i = 1; i < game::BOARD_COLS*game::BOARD_ROWS_NO_IN_1314; ++i)
	{
		if (i % 12 == 0) continue;
			
		if ((*field)[i-1] == color::NONE && (*field)[i] != color::NONE)
			(*field)[i] = color::NONE;
	}

	// This code that to judge between "X" or not.
	// "X" is top of the third row;
	if ((*field)[InfluenceHistX.first] == colorNum2ForBitNum(color::RED))
	{
		if (!isExistRedInColorList)
		{
			(*field)[InfluenceHistX.first] = color::NONE;
		}
		else
		{
			cv::MatND hist_X;
			img2Hist(img_split_vec[InfluenceHistX.first], &hist_X);
			double similar = cv::compareHist(hist_X, InfluenceHistX.second, 0);
			if (similar > 0.3)
			{
				(*field)[InfluenceHistX.first] = color::NONE;
			}
		}
	}


	LOG("DEGUB for AD");
	// Iterator can be anything.
	std::map<std::string, int> V_for_debug;
	debug::saveElem(V_for_debug.begin(), V_for_debug.end(), "", true);
	for (const auto &[index_AD, img_AD] : InfluenceImgAllDelete)
	{
		V_for_debug.clear();
		cv::Mat diff;
		cv::absdiff(img_split_vec[index_AD], img_AD, diff);
		cv::resize(diff, diff, cv::Size(), 0.1, 0.1);
		int rows = diff.rows;
		int cols = diff.cols;
		for (int y = 0; y < rows; ++y)
		{
			cv::Vec3b *p = &diff.at<cv::Vec3b>(y, 0);
			for (int x = 0; x < cols; ++x, ++p)
			{
				int v = static_cast<int>((*p)[2]);
				++V_for_debug[std::to_string((v/20)*20)];
			}
		}
		/*
		std::vector<cv::Mat> diff_channels;
		cv::split(diff, diff_channels);
		double max_v, min_v;
		cv::minMaxLoc(diff_channels[2], &min_v, &max_v, nullptr, nullptr);
		int mean_v;
		mean_v = (int)cv::mean(diff_channels[2])[0];
		std::cout << index_AD << "(mean, max, min) -> ("  << mean_v << ", " << max_v << ", " << min_v << ")" << std::endl;
		*/
		debug::saveElem(V_for_debug.begin(), V_for_debug.end(), "AD_H");
	}

	// this code that to judge between "all delete" or not,
	for (const auto &[index_AD, img_AD]: InfluenceImgAllDelete)		
	{
			
		if ((*field)[index_AD] == colorNum2ForBitNum(color::YELLOW))
		{
			if (!isExistYellowInColorList)
			{
				(*field)[index_AD] = color::NONE;
			}
			else
			{
				cv::Mat diff;
				cv::absdiff(img_split_vec[index_AD], img_AD, diff);
				std::vector<cv::Mat> diff_channels;
				cv::split(diff, diff_channels);
				double max_v;
				cv::minMaxLoc(diff_channels[2], nullptr, &max_v, nullptr, nullptr);
				if (100 > max_v)
				{
					(*field)[index_AD] = color::NONE;
				}
			}
		}
		/*
		if (((*field)[index_AD] == colorNum2ForBitNum(color::YELLOW) && \
				isExistYellowInColorList) || \
				(*field)[index_AD] == color::DIST)
		{
			continue;
		}
		*/
	}

	// don't think about floating the color other than color::NONE. (again)
	for (int i = 1; i < game::BOARD_COLS*game::BOARD_ROWS_NO_IN_1314; ++i)
	{
		if (i % game::BOARD_ROWS_NO_IN_1314 == 0) continue;
			
		if ((*field)[i-1] == color::NONE && (*field)[i] != color::NONE)
			(*field)[i] = color::NONE;
	}

	}


int State::getColor(const cv::Mat &img)
{
	int color = toGetPuyoColorPerPiece(img);

	// throw
	if (color == color::NONE || color == color::DIST)
		return color;
	
	auto it = std::find(puyo_color_list.begin(), puyo_color_list.end(), color);
	
	if (!initColorList && it == puyo_color_list.end()) {
		puyo_color_list.push_back(color);

		if (puyo_color_list.size() == color::PUYO_COLOR_NUM)
		{
			initColorList = true;
			auto it_red = std::find(puyo_color_list.begin(), puyo_color_list.end(), color::RED);
			if (it_red != puyo_color_list.end())
				isExistRedInColorList = true;
			auto it_yellow = std::find(puyo_color_list.begin(), puyo_color_list.end(), color::YELLOW);
			if (it_yellow != puyo_color_list.end())
				isExistYellowInColorList = true;
		}
	}
	return colorNum2ForBitNum(color);
}

bool State::isExistNext_1p()
{
	cv::Mat is_next_img_1p(this->img, pic::is_next_1p);
	return (color::NONE != toGetPuyoColorPerPiece(is_next_img_1p, true));
}

bool State::isExistNext_2p()
{
	cv::Mat is_next_img_2p(this->img, pic::is_next_2p);
	return (color::NONE != toGetPuyoColorPerPiece(is_next_img_2p, true));
}

void State::splitImage(const cv::Mat &image, 
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

	// initialize.
	initializeField(&size, image_vec);

	for (int i = 0; i < size; ++i)
	{
		cv::Mat cropped(image, crop_vec[i]);
		(*image_vec)[i] = cropped;
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
	for (int y = 0; y < rows; ++y)
	{
		cv::Vec3b *p = &resize_img.at<cv::Vec3b>(y, 0);
		for (int x = 0; x < cols; ++x, ++p)
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
				++yellow;
				break;
			case color::GREEN:
				++green;
				break;
			case color::OTHER:
				++other;
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


int State::toGetPuyoColorPerPiece(const cv::Mat &image, bool is_exist_next)
{
	// for yellow puyo.
	cv::Mat image_padding;
	// for exist next
	if (is_exist_next)
		paddingImg(image, image_padding, 0.1, 0.35, 0.8, 0.65);
	else
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
	if (is_exist_next)
		color_pixel_dict[color::NONE]*=3.0;

	// for puyo eye.
	if (color_pixel_dict[color::NONE] >= 5)
		color_pixel_dict[color::NONE]-=5;

	/////////////////////////////////////////////////
	// DEBUG
	// save element count of color of puyo color per piece.
	std::map<std::string, int> color_picel_dict_for_debug;
	for (const auto &[color_num, count] : color_pixel_dict)
	{
		std::string color_str;
		colorNum2ColorString(color_num, &color_str);
		color_picel_dict_for_debug.insert(std::pair<std::string, int>
													(color_str, count));
	}
	debug::saveElem(color_picel_dict_for_debug.begin(), color_picel_dict_for_debug.end(), "color_elem");
	

	std::pair<int, int> max_color = *std::max_element
		(color_pixel_dict.begin(), color_pixel_dict.end(),
    [](const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
			return p1.second < p2.second; 
		}
	);
	return max_color.first;
}
