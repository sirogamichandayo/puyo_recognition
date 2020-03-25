#define DEBUG_PUYO_COLOR 1
#define DEBUG_YELLOW 1

#include "./state_main.h"

#include <vector>
#include <cassert>
#include <iostream>


bool State::isGetState(const int &mode)
{
	if (mode == get_mode::IS_FIGHT_END)
	{
		return isJudgeFightEnd();
	} 

	if (mode == get_mode::IS_CLEAR)
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
	if (mode == get_mode::IS_EXIST_NEXT_1P)
	{
		return isExistNext_1p();
	}

	if (mode == get_mode::IS_EXIST_NEXT_1P)
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
	// is win
	if (mode == get_mode::BATTLE_RESULT)
	{
		getResult(&issue);
		return;
	}

	////////////////////////////////////////
	LOG("No exist mode.");
	std::exit(0);
}

void State::getState(const int &mode, std::vector<int> *const field, bool isColorNum)
{
	// Can shorter?
	if (mode == get_mode::ALL_PUYO_1P)
	{
		size_t size =  game::BOARD_COLS * game::BOARD_ROWS_NO_IN_1314 +
			game::NEXT1_COLS * game::NEXT1_ROWS +
			game::NEXT2_COLS * game::NEXT2_ROWS;
		assert(field->size() == size);

		std::vector<int> board(game::BOARD_COLS*game::BOARD_ROWS_NO_IN_1314);
		std::vector<int> next1(game::NEXT1_COLS*game::NEXT1_ROWS);
		std::vector<int> next2(game::NEXT2_COLS*game::NEXT2_ROWS);

		// getPuyoColorSet(XXX, XXX, XXX, XXX, debug direcotory name);
		getPuyoColorSet(&board, game::BOARD_COLS, game::BOARD_ROWS_NO_IN_1314,
						(*_pic_rect_list)[pic::BOARD_1P_RECT_I],"board_1p");
		getPuyoColorSet(&next1, game::NEXT1_COLS, game::NEXT1_ROWS,
						(*_pic_rect_list)[pic::NEXT1_1P_RECT_I]/*, "next1_1p"*/);
		getPuyoColorSet(&next2, game::NEXT2_COLS, game::NEXT2_ROWS,
						(*_pic_rect_list)[pic::NEXT2_1P_RECT_I]/*, "next2_1p"*/);

		auto begin = field->begin();
		std::move(board.begin(), board.end(), begin);
		std::move(next1.begin(), next1.end(), begin+=board.size());
		std::move(next2.begin(), next2.end(), begin+=next1.size());

	}
	else if (mode == get_mode::ALL_PUYO_2P)
	{
	  size_t size =  game::BOARD_COLS * game::BOARD_ROWS_NO_IN_1314 +
		  game::NEXT1_COLS * game::NEXT1_ROWS +
		  game::NEXT2_COLS * game::NEXT2_ROWS;
	  assert(field->size() == size);
		
	  std::vector<int> board(game::BOARD_COLS*game::BOARD_ROWS_NO_IN_1314);
	  std::vector<int> next1(game::NEXT1_COLS*game::NEXT2_ROWS);
	  std::vector<int> next2(game::NEXT2_COLS*game::NEXT2_ROWS);
	  getPuyoColorSet(&board, game::BOARD_COLS, game::BOARD_ROWS_NO_IN_1314,
					  (*_pic_rect_list)[pic::BOARD_2P_RECT_I]);
	  getPuyoColorSet(&next1, game::NEXT1_COLS, game::NEXT1_ROWS,
					  (*_pic_rect_list)[pic::NEXT1_2P_RECT_I]);
	  getPuyoColorSet(&next2, game::NEXT2_COLS, game::NEXT2_ROWS,
					  (*_pic_rect_list)[pic::NEXT2_2P_RECT_I]);
		
	  auto begin = field->begin();
	  std::move(board.begin(), board.end(), begin);
	  std::move(next1.begin(), next1.end(), begin+=board.size());
	  std::move(next2.begin(), next2.end(), begin+=next1.size());

	}
	else if (mode == get_mode::BOARD_PUYO_1P)
	{
		size_t size = game::BOARD_COLS * game::BOARD_ROWS_NO_IN_1314;
		assert(field->size() == size);
	
		std::vector<int> board(size);
		getPuyoColorSet(&board, game::BOARD_COLS, game::BOARD_ROWS_NO_IN_1314,
						(*_pic_rect_list)[pic::BOARD_1P_RECT_I]);

		auto begin = field->begin();
		std::move(board.begin(), board.end(), begin);
	}
	else if (mode == get_mode::BOARD_PUYO_2P)
	{
		size_t size = game::BOARD_COLS * game::BOARD_ROWS_NO_IN_1314;
		assert(field->size() == size);

		std::vector<int> board(size);
		getPuyoColorSet(&board, game::BOARD_COLS, game::BOARD_ROWS_NO_IN_1314,
						(*_pic_rect_list)[pic::BOARD_2P_RECT_I]);

		auto begin = field->begin();
		std::move(board.begin(), board.end(), begin);
	}
	else if (mode == get_mode::NEXT_PUYO_1P)
	{
		size_t size = game::NEXT1_COLS * game::NEXT1_ROWS +
			game::NEXT2_COLS * game::NEXT1_ROWS;
		assert(field->size() == size);

		std::vector<int> next1(game::NEXT1_COLS * game::NEXT1_ROWS);
		std::vector<int> next2(game::NEXT2_COLS * game::NEXT2_ROWS);
		getPuyoColorSet(&next1, game::NEXT1_COLS, game::NEXT1_ROWS,
						(*_pic_rect_list)[pic::NEXT1_1P_RECT_I]);
		getPuyoColorSet(&next2, game::NEXT2_COLS, game::NEXT2_ROWS,
						(*_pic_rect_list)[pic::NEXT2_1P_RECT_I]);
		auto begin = field->begin();
		std::move(next1.begin(), next1.end(), begin);
		std::move(next2.begin(), next2.end(), begin+=next1.size());
	}
	else if (mode == get_mode::NEXT_PUYO_2P)
	{
		size_t size = game::NEXT2_COLS * game::NEXT2_COLS +
			game::NEXT2_COLS + game::NEXT1_ROWS;
		assert(field->size() == size);

		std::vector<int> next1(game::NEXT1_COLS * game::NEXT1_ROWS);
		std::vector<int> next2(game::NEXT2_COLS * game::NEXT2_ROWS);
		getPuyoColorSet(&next1, game::NEXT1_COLS, game::NEXT1_ROWS, (*_pic_rect_list)[pic::NEXT1_2P_RECT_I]);
		getPuyoColorSet(&next1, game::NEXT2_COLS, game::NEXT2_ROWS, (*_pic_rect_list)[pic::NEXT2_2P_RECT_I]);
	}
	else
	{
		////////////////////////////////////////
		LOG("No exist mode.");
		return;
	}

	if (isColorNum)
		bitNum2ColorNumForVec(field);
}

