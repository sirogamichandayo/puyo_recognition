#include "../src/recognition/state_main.h"
#include "../src/recognition/screen_shot.h"
#include "../src/log/log.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <stopWatch/stopWatch.h>
#include <memory>
#include <unistd.h>
#include <vector>
#include <string>
#include <map>

using namespace std;

#define CSV pair<vector<string>, vector<vector<int>>>

int main(int argc, char *argv[])
{
	assert(argc == 2);
	debug::initializeDir(argv[1], true);
	ScreenShot scr = ScreenShot::getScreenShot("three");
	State env(&scr, player::DEFAULT);

	std::vector<int> all_puyo_1p_vec(76);
	std::vector<std::vector<int>> all_puyo_1p_vec_vec;
	unsigned long int max_size = all_puyo_1p_vec_vec.max_size();
	all_puyo_1p_vec_vec.reserve(80 * 200);

	for (unsigned long int i = 0; i < 100; ++i)
	{
		env.step();
		if (env.isGetState(get_mode::isClear))
			break;

		if (!env.isGetState(get_mode::existNext_1p))
		{
			env.step();
			env.getState(get_mode::allPuyo_1p, all_puyo_1p_vec);
			all_puyo_1p_vec_vec.push_back(all_puyo_1p_vec);
			usleep(300000);
		}
	}
	vector<string> title;
	for (int i = 0; i < 76; ++i)
	{
		title.push_back(to_string(i));
	}

	LOG("Saving...");
	CSV csv = CSV(title, all_puyo_1p_vec_vec);
	debug::saveElem(csv, "fiane_84");
}