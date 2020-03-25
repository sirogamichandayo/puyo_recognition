#include "../src/recognition/state_main.h"
#include "../src/recognition/screen_shot.h"
#include "../src/log/log.h"
#include "../src/tools/stopWatch/stopWatch.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"

#include <vector>
#include <string>
#include <iostream>

int main(int argc, char **argv)
{
	//<----------
	// stopWatch setting
	stopWatchController timer_con;

	timer_con.set_format(stopwatch::PLOT);
	timer_con.set_title_name("speed test");
	timer_con.set_file_name("speed_plot3_isExistNext_getAllPuyo_step");

	unsigned const int IS_EXIST = timer_con.new_timer("is_exist");
	unsigned const int GET_ALL_PUYO = timer_con.new_timer("get_all_puyo");
	unsigned const int STEP = timer_con.new_timer("step");
	//---------->

	ScreenShot src(pic::full_hd::X, pic::full_hd::Y,
				   pic::full_hd::WIDTH, pic::full_hd::HEIGHT);
	State env(&src, &pic::full_hd::PIC_RECT_LIST);

	std::vector<int> all_puyo_1p(game::ALL_PUYO_NUM);

	unsigned const int TIMES = 50;
	for (unsigned int i = 0; i < TIMES; ++i)
	{
		timer_con.start(STEP);
		env.step();
		timer_con.lap(STEP);
		timer_con.start(GET_ALL_PUYO);
		env.getState(get_mode::ALL_PUYO_1P, &all_puyo_1p);
		timer_con.lap(GET_ALL_PUYO);
		timer_con.start(IS_EXIST);
		env.isGetState(get_mode::IS_EXIST_NEXT_1P);
		timer_con.lap(IS_EXIST);
	}
	
}
