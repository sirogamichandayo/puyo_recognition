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
	State(ScreenShot *const scr_, const int &player_ = player::DEFAULT)
		: player(player_), initColorList(false), 
			isExistRedInColorList(false),
			isExistYellowInColorList(false)
	{
		this->scr = scr_;
		this->puyo_color_list.reserve(color::PUYO_COLOR_NUM);
		this->player_resize = {
			{player::MOMOKEN, pic::momoken},
			{player::HISYA, pic::hisya},
			{player::KAMESTORY, pic::kamestry}
		};
				
		// search key in player_resize
		if (this->player_resize(this->player) == this->player_resize.end())
		{
			LOG("Not exist player.");
			exit(1);
		}

		// make "X" histgram.
		const std::string DIR_PATH = "../data/AllDelete_X/";
		cv::Mat img_X = cv::imread(DIR_PATH+"puyo35.jpg", 1);
		this->InfluenceImgX = \
			std::make_pair(35, img_X);

		cv::cvtColor(img_X, img_X, cv::COLOR_BGR2HSV);

		cv::MatND hist_X;
		img_p::img2Hist(img_X, &hist_X);
		this->InfluenceHistX = \
			std::make_pair(35, hist_X);
		
		// make "All Delete" histgram.
		std::vector<int> all_delete_indexes{8, 9, 10,
																				20, 21, 22,
																				32, 33, 34,
																				44, 45, 46,
																				56, 57, 58,
																				68, 69};
		for (const auto &index : all_delete_indexes)
		{
			// make histgram.
			const std::string FILE_PATH = DIR_PATH + "puyo" + std::to_string(index) + ".jpg";
			cv::Mat img_all_delete = cv::imread(FILE_PATH, 1);
			cv::cvtColor(img_all_delete, img_all_delete, cv::COLOR_BGR2HSV);
			InfluenceImgAllDelete.insert(std::pair<int, cv::MatND>(index, img_all_delete));


			cv::MatND hist_all_delete;
			img_p::img2Hist(img_all_delete, &hist_all_delete);
			InfluenceHistAllDelete.insert(std::pair<int, cv::MatND>(index, hist_all_delete));
		}
	}

	inline void step()
	{
		cv::Mat image;
		(*scr) >> image;
		setImg(image);
	}

	inline void setImg(cv::Mat &img_) {
		cv::cvtColor(img_, img_, CV_BGR2HSV);
		if (img_.cols != pic::HD_WIDTH || img_.rows != pic::HD_HEIGHT) {
			img_p::toHDImg(&img_);
		}
		if (this->player =! player::default)
			cutImg(&img_);
		this->img = img_;
	}

	inline void getImg(cv::Mat &img_)
	{
		img_ = this->img;
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
	bool isExistRedInColorList; // for "X"
	bool isExistYellowInColorList; // for all delete in other words zenkesi.
	std::map<int, cv::MatND> InfluenceHistAllDelete; // Plan to delete.
	std::map<int, cv::Mat>   InfluenceImgAllDelete;
	std::pair<int, cv::MatND> InfluenceHistX; // Plan to delete.
	std::pair<int, cv::Mat>   InfluenceImgX;
	ScreenShot *scr;


	template <typename T>
	void initializeField(const int *const size, std::vector<T> *const field)
	{
		std::vector<T> ().swap(*field);
		if (field->size() != *size)
			field->resize(*size);
	}

	void cutImg(cv::Mat *const img_);

	int colorNum2ForBitNum(const int& color);
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
	// Judge get result of fight.
	void getResult(int *const result);		
	int toGetPuyoColorPerPiece(const cv::Mat &image, bool is_exist_next=false);

};

#endif // STATE_MAIN_
