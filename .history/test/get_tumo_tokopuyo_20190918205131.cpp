#include "../recognition/state_main.h"
#include "../recognition/screen_shot.h"
#include "../recognition/env_img.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <string>
#include <map>

// <path, player>
std::map<std::string, int> file_list
{
	{"/mnt/programming/data/MO/tokopuyo/test/test2.mp4", player::DEFAULT},
}

int main()
{
	/*
ファイルを指定。環境を作って
ツモを抽出
ファイルに貯める。
*/
	for (const auto &[path, player] : file_list)
	{
		VideoCapture cap = new VideoCapture(path);
		EnvImg envImg(cap);
		State env(envImg, player);
		while()
		{
			// get 
			if (!env.step())
				break;

			if (env.isExistNext_1p())
			{
				std::vector<int> tumo;
				env.getState(get_mode::allPuyo_1p, tumo);
				// 加える。
			}
		}

		delete cap;
	}
}