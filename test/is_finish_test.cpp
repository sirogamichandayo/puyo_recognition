#include "../src/recognition/state_main.h"
#include "../src/recognition/screen_shot.h"
#include "../src/log/log.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <unistd.h>
#include <memory>
#include <vector>
#include <string>
#include <map>

int main(int argc, char *argv[])
{
	ScreenShot scr = ScreenShot::getScreenShot("three");
	State env(&scr, player::DEFAULT);
	debug::initializeDir();
	std::map<std::string, cv::Mat> img_map;
	int index = 0;
	int time = std::atoi(argv[1]) * 1000;
	for (;;)
	{
		env.step();
		if (env.isGetState(get_mode::isFightEnd))
		{
			std::cout << "finish" << ++index % 10 << std::endl;
			usleep(time);
		}
	}
}