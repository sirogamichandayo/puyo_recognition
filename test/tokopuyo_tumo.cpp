#include "../src/recognition/state_main.h"
#include "../src/recognition/screen_shot.h"
#include "../src/log/log.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <stopWatch/stopWatch.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <string>

const std::string DIR_PATH = "/mnt/programming/data/MO/tokopuyo/tomo_flane";
const std::string FILE_PATH = "1.csv";

void save(const std::vector<std::vector<int>>& vec_vec, bool first)
{
	std::ofstream write_file;
	if (first)
		write_file.open(DIR_PATH+FILE_PATH, std::ios::trunc);
	else
		write_file.open(DIR_PATH+FILE_PATH, std::ios_base::app);

	std::ostringstream ss;
	for (const auto &vec : vec_vec)
	{
		for (const auto &elem : vec)
		{
			ss << (std::to_string(elem) + ',');
		}
		ss << '\n';
	}
	write_file << ss.str();
}		

int main()
{
	std::ofstream write_file;
	ScreenShot src = ScreenShot::getScreenShot("three");
	State env(player::DEFAULT);
	cv::Mat img;
	int index = 0;
	int save_freq = 100;
	std::vector<int> all_puyo_vec(game::BOARD_ROWS_NO_IN_1314*game::BOARD_COLS+
														game::NEXT1_ROWS*game::NEXT1_COLS+
														game::NEXT2_ROWS*game::NEXT2_ROWS);
  std::vector<std::vector<int>> all_puyo_vec_vec(save_freq);
	bool first = true;

	while (true)
	{
		src >> img;
		env.setImg(img);

		if (!env.isGetState(get_mode::existNext_1p))
		{
			env.getState(get_mode::allPuyo_1p, all_puyo_vec);
			all_puyo_vec_vec.push_back(all_puyo_vec);
			std::cout << index << std::endl;
			if (++index % save_freq == 0)
			{
				save(all_puyo_vec_vec, first);
				first &= false;
			}
			stopWatchController::wait(100000);
				
		}
	}
	return 0;
}
