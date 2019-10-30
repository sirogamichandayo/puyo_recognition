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

	if (mode == get_mode::isClear)
	{
		return isJudgeClear();
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
	// Can shorter?
	if (mode == get_mode::allPuyo_1p)
	{
		int size =  game::BOARD_COLS * game::BOARD_ROWS_NO_IN_1314 +
								game::NEXT1_COLS * game::NEXT1_ROWS +
								game::NEXT2_COLS * game::NEXT2_ROWS;
		initializeField(&size, &field);

		std::vector<int> board(game::BOARD_COLS*game::BOARD_ROWS_NO_IN_1314);
		std::vector<int> next1(game::NEXT1_COLS*game::NEXT1_ROWS);
		std::vector<int> next2(game::NEXT2_COLS*game::NEXT2_ROWS);

		// getPuyoColorSet(XXX, XXX, XXX, XXX, debug direcotory name);
		getPuyoColorSet(&board, game::BOARD_COLS, game::BOARD_ROWS_NO_IN_1314,
										pic::board_1p/*, "board_1p"*/);
		getPuyoColorSet(&next1, game::NEXT1_COLS, game::NEXT1_ROWS,
										pic::next_1p/*, "next1_1p"*/);
		getPuyoColorSet(&next2, game::NEXT2_COLS, game::NEXT2_ROWS,
										pic::next2_1p/*, "next2_1p"*/);
				
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
	else if (mode == get_mode::boardPuyo_1p)
	{
		int size = game::BOARD_COLS * game::BOARD_ROWS_NO_IN_1314;
		initializeField(&size, &field);
	
		std::vector<int> board(size);
		getPuyoColorSet(&board, game::BOARD_COLS, game::BOARD_ROWS_NO_IN_1314,
										pic::board_1p);
	}
	else if (mode == get_mode::boardPuyo_2p)
	{
		int size = game::BOARD_COLS * game::BOARD_ROWS_NO_IN_1314;
		initializeField(&size, &field);

		std::vector<int> board(size);
		getPuyoColorSet(&board, game::BOARD_COLS, game::BOARD_ROWS_NO_IN_1314,
										pic::board_2p);
	}
	else if (mode == get_mode::nextPuyo_1p)
	{
		int size = game::NEXT1_COLS * game::NEXT1_ROWS +
							 game::NEXT2_COLS * game::NEXT1_ROWS;
		initializeField(&size, &field);

		std::vector<int> next1(game::NEXT1_COLS * game::NEXT1_ROWS);
		std::vector<int> next2(game::NEXT2_COLS * game::NEXT2_ROWS);
		getPuyoColorSet(&next1, game::NEXT1_COLS, game::NEXT1_ROWS,
										pic::next_1p);
		getPuyoColorSet(&next2, game::NEXT2_COLS, game::NEXT2_ROWS,
										pic::next2_1p);

		auto begin = field.begin();
		std::move(next1.begin(), next1.end(), begin);
		std::move(next2.begin(), next2.end(), begin+=next1.size());
	}	
	// TODO: implement get_mode::nextPuyo_2p
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
		return;
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
		return;
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
void State::cutImg(cv::Mat *const img_) {
	*img_ = (*img_)(player_resize[player]);
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
		LOG("The size of \"field\" is strange. Initialize.");
		initializeField(&size, field);
	}

	cv::Mat img_(this->img, target_rect);
	std::vector<cv::Mat> img_split_vec(size);
	img_p::splitImage(img_, cols, rows, &img_split_vec);
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

	// Delete floating puyo.
	for (int i = 1; i < game::BOARD_COLS*game::BOARD_ROWS_NO_IN_1314; ++i)
	{
		if (i % game::BOARD_ROWS_NO_IN_1314 == 0) continue;
			
		if ((*field)[i-1] == color::NONE && (*field)[i] != color::NONE)
			(*field)[i] = color::NONE;
	}

	// This code that to judge between "X" or not.
	// "X" is top of the third row;
	if ((*field)[InfluenceImgX.first] == colorNum2ForBitNum(color::RED))
	{
		if (!isExistRedInColorList)
		{
			(*field)[InfluenceImgX.first] = color::NONE;
		}
		else
		{
			cv::Mat element(3, 3, CV_8U, cv::Scalar::all(255));
			cv::Mat diff_X_FD;
			cv::absdiff(img_split_vec[InfluenceImgX.first], InfluenceImgX.second, diff_X_FD);
			cv::Mat hsv_channels[3];
			cv::split(diff_X_FD, hsv_channels);
			cv::threshold(hsv_channels[2], hsv_channels[2], 100, 255, cv::THRESH_BINARY);
			img_p::paddingImg(hsv_channels[2], hsv_channels[2], 0.05, 0.05, 0.9, 0.9);
			cv::morphologyEx(hsv_channels[2], hsv_channels[2], cv::MORPH_CLOSE, element, cv::Point(-1, -1), 3);
			cv::resize(hsv_channels[2], hsv_channels[2], cv::Size(), 0.5, 0.5, cv::INTER_NEAREST);
			if (50 < cv::countNonZero(hsv_channels[2]))
				(*field)[InfluenceImgX.first] = color::NONE;
		}
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
				// use different between origina imgage and teacher image.
				// TODO: test
				// grayscale, edge extraction.
				std::map<int, int> for_judge_AD;
				cv::Mat diff;
				cv::absdiff(img_split_vec[index_AD], img_AD, diff);
				cv::resize(diff, diff, cv::Size(), 0.1, 0.1);
				int rows = diff.rows;
				int cols = diff.cols;
				#pragma omp parallel for
				for (int y = 0; y < rows; ++y)
				{
					cv::Vec3b *p = &diff.at<cv::Vec3b>(y, 0);
					for (int x = 0; x < cols; ++x, ++p)
					{
						int v = static_cast<int>((*p)[2]);
						++for_judge_AD[(v/40)*40];
					}
				}
				if (for_judge_AD[0] > 30)
				{
					(*field)[index_AD] = color::NONE;
				}
			}
		}
	}

	// Delete floating puyo.(again)
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

