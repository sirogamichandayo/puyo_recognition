#include "../src/recognition/state_main.h"
#include "../src/recognition/screen_shot.h"
#include "../src/log/log.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <stopWatch/stopWatch.h>
#include <sstream>
#include <memory>
#include <vector>
#include <string>

int main()
{
	shared_ptr<stopWatchController> con;
	con = make_shared<stopWatchController>();
	std::string file_name = "time1.png";
	con->set_file_name(file_name);
	std::string allPuyo = "get_all_puyo";
	std::string allPuyo_2 = "get_all_puyo";

	unsigned const int GET = con->new_timer(allPuyo);
	unsigned const int GET_2 = con->new_timer(allPuyo_2);



	std::vector<int> vec;
	int key = 0;
	ScreenShot src = ScreenShot::getScreenShot("three");
	cv::Mat img;
	State env(player::DEFAULT);
	for (int i = 0; i < 10; ++i)
	{
		src >> img;
		env.setImg(img);

		con->start(GET);
		env.getState(get_mode::allPuyo_1p, vec);
		con->start(GET_2);
		// process
		con->lap(GET_2);
		con->lap(GET);
	}
}