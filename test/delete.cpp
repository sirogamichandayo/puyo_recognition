#include "../src/recognition/state_main.h"
#include "../src/recognition/screen_shot.h"
#include "../src/log/log.h"
#include "../src/tools/debug/debug.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"

#include <vector>
#include <string>
#include <iostream>

namespace pic_ = pic::gst;

int main(int argc, char **argv)
{
	ScreenShot scr(pic_::X, pic_::Y,
				   pic_::WIDTH, pic_::HEIGHT);
	State env(scr, pic_::PIC_RECT_LIST, pic_::RED_PUYO_IMG, pic_::YELLOW_PUYO_IMG);

	std::vector<int> all_puyo_1p(game::ALL_PUYO_NUM), all_puyo_2p(game::ALL_PUYO_NUM);
	env.step();
	env.getState(get_mode::ALL_PUYO_1P, &all_puyo_1p);
}
