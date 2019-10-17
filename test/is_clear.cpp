#include "../src/recognition/state_main.h"
#include "../src/recognition/screen_shot.h"
#include "../src/log/log.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <unistd.h>
#include <memory>
#include <vector>
#include <string>
#include <map>

int main()
{
	HSV hsv;
	ScreenShot scr = ScreenShot::getScreenShot("three");
	debug::initializeDir();
	cv::Mat img;
	scr >> img;
	img = img(pic::clear);
	cv::cvtColor(img, img, cv::COLOR_BGR2HSV);
	cv::resize(img, img, cv::Size(), 0.1, 0.1);
	for (int y = 0; y < img.rows; ++y)
	{
		cv::Vec3b *p = &img.at<cv::Vec3b>(y, 0);
		for (int x = 0; x < img.cols; ++x, ++p)
		{
		/*	
			hsv((int)(*p)[0], (int)(*p)[1], (int)(*p)[2]);
			hsv.show_all();
			*/
			vec = {(int)(*p)[0], (int)(*p)[1], (int)(*p)[2]};
			std::cout << "H : " << (int)(*p)[0] * 2 << ", ";
			std::cout << "S : " << (int)(*p)[1] << ", ";
			std::cout << "V : " << (int)(*p)[2] << std::endl;
			data.push_back(vec);
		}
	}
	auto CSVDATA = std::pair<std::vector<std::string>, std::vector<std::vector<int>>>
			(title, data);
	debug::saveElem(CSVDATA, "clear");
}