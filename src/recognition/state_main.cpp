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
						(*_pic_rect_list)[pic::BOARD_1P_RECT_I] /*,"board_1p"*/);
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
	if (dir_path != "")
	{
		//////////////////////////////
		// DEBUG
		
		// save puyo image per piece
		std::map<std::string, cv::Mat> img_split_vec_for_debug;
		for (size_t i = 0; i < size; ++i)
		{
			std::string file_path_split = "puyo" + std::to_string(i); // for debug
			img_split_vec_for_debug.insert(std::pair<std::string, cv::Mat>(file_path_split, img_split_vec[i]));
		}
		debug::saveImg(img_split_vec_for_debug.begin(), img_split_vec_for_debug.end(), dir_path, true);
		/////////////////////////////////
	}

	for (size_t i = 0; i < size; ++i)
		(*field)[i] = getColorNumber(img_split_vec[i]);

	if (size == game::BOARD_COLS * game::BOARD_ROWS_NO_IN_1314)
		complementPuyoColorSet(field, img_split_vec, size);
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
	const int x_place = 35;
	const int red_bit_num = colorNum2BitNum(color::RED);
	if ((*field)[x_place] == red_bit_num)
	{
		if (!_isExistRedInColorList)
		{
			(*field)[x_place] = color::NONE;
		}
		else
		{
			cv::Mat element(3, 3, CV_8U, cv::Scalar::all(255));
			cv::Mat diff_X_FD;
			cv::absdiff(img_split_vec[x_place], _redPuyo, diff_X_FD);
			cv::Mat hsv_channels[3];
			cv::split(diff_X_FD, hsv_channels);
			cv::threshold(hsv_channels[2], hsv_channels[2], 100, 255, cv::THRESH_BINARY);
			img_p::imgAroundCutRate(hsv_channels[2], &hsv_channels[2], 0.05, 0.05, 0.9, 0.9);
			// opening.
			cv::morphologyEx(hsv_channels[2], hsv_channels[2], cv::MORPH_CLOSE, element, cv::Point(-1, -1), 3);
			cv::resize(hsv_channels[2], hsv_channels[2], cv::Size(), 0.5, 0.5, cv::INTER_NEAREST);
			if (50 < cv::countNonZero(hsv_channels[2]))
				(*field)[x_place] = color::NONE;
		}
	}
		
	// for all delete or not, and chain effect or not.
	const int yellow_bit_num = colorNum2BitNum(color::YELLOW);
	for (size_t i = 0; i < field->size(); ++i)
	{
		
		if ((*field)[i] == yellow_bit_num)
		{
			if (!_isExistYellowInColorList)
				(*field)[i] = color::NONE;
			else
			{
				cv::Mat element(3, 3, CV_8U, cv::Scalar::all(255));
				cv::Mat diff;
				cv::absdiff(img_split_vec[i], _yellowPuyo, diff);
				cv::Mat hsv_channels[3];
				cv::split(diff, hsv_channels);
				cv::threshold(hsv_channels[2], hsv_channels[2], 100, 255, cv::THRESH_BINARY);
				img_p::imgAroundCutRate(hsv_channels[2], &hsv_channels[2], 0.05, 0.05, 0.9, 0.9);
				// opening.
				cv::morphologyEx(hsv_channels[2], hsv_channels[2], cv::MORPH_CLOSE, element, cv::Point(-1, -1), 3);
				cv::resize(hsv_channels[2], hsv_channels[2], cv::Size(), 0.5, 0.5, cv::INTER_NEAREST);
				if (50 < cv::countNonZero(hsv_channels[2]))
					(*field)[i] = color::NONE;
			}
		}
	}

	// Delete floating puyo.(again)
	for (size_t i = 1; i < game::BOARD_COLS*game::BOARD_ROWS_NO_IN_1314; ++i)
	{
		if (i % game::BOARD_ROWS_NO_IN_1314 == 0) continue;
			
		if ((*field)[i-1] == color::NONE && (*field)[i] != color::NONE)
			(*field)[i] = color::NONE;
	}

	}


int State::getColorNumber(const cv::Mat &img)
{
	int color = toGetPuyoColorPerPiece(img);

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

bool State::isExistNext_1p()
{
	cv::Mat is_next_img(_img, (*_pic_rect_list)[pic::IS_NEXT_1P_RECT_I]);
	cv::Mat hsv_channels[3];
	cv::split(is_next_img, hsv_channels);
	cv::Mat gray = hsv_channels[2];
	/*
	cv::Canny(hsv_channels[2], hsv_channels[2], 100, 200);
	cv::bitwise_not(hsv_channels[2], hsv_channels[2]);
	*/
	std::vector<cv::Vec3f> circles;
	HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1, 40, 10, 30, /*minRadius*/20);

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

	return (circles.size() == 0) ? false : true;
}

bool State::isExistNext_2p()
{
	cv::Mat is_next_img_2p(_img, (*_pic_rect_list)[pic::IS_NEXT_2P_RECT_I]);
	return true;
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


int State::toGetPuyoColorPerPiece(const cv::Mat &image, bool is_exist_next)
{
	// for yellow puyo.
	cv::Mat image_padding;
	// for exist next
	if (is_exist_next)
		img_p::imgAroundCutRate(image, &image_padding, 0.1, 0.35, 0.8, 0.65);
	else
		img_p::imgAroundCutRate(image, &image_padding, 0.1, 0.1, 0.8, 0.8);
	
	// Receive image(piece puyo), and judge puyo color
	float size = 1.0;
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
