#include "../src/recognition/state_main.h"
#include "../src/recognition/screen_shot.h"
#include "../src/log/log.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <string>
#include <atomic>
#include <unistd.h>

int main(int argc, char *argv[])
{
#if true
	assert(argc >= 2);
	debug::initializeDir(argv[1], true);
#endif
	int time = std::atoi(argv[2]) * 1000;
	std::vector<int> board_puyo_1p(72);
	std::vector<int> next_puyo_1p(4);
	ScreenShot scr = ScreenShot::getScreenShot("three");
	State env(&scr, player::DEFAULT);
	std::map<std::string, cv::Mat> img_map;
	int index = 0;
	for (;;)
	{
		env.step();
		if (!env.isGetState(get_mode::existNext_1p))
		{
			std::string title = "field" + std::to_string(index);
			img_map.insert(std::pair<std::string, cv::Mat>
									(title, env.getImg()));
			usleep(time);
			env.step();
			title = "next" + std::to_string(index);
			cv::Mat img_(env.getImg(), pic::next_1p);
			img_map.insert(std::pair<std::string, cv::Mat>(title, img_));
			std::cout << ++index << std::endl;
		}
		if (index == std::atoi(argv[3]))break;
	}
	LOG("Writing...");
	debug::saveImg(img_map.begin(), img_map.end(), "next_test");
}