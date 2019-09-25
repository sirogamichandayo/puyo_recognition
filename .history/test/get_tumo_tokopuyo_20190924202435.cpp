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

// <path, player>
std::map<std::string, int> file_list = 
{
	{"/mnt/programming/data/MO/tokopuyo/test/test2.mp4", player::DEFAULT}
};

int main()
{
	/*
ファイルを指定。環境を作って
ツモを抽出
ファイルに貯める。
*/

	shared_ptr<stopWatchController> timer_con;
	timer_con = make_shared<stopWatchController>();
	std::string file_name = "./time.png";
	timer_con->set_file_name(file_name);
	std::string title1 = "shot";
	std::string title2 = "is_exist";

	unsigned const int SHOT = timer_con->new_timer(title1);
	unsigned const int IS = timer_con->new_timer(title2);

	for (const auto &[path, player] : file_list)
	{
		int key = 0;
		cv::VideoCapture cap(path);
		cv::Mat img;
		cv::Mat img_hsv;
		State env(player);
		for (int i = 0; i < 100; ++i)
		{
			// get 
			timer_con->start(SHOT);
			cap >> img;
			if(img.empty())
				break;
			env.setImg(img);
			timer_con->lap(SHOT);
			/* wanna
			env.step();
			*/
			timer_con->start(IS);
			bool test = env.isGetState(get_mode::existNext_1p);
			timer_con->lap(IS);

			}
		}
	}
}