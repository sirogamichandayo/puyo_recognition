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
	unsigned const int BOARD_COLS = 6;
	unsigned const int BOARD_ROWS_NO_IN_1314 = 12;
	unsigned const int BOARD_ROWS_IN_1314 = 14;
	unsigned const int NEXT1_COLS = 1;
	unsigned const int NEXT1_ROWS = 2;
	unsigned const int NEXT2_COLS = 1;
	unsigned const int NEXT2_ROWS = 2;
}


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
	State(ScreenShot *const scr_)
	{
		initialize();
		this->scr = scr_;
		this->puyo_color_list.reserve(color::PUYO_COLOR_NUM);
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

	inline void setImg(const cv::Mat &img_) {
		cv::Mat img, resize_img;
		
		cv::cvtColor(img_, img, CV_BGR2HSV);
		this->img = img;
		
		cv::resize(img, resize_img, cv::Size(), 0.2, 0.2);
		this->resize_img = resize_img;
	}

	inline void getImg(cv::Mat *const img_output, bool is_rgb = false)
	{
		if (is_rgb)
		{
			cv::cvtColor(this->img, *img_output, cv::COLOR_HSV2BGR);
		}
		else
		{
			*img_output = this->img.clone();
		}
	}

	bool isGetState(const int &mode);
	/* example : (Win, Lose, Drow)*/
	void getState(const int &mode, int &issue);
	// Mainly use to get field or next
	void getState(const int &mode, std::vector<int> *const field, bool isColorNum=false);

	void bitNum2ColorNumForVec(std::vector<int> *const field);
	void colorNum2bitNumForVec(std::vector<int> *const field);
	void colorNum2ColorStringForVec(const std::vector<int> &field_int, std::vector<std::string> *const field_str);

protected:	
	cv::Mat img, resize_img;
	std::vector<int> puyo_color_list;
	bool initColorList;
	bool isExistRedInColorList;           // for "X"
	bool isExistYellowInColorList;        // for all delete.
	cv::Mat redPuyo;                      // for "X"
	cv::Mat yellowPuyo;                   // for all delete and chain effect
	ScreenShot *scr;					

	int colorNum2BitNum(const int& color);
	int bitNum2ColorNum(const int& color);
	void colorNum2ColorString(const int& color, std::string *const str);
	
	int getColorNumber(const cv::Mat &img);
	void getPuyoColorSet(std::vector<int> *field, const int& col_num, const int& row_num, 
									const cv::Rect &target_rect, const std::string &dir_path = "");
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
	int toGetPuyoColorPerPiece(const cv::Mat &image, bool is_exist_next=false);

};

#endif // STATE_MAIN_
