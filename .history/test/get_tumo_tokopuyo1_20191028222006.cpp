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
	debug::initializeDir(argv[1]);
	int time = 300 * 500;

	vector<int> all_puyo(76);
	vector<vector<int>> all_puyo_his;
	all_puyo_his.reserve(50 * 200);

	ScreenShot scr = ScreenShot::getScreenShot("three");

	int player;
	if (argc == 3)
	{
		if (argv[2] == "kame")
		{
			player = player::KAMESTORY;
		}
		else if (argv[2] == "momoken")
		{	
			player = player::MOMOKEN;
		}
		else if (argv[2] == "kame")
		{
			player = player::KAMESTORY;
		}
		else if (argv[2] == "hisya")
		{
			player = player::HISYA;
		}
	}
	else /*argc != 3*/
	{
		player = player::DEFAULT;
	}
	State env(&scr, player);

	while (true)
	{
		std::string is_start;
		std::cout << "START?(yes or no)" << std::endl;
		std::cout << ">> ";
		std::cin >> is_start;
		if (is_start == "yes" || is_start == "y")
			break;
	}

	std::map<std::string, cv::Mat> save_img;
	int index = 0;
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

			// stock img for save.
			std::string title = "puyo_field" + std::to_string(index);
			cv::Mat image(env.getImg(), pic::board_and_next_1p);
			cv::resize(image, image, cv::Size(), 0.5, 0.5);
			save_img.insert(std::pair<std::string, cv::Mat>(title, image);

			std::cout << ++index << std::endl;
		}
		if (index == 10) break;
	}
	LOG("Writing...");
	// save tumo.
	vector<string> title;
	for (int i = 0; i < 76; ++i)
		title.push_back("puyo_field" + to_string(i));

	auto CSV = pair<vector<string>, vector<vector<int>>>
											(title, all_puyo_his);

	debug::saveElem(CSV, "tumo", true);

	// save img;
	debug::saveImg(save_img.begin(), save_img.end(), "field_img");
}