// Judge between fight or not.
bool State::isJudgeFightEnd()
{
	HSV hsv;
	int yellow = 0;
	int green = 0;
	int other = 0;
	cv::Mat finish(this->img, pic::finish);
	cv::resize(finish, finish, cv::Size(), 0.1, 0.1);

	int rows = finish.rows;
	int cols = finish.cols;
	#pragma omp parallel for
	for (int y = 0; y < rows; ++y)
	{
		cv::Vec3b *p = &finish.at<cv::Vec3b>(y, 0);
		for (int x = 0; x < cols; ++x, ++p)
		{
			hsv((int)(*p)[0]*2, (int)(*p)[1], (int)(*p)[2]);

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
// TODO: put together
  #pragma omp parallel for
	cv::Mat end_2p(img, pic::result_2p);
	cv::resize(resize_img, end_2p, cv::Size(), 0.1, 0.1);
	rows = resize_img.rows;
	cols = resize_img.cols;
	for (int y = 0; y < rows; ++y)
	{
		cv::Vec3b *p = &end_2p.at<cv::Vec3b>(y, 0);
		for (int x = 0; x < cols; ++x, ++p)
		{
			hsv((int)(*p)[0]*2, (int)(*p)[1], (int)(*p)[2]);
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
	cv::Mat img_;
	// for exist next
	if (is_exist_next)
		img_p::paddingImg(img_, img_, 0.1, 0.35, 0.8, 0.65);
	else
		img_p::paddingImg(img_, img_, 0.1, 0.1, 0.8, 0.8);
	
	// Receive image(piece puyo), and judge puyo color
	float size = 0.1;

	std::map<int, int>
			color_pixel_dict = {
					{color::NONE, 0}, 
					{color::DIST, 0},
					{color::RED, 0},
					{color::BLUE, 0},
					{color::YELLOW, 0},
					{color::GREEN, 0},
					{color::PURPLE, 0}};
	cv::resize(img_, img_, cv::Size(), size, size);
#if false
	int rows = resize_image.rows;
	int cols = resize_image.cols;
  #pragma omp parallel for
	for (int y = 0; y < rows; ++y)
	{
		cv::Vec3b *p = &resize_image.at<cv::Vec3b>(y, 0);
		for (int x = 0; x < cols; ++x, ++p)
		{
			hsv((int)(*p)[0]*2, (int)(*p)[1], (int)(*p)[2]);
			++color_pixel_dict[hsv.toGetPixelPuyoColor()];
		}
	}
#endif
	std::vector<std::vector<int>> elem_vec_vec;
	img_p::img2ElemPerPixelForVec(img_, elem_vec_vec,
  															/*elem_size*/3);
	HSV hsv;
	for (const auto &elem_vec : elem_vec_vec)
	{
		hsv(elem_vec[0]*2, elem_vec[1], elem_vec[2]);
		//  [h]            [s]          [v] 
		++color_pixel_dict[hsv.toGetPixelPuyoColor()];
	}

	if (is_exist_next)
	{
		color_pixel_dict[color::NONE]*=3.0;
		color_pixel_dict[color::PURPLE]*=2.0;
	}
	
	if (color_pixel_dict[color::DIST] >= 5)
		color_pixel_dict[color::DIST]-=5;

#if false
	/////////////////////////////////////////////////
	// DEBUG
	// save element count of color of puyo color per piece.
	std::string color_str;
	std::map<std::string, int> color_picel_dict_for_debug;
	for (const auto &[color_num, count] : color_pixel_dict)
	{
		colorNum2ColorString(color_num, &color_str);
		color_picel_dict_for_debug.insert(std::pair<std::string, int>
													(color_str, count));
	}
	debug::saveElem(color_picel_dict_for_debug.begin(), color_picel_dict_for_debug.end(), "color_elem");
	///////////////////////////////////////////
#endif

	std::pair<int, int> max_color = *std::max_element
		(color_pixel_dict.begin(), color_pixel_dict.end(),
    [](const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
			return p1.second < p2.second; 
		}
	);
	return max_color.first;
}

bool State::isJudgeClear()
{
	HSV hsv;
	int count_orange = 0;
	int count_other  = 0;

	cv::Mat clear_img(this->img, pic::clear);
	cv::resize(clear_img, clear_img, cv::Size(), 0.1, 0.1);
	int rows = clear_img.rows;
	int cols = clear_img.cols;
	#pragma omp parallel for
	for (int y = 0; y < rows; ++y)
	{
		cv::Vec3b *p = &clear_img.at<cv::Vec3b>(y, 0);
		for (int x = 0; x < cols; ++x, ++p)
		{
			hsv((int)(*p)[0]*2, (int)(*p)[1], (int)(*p)[2]);

			switch (hsv.toReadOrange())
			{
			case color::ORANGE:
				++count_orange;
				break;
			case color::OTHER:
				++count_other;
				break;
			}
		}
	}
	if (1100 < count_orange)
		return true;
	return false;
}