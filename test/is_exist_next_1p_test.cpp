#include "../src/recognition/state_main.h"
#include "../src/recognition/screen_shot.h"
#include "../src/log/log.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"

#include <vector>
#include <string>
#include <iostream>

void print(const int &num)
{
	for (int i = 0; i < num; ++i)
	{
		std::cout << "=";
	}
	std::cout << std::endl;
}

int main(int argc, char **argv)
{
	ScreenShot src(pic::full_hd::X, pic::full_hd::Y,
				   pic::full_hd::WIDTH, pic::full_hd::HEIGHT);
	State env(&src, &pic::full_hd::PIC_RECT_LIST);

	int index = 0;
	while (true)
	{
		env.step();
		if (!env.isGetState(get_mode::IS_EXIST_NEXT_1P))
		{
			print(index++%10);
		}
	}
}

