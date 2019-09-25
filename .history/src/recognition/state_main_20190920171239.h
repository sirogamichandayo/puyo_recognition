#ifndef _STATE_MAIN_
#define _STATE_MAIN_

// Include screenshotpath
#include "./color.h"
#include "./screen_shot.h"
#include "./env_img.h"
#include "../log/log.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <algorithm>
#include <chrono>
#include <deque>
#include <vector>
#include <string>
#include <omp.h>
#include <thread>
#include <iostream>
#include <map>

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
		player = player_;
		puyo_color_list.reserve(4);
		initColorList = false;
		player_resize = {
			{player::DEFAULT, pic::normal},
			{player::MOMOKEN, pic::momoken},
			{player::HISYA, pic::hisya},
			{player::KAMESTORY, pic::kamestry}
		};
	}

	void setImg(const cv::Mat &img_) {
		cut_img(img_);
	}

	bool getState(const int &mode);
	/* ex : (Win, Lose, Drow)*/
	void getState(const int &mode, int &issue);
	// Mainly use to get field or next
	void getState(const int &mode, std::vector<int> &field);

private:
	std::map<unsigned int, cv::Rect> player_resize;
	int player;
	cv::Mat img;
	bool initColorList;
	std::vector<int> puyo_color_list;


	template <typename T>
	void initializeField(const int *const size, std::vector<T> *const field);
	void cut_img(const cv::Mat *const img_);

	int colorNum2ForBitNum(int color);
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
	int getResult();		
	int toGetPuyoColorPerPiece(const cv::Mat *const image);
};

#endif // STATE_MAIN_