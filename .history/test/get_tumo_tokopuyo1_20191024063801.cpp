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

using namespace std;
int main(int argc, char *argv[])
{
	debug::initializeDir("tokopuyo_tumo");
	int time = 300 * 500;

	vector<int> board_puyo(72);

	vector<vector<int>> vec_vec;
	vec_vec.reserve(50 * 200);

	ScreenShot scr = ScreenShot::getScreenShot("three");
	State env(&scr, player::DEFAULT);
	int index = 0;
	for (;;)
	{
		env.step();

		if (env.isGetState(get_mode::isClear))
			break;

		if (!env.isGetState(get_mode::existNext_1p))
		{
			// board puyo
			env.getState(get_mode::boardPuyo_1p, board_puyo);
			vec_vec.push_back(board_puyo);

			// delay
			usleep(time);
			env.step();

			std::cout << ++index << std::endl;
		}
		if (index == 100) break;
	}
	LOG("Writing...");
	vector<string> title;
	for (int i = 0; i < 72; ++i)
		title.push_back("puyo_field" + to_string(i));

	auto CSV = pair<vector<string>, vector<vector<int>>>
											(title, vec_vec);

	debug::saveElem(CSV, "fiane_board_84");
}