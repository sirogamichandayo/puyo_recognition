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
	vector<int> next_puyo(4);

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

			// delay
			usleep(time);
			env.step();

			std::cout << ++index << std::endl;
		}
		if (index == 100) break;
	}
	LOG("Writing...");
	vector<string> title;
	for (int i = 0; i < 75; ++i)
		title.push_back("puyo" + to_string(i));

	for (int i = 0; i < board_puyo_vec.size(); ++i)
	{
		board_puyo_vec[i].insert(board_puyo_vec[i].end(), next_puyo_vec[i].begin(), next_puyo_vec[i].end());
	}
	auto CSV = pair<vector<string>, vector<vector<int>>>
											(title, board_puyo_vec);

	debug::saveElem(CSV, "fiane_84");
}