void State::bitNum2ColorNumForVec(const std::vector<int> &input_puyo_field,
								  std::vector<int> *const output_puyo_field)
{
	assert(input_puyo_field.size() == output_puyo_field->size());
	if (!_initColorList)
	{
		LOG("Not initialized _initColorList in State Class.");
		return;
	}
	auto input_begin = input_puyo_field.begin();
	auto input_end   = input_puyo_field.end();
	auto output_begin = output_puyo_field->begin();
	auto output_end   = output_puyo_field->end();

	for (; input_begin != input_end && output_begin != output_end; ++input_begin, ++output_begin)
	{
		*output_begin = bitNum2ColorNum(*input_begin);
	}
}

void State::bitNum2ColorNumForVec(std::vector<int> *const puyo_field)
{
	if (!_initColorList)
	{
		LOG("Not initialized initColorList in State Class.");
		return;
	}
	for (auto& elem : *puyo_field)
	{
		elem = bitNum2ColorNum(elem);
	}
}

void State::colorNum2BitNumForVec(const std::vector<int> &input_puyo_field,
								  std::vector<int> *const output_puyo_field)
{
	assert(input_puyo_field.size() == output_puyo_field->size());
	if (!_initColorList)
	{
		LOG("Not initialized _initColorList in State Class.");
		return;
	}
	auto input_begin = input_puyo_field.begin();
	auto input_end   = input_puyo_field.end();
	auto output_begin = output_puyo_field->begin();
	auto output_end   = output_puyo_field->end();

	for (; input_begin != input_end && output_begin != output_end; ++input_begin, ++output_begin)
	{
		*output_begin = colorNum2BitNum(*input_begin);
	}
}


