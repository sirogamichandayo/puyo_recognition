#include "../src/recognition/state_main.h"
#include "../src/recognition/screen_shot.h"
#include "../src/log/log.h"

#include <stopWatch/stopWatch.h>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <memory>
#include <vector>
#include <string>
#include <map>

int main(int argc, char *argv[])
{
	int time = atoi(argv[1]);
	std::cout << time << std::endl;
	ScreenShot scr = ScreenShot::getScreenShot("three");
	State env(&scr, player::DEFAULT);
	debug::initializeDir();
	std::map<std::string, cv::Mat> vec_map;
	int index = 0;
	for (;;)
	{
		env.step();
		if (!env.isGetState(get_mode::existNext_1p))
		for (;;)
		{
			env.step();
			if (env.isGetState(get_mode::existNext_1p))
			{
				if (time)
				{
					usleep(time);
					env.step();
				}
				std::string title = "puyofield_" + std::to_string(index);
				cv::Mat img = env.getImg();
				vec_map.insert(std::pair<std::string, cv::Mat>(title, img));
				++index;
				break;
			}
		}
		if (index == 50)
			break;
	}
	LOG("Writing...");
	debug::saveImg(vec_map.begin(), vec_map.end(), "puyo_field", true);
}