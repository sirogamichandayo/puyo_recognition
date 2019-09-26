#ifndef _STATE_MAIN_
#define _STATE_MAIN_

// Include screenshotpath
#include "./color.h"
#include "./screen_shot.h"
#include "./env_img.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
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
#include <thread>
#include <iostream>
#include <map>

namespace game
{
unsigned const int BOARD_COLS = 6;
unsigned const int BOARD_ROWS_NO_IN_1314 = 12;
unsigned const int board_rows_IN_1314 = 14;
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
unsigned const int existNext_1p = 0;
unsigned const int existNext_2p = 1;
unsigned const int allPuyo_1p = 2;
unsigned const int allPuyo_2p = 3;
unsigned const int isFightEnd = 4;
unsigned const int battleResult = 5;

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
	State(const int &player_ = player::DEFAULT)
	{
		this->player = player_;
		this->puyo_color_list.reserve(color::PUYO_COLOR_NUM);
		this->initColorList = false;
		this->isExistRedInColorList = false;
		this->player_resize = {
			{player::MOMOKEN, pic::momoken},
			{player::HISYA, pic::hisya},
			{player::KAMESTORY, pic::kamestry}
		};
	}

	inline void setImg(cv::Mat &img_) {
		cv::cvtColor(img_, img_, CV_BGR2HSV);
		if (img_.cols != pic::HD_WIDTH || img_.rows != pic::HD_HEIGHT) {
			toHDImg(&img_);
		}
		cutImg(&img_);
		this->img = img_;
	}

	bool isGetState(const int &mode);
	/* ex : (Win, Lose, Drow)*/
	void getState(const int &mode, int &issue);
	// Mainly use to get field or next
	void getState(const int &mode, std::vector<int> &field);

private:
	std::map<unsigned int, cv::Rect> player_resize;
	int player;
	cv::Mat img;
	std::vector<int> puyo_color_list;
	bool initColorList;
	bool isExistRedInColorList;

	// debug
	int count_call_is_next_1p=0;


	template <typename T>
	void initializeField(const int *const size, std::vector<T> *const field)
	{
		field->clear();
		if (field->size() != *size)
			field->resize(*size);
	}

	void cutImg(cv::Mat *const img_);
	void toHDImg(cv::Mat *const img_);
	void paddingImg(const cv::Mat &img_, cv::Mat &img_pad, const float &x1_rate, 
	      const float &y1_rate, const float &w_rate, const float &h_rate);

	int colorNum2ForBitNum(int color);
	int getColor(const cv::Mat &img);
	void getNext_1p(std::vector<int> *field);
	void getNext_2p(std::vector<int> *field);
	void getNext2_1p(std::vector<int> *filed);
	void getNext2_2p(std::vector<int> *field);
	void getBoard_1p(std::vector<int> *field);
	void getBoard_2p(std::vector<int> *field);
	void splitImage(const cv::Mat *const image, const int *const col_num,
									const int *const row_num, std::vector<cv::Mat> *const image_vec);
	bool isExistNext_1p();
	bool isExistNext_2p();
	/*
	I'm not goint to implement it for amination.
	void getMyNoticeOjama(std::vector<int> &field);
	void getMyNoticeOjama(std::vector<int> &filed);
	*/
	// Judge between enf of fight or not.
	bool isJudgeFightEnd();
	// Judge get result of fight.
	void getResult(int *const result);		
	int toGetPuyoColorPerPiece(const cv::Mat &image);


	// For debug
	void showForDebug(const cv::Mat &image)
	{
		cv::Mat img_rgb;
		cv::cvtColor(image, img_rgb, CV_HSV2BGR);
		cv::imshow("debug", img_rgb);
		cv::waitKey(100);
	}
	
	template<class InputIterator>
	void saveColorAndImg(InputIterator begin, InputIterator end, const cv::Mat &img_)
	{
		cv::Mat img_hsv;
		cv::cvtColor(img_, img_hsv, CV_HSV2BGR);

		const std::string path = "/mnt/programming/data/MO/tokopuyo/recognition_data2/";
		std::ofstream write_file;
		if (count_call_is_next_1p == 0)
			write_file.open(path+"color_elem.txt", std::ios::trunc);
		else
			write_file.open(path+"color_elem.txt", std::ios_base::app);
		std::array<std::string, 7> color_list = {
				"NONE",
				"DIST",
				"RED",
				"BLUE",
				"YELLOW",
				"GREEN",
				"PURPLE"
		};

		std::ostringstream ss;
		ss << "puyo" << std::setw(3) << std::setfill(' ') << count_call_is_next_1p << " -> ";

		std::string str = "";
		str += ss.str();
		for (auto const& color : color_list)
		{
			std::ostringstream ss_color, ss_num;
			ss_color << std::setw(7) << std::setfill(' ') << color << " : ";
			ss_num << std::setw(3) << std::setfill('0') << begin++->second;

			str += (ss_color.str() + ss_num.str());
		}
		str += '\n';

		write_file << str;
		cv::imwrite(path+"puyo"+std::to_string(count_call_is_next_1p)+".jpg", img_hsv);
		
		++count_call_is_next_1p;
	}
};

#endif // STATE_MAIN_