void State::colorNum2BitNumForVec(std::vector<int> *const puyo_field)
{
	if (!_initColorList)
	{
		LOG("Not initialized initColorList in State Class.");
		return;
	}
	for (auto elem : *puyo_field)
	{
		elem = colorNum2BitNum(elem);
	}
}

void State::bitNum2ColorStringForVec(const std::vector<int> &input_puyo_field,
									 std::vector<std::string> *const output_puyo_field)
{
	assert(input_puyo_field.size() == output_puyo_field->size());
	if (!_initColorList)
	{
		LOG("Not initialized _initColorList in State Class.");
		return;
	}
	auto input_begin = input_puyo_field.begin();
	auto input_end   = input_puyo_field.end();
	auto output_begin = output_puyo_field->begin();
	auto output_end   = output_puyo_field->end();

	for (; input_begin != input_end && output_begin != output_end; ++input_begin, ++output_begin)
	{
		bitNum2ColorString(*input_begin, &(*output_begin));
	}	
}

void State::colorNum2ColorStringForVec(const std::vector<int> &input_puyo_field, 
									   std::vector<std::string> *const output_puyo_field)
{
	assert(input_puyo_field.size() == output_puyo_field->size());
	if (!_initColorList)
	{
		LOG("Not initialized _initColorList in State Class.");
		return;
	}
	auto input_begin = input_puyo_field.begin();
	auto input_end   = input_puyo_field.end();
	auto output_begin = output_puyo_field->begin();
	auto output_end   = output_puyo_field->end();

	for (; input_begin != input_end && output_begin != output_end; ++input_begin, ++output_begin)
	{
		colorNum2ColorString(*input_begin, &(*output_begin));
	}		
}


////////////////////////////////////////////////////////////////////////
/* private */
////////////////////////////////////////////////////////////////////////

int State::colorNum2BitNum(const int &color)
{
	/*
	  Puyo real color has no measning.
	  So change real color number to number for bit operatorn.
	*/
	if (color == color::NONE || color == color::DIST)
		return color;
	else if (color == _puyo_color_list[0])
		return 0b100;
	else if (color == _puyo_color_list[1])
		return 0b101;
	else if (color == _puyo_color_list[2])
		return 0b110;
	else if (color == _puyo_color_list[3])
		return 0b111;

	return color::MISS;
}

