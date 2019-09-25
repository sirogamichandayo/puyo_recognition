#include "../src/recognition/state_main.h"
#include "../src/recognition/screen_shot.h"
#include "../src/log/log.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <vector>
#include <string>
#include <map>

// <path, player>
std::map<std::string, int> file_list = 
{
	{"/mnt/programming/data/MO/tokopuyo/test/test2.mp4", player::DEFAULT}
};

int main()
{
	int delay = 100;
	/*
ファイルを指定。環境を作って
ツモを抽出
ファイルに貯める。
*/
	for (const auto &[path, player] : file_list)
	{
		cv::VideoCapture cap(path);
		cv::Mat img;
		cv::Mat img_hsv;
		State env(player);
		while (true)
		{
			// get 
			cap >> img;
			if(img.empty())
				break;
			env.setImg(img);
			/* wanna
			env.step();
			*/
			if (env.isGetState(get_mode::existNext_1p))
				std::cout << "true" << std::endl;
		}
	}
}