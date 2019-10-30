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

int main(int argc, char *argv[])
{
#if true
	assert(argc == 2);
	debug::initializeDir(argv[1], true);
#endif
	std::vector<int> board_puyo_1p(72);
	std::vector<int> next_puyo_1p(4);
	ScreenShot s
}