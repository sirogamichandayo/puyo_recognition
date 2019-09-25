#include "../src/recognition/state_main.h"
#include "../src/recognition/screen_shot.h"
#include "../src/log/log.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <memory>
#include <vector>
#include <string>

int main()
{
	std::vector<int> vec;
	int key = 0;
	ScreenShot src = ScreenShot::getScreenShot("three");
	cv::Mat img;
	State env(player::DEFAULT);
	for (int i = 0; i < 1; ++i)
	{
		src >> img;
		env.setImg(img);

		env.getState(get_mode::allPuyo_1p, vec);
		for (auto const elem : vec)
		{
			std::cout << elem;
		}
	}
}