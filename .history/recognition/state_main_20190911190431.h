#ifndef _STATE_MAIN_
#define _STATE_MAIN_

// Include screenshotpath
#include "./color.h"
#include "./screen_shot.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <chrono>
#include <string>
#include <vector>
#include <string>
#include <omp.h>
#include <thread>
#include <iostream>

class State
{
public:
	explicit State(std::string set_monitor)
	{
		screen = ScreenShot::getScreenshot(set_monitor);
	}

	// update image from screen.
	void operator()
	{
		screen(img);
		cv::cvtColor(img, img, cv::COLOR_BGR2HSV);
	}

	bool getState(std::string mode);
	/* ex : (Win, Lose, Drow)*/
	void getState(std::string mode, int *const issue); 
	// Mainly use to get field or next
	void getState(std::string mode, std::vector<int>* field);

private:
	cv::Mat img;
	ScreenShot screen;

	void getNext_1p(std::vector<int>* field);
	void getNext2_1p(std::vector<int>* filed);
	void getBoard_1p(std::vector<int>* field);
	void getNext_2p(std::vector<int>* field);
	void getNext2_2p(std::vector<int>* field);
	void getBoard_2p(std::vector<int>* field);
	void splitImage(cv::Mat *const image, const int *const int col_num, const int *const int row_num, std::vector<cv::Mat> *const image_vec);
	/*
	I'm not goint to implement it for amination.
	void getMyNoticeOjama(std::vector<int> &field);
	void getMyNoticeOjama(std::vector<int> &filed);
	*/
	// finmw
	bool isExistNext();
	// Judge between enf of fight or not.
	bool isJudgeFightEnd();
	// Judge get result of fight.
	int getResult();
	// Recognition color of puyo.
	int toReadPuyoColor(const HSV *const hsv);
	int toReadBlueRed(const HSV* const hsv):
	int toReadGreenYellow(const HSV* const hsv):

}

#endif // STATE_MAIN_