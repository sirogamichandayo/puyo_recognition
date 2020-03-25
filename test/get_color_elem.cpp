#include "../src/recognition/state_main.h"
#include "../src/recognition/screen_shot.h"
#include "../src/log/log.h"
#include "../src/tools/debug/debug.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"

#include <vector>
#include <string>
#include <iostream>

namespace pic_ = pic::full_hd;

int main(int argc, char **argv)
{
	debug::initializeDir("/home/sirogami/programming/cota_Puyoai/screenshot_check/");

	ScreenShot src(pic_::X, pic_::Y,
				   pic_::WIDTH, pic_::HEIGHT);
	State env(&src, &pic_::PIC_RECT_LIST);

	std::vector<int> all_puyo_1p(game::ALL_PUYO_NUM);
	env.step();
	env.getState(get_mode::ALL_PUYO_1P, &all_puyo_1p);
}
