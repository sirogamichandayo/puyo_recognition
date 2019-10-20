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

int main()
{
	cv::Mat img;
	if (!cv::imwrite("/mnt/programming/data/MO/tokopuyo/recognition_data11/next_1p/next15.png", img))
		std::cout << "a" << std::endl;
	cv::imshow("test", img);
	cv::waitKey(100000);
}