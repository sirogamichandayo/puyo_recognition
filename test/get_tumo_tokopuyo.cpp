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
	/*
ファイルを指定。環境を作って
ツモを抽出
ファイルに貯める。
*/
	// timer set.
	shared_ptr<stopWatchController> timer_con;
	timer_con = make_shared<stopWatchController>();
	std::string file_name_time = "test_move.png";
	timer_con->set_file_name(file_name_time);
	std::string title1 = "step";
	std::string title2 = "is_exist";
	std::string title3 = "get_all_puyo_1p";

	unsigned const int STEP = timer_con->new_timer(title1);
	unsigned const int IS_NEXT = timer_con->new_timer(title2);
	unsigned const int GET_ALL = timer_con->new_timer(title3);

	using namespace game;
	int all_puyo_size = BOARD_ROWS_NO_IN_1314 * BOARD_COLS +
														NEXT1_ROWS * NEXT2_COLS + 
														NEXT2_ROWS * NEXT2_COLS;
	std::vector<int> all_puyo(all_puyo_size);
	
	ScreenShot scr = ScreenShot::getScreenShot("three");
	State env(&scr, player::DEFAULT);
	debug::initializeDir();
	for (int i = 0; i < 100; ++i)
	{
		timer_con->start(STEP);
		env.step();
		timer_con->lap(STEP);

		timer_con->start(IS_NEXT);
		env.isGetState(get_mode::existNext_1p);
		timer_con->lap(IS_NEXT);
		
		timer_con->start(GET_ALL);
		env.getState(get_mode::allPuyo_1p, all_puyo);
		timer_con->lap(GET_ALL);
	}
	timer_con->show(STEP);
	env.bitNum2ColorNumForVec(&all_puyo);
	std::vector<std::string> all_puyo_str(all_puyo_size);
	env.colorNum2ColorStringForVec(all_puyo, &all_puyo_str);
	int index = 0;
	for (const auto &elem : all_puyo_str)
	{
		std::cout << index++ << " : "  << elem << std::endl;
	}
}