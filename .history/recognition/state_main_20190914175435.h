#ifndef _STATE_MAIN_
#define _STATE_MAIN_

// Include screenshotpath
#include "./color.h"
#include "./screen_shot.h"
#include "./env_img.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <chrono>
#include <string>
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

class State
{
public:
	State(const EnvImg *const env_, const int &player_ = player::DEFAULT)
	{
		this->player = player_;
		this->puyo_color_list.reserve(4);
		this->init = false;
		this->env = EnvImg(env_);
		this->player_resize = {
			{player::DEFAULT, pic::default},
			{player::MOMOKEN, pic::momoken},
			{player::HISYA, pic::hisya},
			{player::KAMESTORY, pic::kamestry}
		}
	}

	// update image from screen.
	void operator()
	{
		cv::Mat img_, img_hsv;
		env >> img_;
		cv::cvtColor(img_hsv, img_, cv::COLOR_BGR2HSV);
		cut_img(&img_hsv);
	}

	bool getState(std::string mode);
	/* ex : (Win, Lose, Drow)*/
	void getState(std::string mode, int *const issue);
	// Mainly use to get field or next
	void getState(std::string mode, std::vector<int> *field);

private:
	std::map<unsigned int, cv::Rect> player_resize;
	int player;
	cv::Mat img;
	EnvImg env;
	bool initColorList;
	std::vector<int> puyo_color_list;

	void cut_img(const cv::Mat *const img_);

	void initializeField(int size, std::vector<int> *const field);

	int changeColorNum2ForBitNum(int color);
	void getNext_1p(std::vector<int> *field);
	void getNext2_1p(std::vector<int> *filed);
	void getBoard_1p(std::vector<int> *field);
	void getNext_2p(std::vector<int> *field);
	void getNext2_2p(std::vector<int> *field);
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
	// Recognition color of puyo.
	void toGetPuyoColor(const cv::Mat *const image, int *const color) int toReadPixelPuyoColor(const HSV *const hsv);
	int toReadBlueRed(const HSV *const hsv) : int toReadGreenYellow(const HSV *const hsv) :
}

#endif // STATE_MAIN_