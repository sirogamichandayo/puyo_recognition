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

int main()
{
	ScreenShot scr = ScreenShot::getScreenShot("three");
	State env(&scr, player::DEFAULT);
	debug::initializeDir();
	for (unsigned long int i = 0;;)
	{
		env.step();
		if (!env.isGetState(get_mode::existNext_1p))
			std::cout << ++i % 10 << std::endl;
	}
}