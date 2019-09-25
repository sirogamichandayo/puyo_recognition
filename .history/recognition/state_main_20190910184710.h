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
	void getState(std::string mode, int &issue); 
	// Mainly use to get field or next
	void getState(std::string mode, std::vector<int> &field);

private:
	cv::Mat img;
	ScreenShot screen;

	void getMyNext(std::vector<int> &field);
	void getMyNextNext(std::vector<int> &filed);
	void getMyBoard(std::vector<int> &field);
	void getEnemyNext(std::vector<int> &field);
	void getEnemyNextNext(std::vector<int> &field);
	void getEnemyBoard(std::vector<int> &field);
	/*
	I'm not goint to implement it for amination.
	void getMyNoticeOjama(std::vector<int> &field);
	void getMyNoticeOjama(std::vector<int> &filed);
	*/
	bool isExistNext();
	// Judge between enf of fight or not.
	bool isJudgeFightEnd();
	// Judge between Vectory or Defeat.
	bool isJudgeVictoryDefeat();
	// Recognition color of puyo.
	int toReadPuyoColor(HSV *hsv);
	int toReadBlueRed(HSV *hsv):
	int toReadGreenYellow(HSV *hsv):

}

#endif // STATE_MAIN_