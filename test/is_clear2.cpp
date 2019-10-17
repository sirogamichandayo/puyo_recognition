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

int main(int argc, char* argv[])
{
	int time = 7000 * 1000;
	ScreenShot scr = ScreenShot::getScreenShot("three");
	State env(&scr, player::DEFAULT);
	int index = 0;
	for (;;)
	{
		env.step();
		if (env.isGetState(get_mode::isClear))
		{
			std::cout << "###############" << std::endl;
			for (;;)
			{
				env.step();
				if (!env.isGetState(get_mode::isClear)) break;
			}
		}
	}
}