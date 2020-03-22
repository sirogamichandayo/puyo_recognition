#include "../src/recognition/state_main.h"
#include "../src/recognition/screen_shot.h"
#include "../src/log/log.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"

int main(int argc, char **argv)
{
	ScreenShot scr(screenShot::X, screenShot::Y,
				   screenShot::WIDTH, screenShot::HEIGHT);
	State env(&scr);
	cv::Mat img;
	env.getImg(&img);
	
}
