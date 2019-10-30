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
	ScreenShot scr(1080, 0, 1920, 1080);
	std::vector<int> all_puyo(72);

	State env(scr, player::DEFAULT);
	for (int i = 0; i < 50; ++i)
	{
		env.step();
		env.getState(get_mode::allPuyo_1p, all_puyo);
		std::cout << i << std::endl;
	}
	std::vector<int> ().swap(all_puyo);
	LOG("fin");
}