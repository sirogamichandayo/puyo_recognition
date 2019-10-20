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

int main()
{
	ScreenShot scr = ScreenShot::getScreenShot("three");
	State env(&scr, player::DEFAULT);
	std::map<std::string, cv::Mat> img_map;
	debug::initializeDir();
	for (int i = 0; i < 100; ++i)
	{
		env.step();
		env.isGetState(get_mode::existNext_1p);
		cv::Mat next_1p(env.getImg(), pic::is_next_1p);
		std::string title = "next" + std::to_string(i);
		img_map.insert(std::pair<std::string, cv::Mat>(title, next_1p));
		
	}
	debug::saveImg(img_map.begin(), img_map.end(), "next_1p");
}