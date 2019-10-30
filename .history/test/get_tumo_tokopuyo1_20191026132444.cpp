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
	assert(argc == 2);
	debug::initializeDir("tokopuyo_tumo");
	int time = 300 * 500;

	vector<int> all_puyo(76);
	vector<vector<int>> all_puyo_his;
	all_puyo_his.reserve(50 * 200);

	ScreenShot scr = ScreenShot::getScreenShot("three");

	int player;
	if (argc == 3)
	{
		switch (argv[2])
		{
			case: "kame":
				player = player::KAMESTORY;
				break;
			case: "momoken":
				player = player::MOMOKEN;
				break;
			case: "hisya":
				player = player::HISYA;
				break;
		}
		player = player::DEFAULT;
	}
	State env(&scr, player);
	int index = 0;
	while (true)
	{
		std::string is_start;
		std::cout << "START?(yes or no)" << std::endl;
		std::cout << ">> ";
		std::cin >> is_start;
		if (is_start == "yes" || is_start == "y")
			break;
	}
	for (;;)
	{
		env.step();

		if (env.isGetState(get_mode::isClear))
			break;

		if (!env.isGetState(get_mode::existNext_1p))
		{
			vector<int> board_puyo(72);
			vector<int> next_puyo(4);
			// board puyo
			env.getState(get_mode::boardPuyo_1p, board_puyo);

			// delay
			usleep(time);
			env.step();

			// next puyo
			env.getState(get_mode::nextPuyo_1p, next_puyo);

			// push back all_puyo_his.
			auto begin = all_puyo.begin();
			std::copy(board_puyo.begin(), board_puyo.end(), begin);
			std::copy(next_puyo.begin(), next_puyo.end(), begin+=board_puyo.size());
			all_puyo_his.push_back(all_puyo);

			std::cout << ++index << std::endl;
		}
	}
	LOG("Writing...");
	vector<string> title;
	for (int i = 0; i < 76; ++i)
		title.push_back("puyo_field" + to_string(i));

	auto CSV = pair<vector<string>, vector<vector<int>>>
											(title, all_puyo_his);

	debug::saveElem(CSV, argv[1], true);
}