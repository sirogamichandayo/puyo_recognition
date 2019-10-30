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

int main()
{
	ScreenShot scr = ScreenShot::getScreenShot("three");
	cv::Mat img;
	std::vector<int> all_puyo(72);

	State env(&scr, player::DEFAULT);
	for (int i = 0; i < 50000; ++i)
	{
		env.step();
		env.getState(get_mode::allPuyo_1p, all_puyo);
		std::cout << "##########" << std::endl;
		std::cout << "index : " << i << std::endl;
		std::cout << "size  : " << all_puyo.size() << std::endl;
		std::cout << std::endl;
	}
}