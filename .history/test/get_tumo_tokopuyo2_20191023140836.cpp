#include "../src/recognition/state_main.h"
#include "../src/recognition/screen_shot.h"
#include "../src/log/log.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <stopWatch/stopWatch.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <string>

int main(int argc, char *argv[])
{
#if false
	assert(argc == 2);
	debug::initializeDir(argv[1], /*override*/true);
#endif

	using namespace game;
	int all_puyo_size = BOARD_ROWS_NO_IN_1314 * BOARD_COLS +
														NEXT1_ROWS * NEXT2_COLS + 
														NEXT2_ROWS * NEXT2_COLS;
	std::vector<int> all_puyo(all_puyo_size);
	std::vector<std::string> all_puyo_vec(all_puyo_size);
	ScreenShot scr = ScreenShot::getScreenShot("three");

	shared_ptr<stopWatchController> con;
	con = make_shared<stopWatchController>();
	std::string file_name = "vec_tmp.png";
	con->set_file_name(file_name);
	std::string test = "test";
	unsigned const int TEST = con->new_timer(test);

	cv::Mat img;
	State env(&scr, player::DEFAULT);
	int index = 0;
	for (int i = 0; i < 1; ++i)
	{
		env.step();
		con->start(TEST);
		env.getState(get_mode::allPuyo_1p, all_puyo, true);
		con->lap(TEST);
		env.colorNum2ColorStringForVec(all_puyo, &all_puyo_vec);
		for (const auto &elem : all_puyo_vec)
		{
			std::ostringstream ss;
			ss << std::setw(3) << std::setfill('0') << index++ << " : " << elem;
			std::cout << ss.str() << std::endl;
			if (index % 12 == 0)
				std::cout << std::endl;
		}
	}
}