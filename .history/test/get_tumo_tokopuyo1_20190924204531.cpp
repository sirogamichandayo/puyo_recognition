#include "../src/recognition/state_main.h"
#include "../src/recognition/screen_shot.h"
#include "../src/log/log.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <stopWatch/stopWatch.h>
#include <memory>
#include <vector>
#include <string>

int main()
{
	shared_ptr<stopWatchController> timer_con;
	timer_con = make_shared<stopWatchController>();
	std::string file_name = "./time.png";
	timer_con->set_file_name(file_name);
	std::string title1 = "shot";
	std::string title2 = "is_exist";
	std::string title3 = "1p_board";

	unsigned const int SHOT = timer_con->new_timer(title1);
	unsigned const int IS = timer_con->new_timer(title2);
	unsigned const int BOARD = timer_con->new_timer(title3);

	std::vector<int> vec;
	int key = 0;
	ScreenShot src = ScreenShot::getScreenShot("three");
	cv::Mat img;
	State env(player::DEFAULT);
	for (int i = 0; i < 100; ++i)
	{
		timer_con->start(SHOT);
		src >> img;
		env.setImg(img);
		timer_con->lap(SHOT);

		timer_con->start(IS);
		if (env.isGetState(get_mode::existNext_1p));
		timer_con->lap(IS);

		
	}
}