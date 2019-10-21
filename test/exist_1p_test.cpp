#include "../src/recognition/state_main.h"
#include "../src/recognition/screen_shot.h"
#include "../src/log/log.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <stopWatch/stopWatch.h>
#include <memory>
#include <unistd.h>
#include <vector>
#include <string>
#include <map>

using namespace std;

int main(int argc, char *argv[])
{
	assert(argc == 2);
	debug::initializeDir(argv[1], true);

	ScreenShot scr = ScreenShot::getScreenShot("three");
	State env(&scr, player::DEFAULT);
	std::map<std::string, cv::Mat> img_map;

	int index = 0;
	for (;;)
	{
		env.step();
		if (!env.isGetState(get_mode::existNext_1p))
		{
			for (;;)
			{
				env.step();
				if (env.isGetState(get_mode::existNext_1p))
				{
					usleep(1000);
					env.step();
					string title = "next" + to_string(index++);
					img_map.insert(pair<string, cv::Mat>(title, env.getImg()));
					usleep(300000);
					break;
				}
			}
		}
		if (index == 10) break;
	}
	debug::saveImg(img_map.begin(), img_map.end(), "next_1p");
}