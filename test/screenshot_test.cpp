#include "../src/recognition/state_main.h"
#include "../src/recognition/screen_shot.h"
#include "../src/log/log.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"

#include <vector>
#include <string>

namespace pic_ = pic::full_hd;

int main(int argc, char **argv)
{
	ScreenShot scr(pic_::X, pic_::Y,
				   pic_::WIDTH, pic_::HEIGHT);
	State env(&scr, &pic_::PIC_RECT_LIST);
	std::vector<int> all_puyo_1p(game::ALL_PUYO_NUM);

	env.step();
	cv::Mat img;
	env.getImg(&img);
	debug::showForDebug(img, 0, true);
}
