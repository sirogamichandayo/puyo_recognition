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

int main()
{
	ScreenShot src = ScreenShot::getScreenShot("three");
	State env(&src, player::DEFAULT);
	env.saveTeacherImg();
}