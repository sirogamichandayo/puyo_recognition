#define DEBUG_YELLOW 1

#include "../src/recognition/state_main.h"
#include "../src/recognition/screen_shot.h"
#include "../src/log/log.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"

#include <vector>
#include <string>
#include <iostream>

namespace pic_ = pic::gst;

int main(int argc, char **argv)
{
	debug::initializeDir("/home/sirogami/programming/cota_Puyoai/screenshot_check/gst_yellow");
	
	ScreenShot scr(pic_::X, pic_::Y,
				   pic_::WIDTH, pic_::HEIGHT);
	State env(&scr, &pic_::PIC_RECT_LIST);

	std::vector<int> all_puyo_1p(game::ALL_PUYO_NUM);
	std::vector<std::string> all_puyo_str_1p(game::ALL_PUYO_NUM);

	env.step();
	env.getState(get_mode::ALL_PUYO_1P, &all_puyo_1p);
	env.bitNum2ColorStringForVec(all_puyo_1p, &all_puyo_str_1p);
	cout << game::ALL_PUYO_NUM << endl;

	for (unsigned int i = 1; i <= game::ALL_PUYO_NUM; ++i)
	{

		std::cout << i << ": " << all_puyo_str_1p[i-1]<< std::endl;
		if (i % 12 == 0)
			std::cout << "------------------------------" << std::endl;
	}
		
}
