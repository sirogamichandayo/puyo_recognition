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
	
	for (;;)
	{
		env.step());
		if (env.isGetState(get_mode::existNext_1p))
		{
			env.getState(get_mode::boardPuyo_1p, board_puyo_1p);
			
		}
	}
}