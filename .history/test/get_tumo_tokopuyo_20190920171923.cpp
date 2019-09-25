#include "../src/recognition/state_main.h"
#include "../src/recognition/screen_shot.h"
#include "../src/recognition/env_img.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
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
	/*
ファイルを指定。環境を作って
ツモを抽出
ファイルに貯める。
*/
	for (const auto &[path, player] : file_list)
	{
		cv::VideoCapture cap(path);
		EnvImg<cv::VideoCapture> envImg(cap);
		cv::Mat img;
		State env(player);
		while (true)
		{
			// get 
			envImg >> img;
			if(img.empty())
				break;
			env.setImg(img);

			if (env.getState(get_mode::existNext_1p))
			{
				std::vector<int> tumo;
				env.getState(get_mode::allPuyo_1p, tumo);
				// 加える。
			}
		}

	}
}