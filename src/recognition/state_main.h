#ifndef _STATE_MAIN_
#define _STATE_MAIN_

// Include screenshotpath
#include "./color.h"
#include "./screen_shot.h"
#include "./image_processing.h"
#include "../tools/debug.h"

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
unsigned const int BOARD_COLS = 6;
unsigned const int BOARD_ROWS_NO_IN_1314 = 12;
unsigned const int BOARD_ROWS_IN_1314 = 14;
unsigned const int NEXT1_COLS = 1;
unsigned const int NEXT1_ROWS = 2;
unsigned const int NEXT2_COLS = 1;
unsigned const int NEXT2_ROWS = 2;
}

namespace player
{
const int DEFAULT = -1;
unsigned const int MOMOKEN = 0;
unsigned const int HISYA = 1;
unsigned const int KAMESTORY = 2;
} // namespace player

namespace get_mode
{
unsigned const int existNext_1p   = 0;
unsigned const int existNext_2p   = 1;
unsigned const int allPuyo_1p     = 2;
unsigned const int allPuyo_2p     = 3;
unsigned const int boardPuyo_1p   = 4;
unsigned const int boardPuyo_2p   = 5;
unsigned const int nextPuyo_1p    = 6;
unsigned const int nextPuyo_2p    = 7;
unsigned const int isFightEnd     = 8;
unsigned const int battleResult   = 9;
unsigned const int isClear        = 10; 

} // namespace get_mode

namespace judge
{
unsigned const int NONE = 0;	
unsigned const int WIN = 1;
unsigned const int LOSE = 2;
unsigned const int DROW = 3; 	
}

class State
{
public:
	State(ScreenShot *const scr_, const int &player_ = player::DEFAULT)
		: player(player_)
	{
		initialize();
		this->scr = scr_;
		this->puyo_color_list.reserve(color::PUYO_COLOR_NUM);
		this->player_resize = {
			{player::DEFAULT, pic::normal},
			{player::MOMOKEN, pic::momoken},
			{player::HISYA, pic::hisya},
			{player::KAMESTORY, pic::kamestry}
		};
				
		// search key in player_resize
		if (this->player_resize.find(this->player) == this->player_resize.end())
		{
			LOG("Not exist player.");
			exit(1);
		}

		this->redPuyo    = cv::imread("../data/puyoImg/red.jpg");
		this->yellowPuyo = cv::imread("../data/puyoImg/yellow.jpg");

		// The first 10 Screenshots are slow. (why)
		for (int i = 0; i < 10; ++i)
		{
			step();
		}
	}

	inline void initialize()
	{
		isExistRedInColorList = false;
		isExistYellowInColorList = false;
		initColorList = false;
	}

	inline void step()
	{
		cv::Mat image;
		(*scr) >> image;
		setImg(image);
	}

	inline void setImg(cv::Mat &img_) {
		cv::cvtColor(img_, img_, CV_BGR2HSV);

		if (this->player != player::DEFAULT)
			cutImg(&img_);

		if (img_.cols != pic::HD_WIDTH || img_.rows != pic::HD_HEIGHT) 
		{
			img_p::toHDImg(&img_);
		}

		this->img = img_;
	}

	inline cv::Mat getImg()
	{
		cv::Mat rgb_img;
		cv::cvtColor(this->img, rgb_img, cv::COLOR_HSV2BGR);
		return rgb_img;
	}

	inline void cutImg(cv::Mat *const img_)
	{
		*img_ = (*img_)(player_resize[player]);
	}


	bool isGetState(const int &mode);
	/* ex : (Win, Lose, Drow)*/
	void getState(const int &mode, int &issue);
	// Mainly use to get field or next
	void getState(const int &mode, std::vector<int> &field, bool isColorNum=false);

	void bitNum2ColorNumForVec(std::vector<int> *const field);
	void colorNum2bitNumForVec(std::vector<int> *const field);
	void colorNum2ColorStringForVec(const std::vector<int> &field_int, std::vector<std::string> *const field_str);

protected:	
	std::map<unsigned int, cv::Rect> player_resize;
	int player;
	cv::Mat img;
	std::vector<int> puyo_color_list;
	bool initColorList;
	bool isExistRedInColorList;           // for "X"
	bool isExistYellowInColorList;        // for all delete.
	cv::Mat redPuyo;                      // for "X"
	cv::Mat yellowPuyo;                   // for all delete and chain effect
	ScreenShot *scr;


	template <typename T>
	void initializeField(const int *const size, std::vector<T> *const field)
	{
		if (field->size() != *size)
		{
			std::vector<T> ().swap(*field);
			field->resize(*size);
		}
	}


	int colorNum2BitNum(const int& color);
	int bitNum2ColorNum(const int& color);
	void colorNum2ColorString(const int& color, std::string *const str);
	int getColor(const cv::Mat &img);

	void getPuyoColorSet(std::vector<int> *field, const int& col_num, const int& row_num, 
									const cv::Rect &target_rect, const std::string &dir_path = "");
	void complementPuyoColorSet(std::vector<int> *const field, const std::vector<cv::Mat> &img_split_vec, const int &size);
	/*
	I'm not goint to implement it for amination.
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
	int toGetPuyoColorPerPiece(const cv::Mat &image, bool is_exist_next=false);

};

#endif // STATE_MAIN_
