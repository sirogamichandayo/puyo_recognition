#ifndef _STATE_MAIN_
#define _STATE_MAIN_

// Include screenshotpath
#include "./color.h"
#include "./screen_shot.h"
#include "./image_processing.h"
#include "../tools/debug/debug.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <ostream>
#include <iterator>
#include <chrono>
#include <deque>
#include <vector>
#include <string>
#include <omp.h>
#include <memory>
#include <thread>
#include <iostream>
#include <map>
#include <sys/stat.h>

namespace img_p = image_processing;

namespace game
{
	unsigned constexpr int BOARD_COLS = 6;
	unsigned constexpr int BOARD_ROWS_NO_IN_1314 = 12;
	unsigned constexpr int BOARD_ROWS_IN_1314 = 14;
	unsigned constexpr int NEXT1_COLS = 1;
	unsigned constexpr int NEXT1_ROWS = 2;
	unsigned constexpr int NEXT2_COLS = 1;
	unsigned constexpr int NEXT2_ROWS = 2;
	unsigned constexpr int ALL_PUYO_NUM =
		game::BOARD_COLS * game::BOARD_ROWS_NO_IN_1314 +
		game::NEXT1_COLS * game::NEXT1_ROWS +
		game::NEXT2_COLS * game::NEXT2_ROWS;
}


namespace get_mode
{
	unsigned constexpr int IS_EXIST_NEXT_1P   = 0;
	unsigned constexpr int IS_EXIST_NEXT_2P   = 1;
	unsigned constexpr int ALL_PUYO_1P        = 2;
	unsigned constexpr int ALL_PUYO_2P        = 3;
	unsigned constexpr int BOARD_PUYO_1P      = 4;
	unsigned constexpr int BOARD_PUYO_2P      = 5;
	unsigned constexpr int NEXT_PUYO_1P       = 6;
	unsigned constexpr int NEXT_PUYO_2P       = 7;
	unsigned constexpr int IS_FIGHT_END       = 8;
	unsigned constexpr int BATTLE_RESULT      = 9;
	unsigned constexpr int IS_CLEAR           = 10; 
} // namespace get_mode

namespace judge
{
	unsigned constexpr int NONE = 0;	
	unsigned constexpr int WIN = 1;
	unsigned constexpr int LOSE = 2;
	unsigned constexpr int DROW = 3; 	
}

class State
{
public:
	State(ScreenShot *const scr_, const std::vector<cv::Rect> *const rect_vec,
		  const cv::Mat &red_, const cv::Mat &yellow_)
		: _scr(scr_), _pic_rect_list(rect_vec)
	{
		initialize();
		_puyo_color_list.reserve(color::PUYO_COLOR_NUM);

		cv::cvtColor(red_, _redPuyo, CV_BGR2HSV);
		cv::cvtColor(yellow_, _yellowPuyo, CV_BGR2HSV);

		// The first 10 Screenshots are slow. (why)
		for (int i = 0; i < 10; ++i)
		{
			step();
		}
	}

	~State()
	{
		std::vector<int>().swap(_puyo_color_list);
	}

	inline void initialize()
	{
		_isExistRedInColorList = false;
		_isExistYellowInColorList = false;
		_initColorList = false;
	}

	inline void step()
	{
		cv::Mat image;
		(*_scr) >> image;
		setImg(image);
	}

	inline void setImg(const cv::Mat &img_) {
		cv::Mat img, resize_img;
		
		cv::cvtColor(img_, img, CV_BGR2HSV);
		_img = img;
		
		cv::resize(img, resize_img, cv::Size(), 0.2, 0.2);
		_resize_img = resize_img;
	}

	inline void getImg(cv::Mat *const img_output, bool is_rgb = false)
	{
		if (is_rgb)
		{
			cv::cvtColor(_img, *img_output, cv::COLOR_HSV2BGR);
		}
		else
		{
			*img_output = _img.clone();
		}
	}

	bool isGetState(const int &mode);
	/* example : (Win, Lose, Drow)*/
	void getState(const int &mode, int &issue);
	// Mainly use to get field or next
	void getState(const int &mode, std::vector<int> *const field, bool isColorNum=false);

	void bitNum2ColorNumForVec(const std::vector<int> &input_puyo_field, std::vector<int> *const output_puyo_field);
	void bitNum2ColorNumForVec(std::vector<int> *const puyo_field);
	
	void colorNum2BitNumForVec(const std::vector<int> &input_puyo_field, std::vector<int> *const output_puyo_field);
	void colorNum2BitNumForVec(std::vector<int> *const puyo_field);
	
	void bitNum2ColorStringForVec(const std::vector<int> &input_puyo_field, std::vector<std::string> *const outout_puyo_field);
	void colorNum2ColorStringForVec(const std::vector<int> &input_puyo_field, std::vector<std::string> *const output_puyo_field);

protected:	
	cv::Mat _img, _resize_img;
	std::vector<int> _puyo_color_list;
	bool _initColorList;
	bool _isExistRedInColorList;           // for "X"
	bool _isExistYellowInColorList;        // for all delete.

	ScreenShot *_scr;
	const std::vector<cv::Rect> *const _pic_rect_list;
	cv::Mat _redPuyo;                      // for "X"
	cv::Mat _yellowPuyo;                   // for all delete and chain effect
	
	int colorNum2BitNum(const int& color_num);
	int bitNum2ColorNum(const int& bit_num);
	void colorNum2ColorString(const int& color_num, std::string *const str);
	void bitNum2ColorString(const int& color_num, std::string *const str);
	
	void getPuyoColorSet(std::vector<int> *field, const int& col_num, const int& row_num, 
									const cv::Rect &target_rect, const std::string &dir_path = "");
	int getPuyoBitNumPerPiece(const cv::Mat &img);
	int getPuyoColorNumPerPiece(const cv::Mat &image, bool is_next=false);
	bool isExistPuyo(const cv::Mat &img); // Use hough transform
	void complementPuyoColorSet(std::vector<int> *const field, const std::vector<cv::Mat> &img_split_vec, const int &size);
	/*
	void getMyOjamaCount_1p(std::vector<int> &field);
	void getMyOjamaCount_2p(std::vector<int> &filed);
	*/

	bool isExistNext_1p();
	bool isExistNext_2p();
	// Judge between enf of fight or not.
	bool isJudgeFightEnd();
	bool isJudgeClear(); // Judge clear or not(TA).
	// Judge get result of fight.
	void getResult(int *const result);		
};

#endif // STATE_MAIN_
