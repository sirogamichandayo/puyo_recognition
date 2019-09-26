#include "../src/recognition/state_main.h"
#include "../src/recognition/screen_shot.h"
#include "../src/log/log.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <stopWatch/stopWatch.h>
#include <sstream>
#include <memory>
#include <vector>
#include <string>

int main()
{
	using namespace game;
	std::vector<int> all_puyo(BOARD_ROWS_NO_IN_1314 * BOARD_COLS +
														NEXT1_ROWS * NEXT2_COLS + 
														NEXT2_ROWS * NEXT2_COLS);
	auto begin = all_puyo.begin();
	auto end   = all_puyo.end();
	ScreenShot src = ScreenShot::getScreenShot("three");
	cv::Mat img;
	State env(player::DEFAULT);
	for (int i = 0; i < 10; ++i)
	{
		src >> img;
		env.setImg(img);

		env.getState(get_mode::allPuyo_1p, all_puyo.begin(), all_puyo.end());

	}
}