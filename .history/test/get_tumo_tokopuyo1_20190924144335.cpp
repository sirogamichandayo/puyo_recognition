#include "../src/recognition/state_main.h"
#include "../src/recognition/screen_shot.h"
#include "../src/log/log.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <vector>
#include <string>

int main()
{
	int key = 0;
	ScreenShot src = ScreenShot::getScreenShot("three");
	cv::Mat img;
	State env(player::DEFAULT);
	for (int i = 0; i < 5000; ++i)
	{
		src >> img;
		env.setImg(img);
		if (i == 0)
			std::cout << "cols : " << img.cols << std::endl;
			std::cout << "rows : " << img.rows << std::endl;

		if (!env.isGetState(get_mode::existNext_1p))
		{
			std::cout << key << std::endl;
			key = ++key % 10;
		}
	}
}