int State::bitNum2ColorNum(const int &color)
{
	if (color == color::NONE || color == color::DIST)
		return color;
	else if (color == 0b100)
		return _puyo_color_list[0];
	else if (color == 0b101)
		return _puyo_color_list[1];
	else if (color == 0b110)
		return _puyo_color_list[2];
	else if (color == 0b111)
		return _puyo_color_list[3];
	
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

void State::bitNum2ColorString(const int& bit_num, std::string *const str)
{
	int color_num = bitNum2ColorNum(bit_num);
	colorNum2ColorString(color_num, str);
}


void State::getPuyoColorSet(std::vector<int> *const field, 
							const int& cols, const int& rows,
							const cv::Rect &target_rect,
							const std::string &dir_path)
{
	size_t size = cols * rows;
	assert(field->size() == size);
	
	cv::Mat img_(_img, target_rect);
	std::vector<cv::Mat> img_split_vec(size);

	img_p::splitImage(img_, cols, rows, &img_split_vec);
	
#if DEBUG_PUYO_COLOR
	if (dir_path != "")
	{
		//////////////////////////////
		// DEBUG
		
		// save puyo image per piece
		std::map<std::string, cv::Mat> img_split_vec_for_debug;
		for (size_t i = 0; i < size; ++i)
		{
			std::string file_path_split = "puyo" + std::to_string(i+1); // for debug
			img_split_vec_for_debug.insert(std::pair<std::string, cv::Mat>(file_path_split, img_split_vec[i]));
		}
		debug::saveImg(img_split_vec_for_debug.begin(), img_split_vec_for_debug.end(), dir_path, /*is_hsv*/true, /*is_overwrite*/true);
		/////////////////////////////////
	}
#endif

	for (size_t i = 0; i < size; ++i)
		(*field)[i] = getPuyoBitNumPerPiece(img_split_vec[i]);

	if (size >= game::BOARD_COLS * game::BOARD_ROWS_NO_IN_1314)
	{
		complementPuyoColorSet(field, img_split_vec, size);
	}
}


// Recognition adjustment of board.
void State::complementPuyoColorSet(std::vector<int> *const field, 
								   const std::vector<cv::Mat> &img_split_vec, 
								   const int &size)
{
	// Delete floating puyo.
	for (size_t i = 1; i < game::BOARD_COLS*game::BOARD_ROWS_NO_IN_1314; ++i)
	{
		if (i % game::BOARD_ROWS_NO_IN_1314 == 0) continue;
			
		if ((*field)[i-1] == color::NONE && (*field)[i] != color::NONE)
			(*field)[i] = color::NONE;
	}
	// This code that to judge between "X" or not.
	// "X" is top of the third row.
	// "35" represent location of "X".
	static const int x_place = 35;
	static const int red_bit_num = colorNum2BitNum(color::RED);
	if ((*field)[x_place] == red_bit_num)
	{
		if (!_isExistRedInColorList)
		{
			(*field)[x_place] = color::NONE;
		}
		else
		{
			cv::Mat diff_img;
			cv::absdiff(img_split_vec[x_place], _redPuyo, diff_img);
			cv::Mat hsv_channels[3];
			cv::split(diff_img, hsv_channels);
			cv::resize(hsv_channels[2], hsv_channels[2], cv::Size(), 0.5, 0.5, cv::INTER_NEAREST);
			cv::threshold(hsv_channels[2], hsv_channels[2], 100, 255, cv::THRESH_BINARY);
			img_p::imgAroundCutRate(hsv_channels[2], &hsv_channels[2], 0.05, 0.05, 0.9, 0.9);
			img_p::closing(hsv_channels[2], &hsv_channels[2], img_p::kernel_3, /*iteration*/3);
			if (50 < cv::countNonZero(hsv_channels[2]))
				(*field)[x_place] = color::NONE;
		}
	}
		
	// for all delete or not, and chain effect or not.
	static const int yellow_bit_num = colorNum2BitNum(color::YELLOW);
#ifdef DEBUG_YELLOW
	std::map<std::string, cv::Mat> yellow_img_list, diff_img_list, diff_color_img_list ,diff_binary_img_list,
		diff_binary_cut_around_img_list, final_img_list;
#endif
	int field_size = static_cast<int>(field->size());
	for (int i = 0; i < field_size; ++i)
	{
		
		if ((*field)[i] == yellow_bit_num)
		{
			if (!_isExistYellowInColorList)
				(*field)[i] = color::NONE;
			else
			{
				cv::Mat diff_img;
				cv::absdiff(img_split_vec[i], _yellowPuyo, diff_img);
				cv::Mat hsv_channels[3];
				cv::split(diff_img, hsv_channels);

				////////////////////				
#ifdef DEBUG_YELLOW
				cv::Mat diff_color = diff_img.clone();
				std::string debug_diff_color_path = "diff_color" + std::to_string(i+1);
				diff_color_img_list.insert(std::pair<std::string, cv::Mat>(debug_diff_color_path, diff_color));
				
				cv::Mat diff_gray = hsv_channels[2].clone();
				std::string debug_diff_img_file_path = "diff" + std::to_string(i+1);
				diff_img_list.insert(std::pair<std::string, cv::Mat>(debug_diff_img_file_path, diff_gray));

				cv::Mat debug_yellow = img_split_vec[i].clone();
				std::string debug_yellow_img_file_path = "yellow" + std::to_string(i+1);
				yellow_img_list.insert(std::pair<std::string, cv::Mat>(debug_yellow_img_file_path, debug_yellow));
#endif
				////////////////////

				// binary
				cv::threshold(hsv_channels[2], hsv_channels[2], 80, 255, cv::THRESH_BINARY);
				
				////////////////////
#ifdef DEBUG_YELLOW
				cv::Mat debug_diff_binary_img = hsv_channels[2].clone();
				std::string debug_diff_binary_img_file_path = "diff_binary" + std::to_string(i+1);
				diff_binary_img_list.insert(std::pair<std::string, cv::Mat>(debug_diff_binary_img_file_path, debug_diff_binary_img));
#endif
				////////////////////
				
				img_p::opening(hsv_channels[2], &hsv_channels[2], img_p::kernel_3, /*iteration*/1);
				cv::dilate(hsv_channels[2], hsv_channels[2], img_p::kernel_3, cv::Point(-1, -1), 1);
				img_p::imgAroundCutRate(hsv_channels[2], &hsv_channels[2], 0.05, 0.20, 0.9, 0.75);

				////////////////////
#ifdef DEBUG_YELLOW
				cv::Mat debug_diff_binary_cut_img = hsv_channels[2].clone();
				std::string debug_diff_binary_cut_img_file_path = "diff_binary_cut" + std::to_string(i+1);
				diff_binary_cut_around_img_list.insert(std::pair<std::string, cv::Mat>
													   (debug_diff_binary_cut_img_file_path,
														debug_diff_binary_cut_img));
#endif
				////////////////////
				
				cv::resize(hsv_channels[2], hsv_channels[2], cv::Size(), 0.5, 0.5, cv::INTER_NEAREST);
				if (80 < cv::countNonZero(hsv_channels[2]))
					(*field)[i] = color::NONE;
#ifdef DEBUG_YELLOW
				std::cout << "count non zero ("<< i+1<< ")  : " << cv::countNonZero(hsv_channels[2]) << std::endl;
				debug::showForDebug(hsv_channels[2], 0);

				cv::Mat debug_final_img = hsv_channels[2].clone();
				std::string debug_final_img_path = "final" + std::to_string(i+1);
				final_img_list.insert(std::pair<std::string, cv::Mat>
									  (debug_final_img_path,
									   debug_final_img));
#endif
			}
		}
	}
#ifdef DEBUG_YELLOW
	debug::saveImg(diff_color_img_list.begin(), diff_color_img_list.end(), "diff_color", true, true);
	debug::saveImg(diff_img_list.begin(), diff_img_list.end(), "diff_yellow", false, true);
	debug::saveImg(yellow_img_list.begin(), yellow_img_list.end(), "yellow", true, true);
	debug::saveImg(diff_binary_img_list.begin(), diff_binary_img_list.end(), "diff_yellow_binary", false, true);
	debug::saveImg(diff_binary_cut_around_img_list.begin(), diff_binary_cut_around_img_list.end(),
				   "diff_yellow_binary_cut", false, true);
	debug::saveImg(final_img_list.begin(), final_img_list.end(), "final", false, true);
#endif
	// Delete floating puyo.(again)
	for (int i = 1; i < game::BOARD_COLS*game::BOARD_ROWS_NO_IN_1314; ++i)
	{
		if (i % game::BOARD_ROWS_NO_IN_1314 == 0) continue;
			
		if ((*field)[i-1] == color::NONE && (*field)[i] != color::NONE)
			(*field)[i] = color::NONE;
	}

	}


int State::getPuyoBitNumPerPiece(const cv::Mat &img)
{
	int color = getPuyoColorNumPerPiece(img);

	// throw
	if (color == color::NONE || color == color::DIST)
		return color;
	
	auto it = std::find(_puyo_color_list.begin(), _puyo_color_list.end(), color);
	
	if (!_initColorList && it == _puyo_color_list.end()) {
		_puyo_color_list.push_back(color);

		if (_puyo_color_list.size() == color::PUYO_COLOR_NUM)
		{
			_initColorList = true;
			auto it_red = std::find(_puyo_color_list.begin(), _puyo_color_list.end(), color::RED);
			if (it_red != _puyo_color_list.end())
				_isExistRedInColorList = true;
			auto it_yellow = std::find(_puyo_color_list.begin(), _puyo_color_list.end(), color::YELLOW);
			if (it_yellow != _puyo_color_list.end())
				_isExistYellowInColorList = true;
		}
	}
	return colorNum2BitNum(color);
}

// 1p's next background color is blue.
bool State::isExistNext_1p()
{
	cv::Mat next_1p(_img, (*_pic_rect_list)[pic::IS_NEXT_1P_RECT_I]);
	int color = getPuyoColorNumPerPiece(next_1p, true);

	if (color == color::GREEN || color == color::RED || color == color::PURPLE || color == color::YELLOW)
		return true;

	if (color == color::BLUE)
		return isExistPuyo(next_1p);

	return false;
}


bool State::isExistNext_2p()
{
	cv::Mat next_2p(_img, (*_pic_rect_list)[pic::IS_NEXT_2P_RECT_I]);
	int color = getPuyoColorNumPerPiece(next_2p, true);

	if (color == color::GREEN || color == color::BLUE || color == color::PURPLE || color == color::YELLOW)
		return true;

	if (color == color::RED)
		return isExistPuyo(next_2p);

	return false;
}

// Judge between fight or not.
bool State::isJudgeFightEnd()
{
	HSV hsv;
	int yellow = 0;
	int green = 0;
	int other = 0;
	cv::Mat finish(_img, (*_pic_rect_list)[pic::FINISH_RECT_I]);
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
	cv::Mat end_1p(_img, (*_pic_rect_list)[pic::RESULT_1P_RECT_I]);
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
	cv::Mat end_2p(_img, (*_pic_rect_list)[pic::RESULT_2P_RECT_I]);
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


int State::getPuyoColorNumPerPiece(const cv::Mat &image, bool is_next)
{
	// for yellow puyo.
	cv::Mat image_padding;
	// for exist next
	if (is_next)
		img_p::imgAroundCutRate(image, &image_padding, 0.1, 0.35, 0.8, 0.65);
	else
		img_p::imgAroundCutRate(image, &image_padding, 0.1, 0.1, 0.8, 0.8);
	
	// Receive image(piece puyo), and judge puyo color
	float size = 0.2;
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
			++color_pixel_dict[hsv.toGetPixelPuyoColor()];
		}
	}
	if (is_next)
	{
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
	debug::saveElem(color_picel_dict_for_debug.begin(), color_picel_dict_for_debug.end(), "color_elem", false);
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

bool State::isExistPuyo(const cv::Mat &img)
{
	cv::Mat hsv_channels[3];
	cv::split(img, hsv_channels);
	cv::Mat gray = hsv_channels[2];

	cv::bitwise_not(gray, gray);
	// img_p::sharpningKernel9(gray, &gray);
	// img_p::opening(gray, &gray, img_p::kernel_3, 2);
	cv::threshold(gray, gray, 40, 255, cv::THRESH_TOZERO);
	cv::bitwise_not(gray, gray);
	std::vector<cv::Vec3f> circles;
	HoughCircles(gray, circles, cv::HOUGH_GRADIENT,
				 /*dp*/1,
				 /*minDist*/40,
				 /*param1*/10,
				 /*param2*/20,
				 /*minRadius*/20);
#if false
	// Display 
	for( size_t i = 0; i < circles.size(); i++ )
	{
		cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// circle center
		cv::circle( gray, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );
		// circle outline
		cv::circle( gray, center, radius, cv::Scalar(0,0,255), 3);
	}
	debug::showForDebug(gray, 1);
#endif
	return (circles.size() == 0) ? false : true;
}

bool State::isJudgeClear()
{
	HSV hsv;
	int count_orange = 0;
	int count_other  = 0;

	cv::Mat clear_img(_img, (*_pic_rect_list)[pic::CLEAR_RECT_I]);
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
