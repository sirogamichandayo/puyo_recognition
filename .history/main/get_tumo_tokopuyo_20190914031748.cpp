#include "../recognition/state_main.h"
#include "../recognition/screen_shot.h"
#include "../recognition/env_img.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <string>
#include <map>

std::map<std::string, std::string> file_list {
	{"/mnt/programming/data/MO/tokopuyo/test/test2.mp4", ""}	
}


int main()
{
/*
ファイルを指定。環境を作って
ツモを抽出
ファイルに貯める。
*/	
	for (const auto& [path, player] : file_list)
	{
		VideoCapture cap = new VideoCapture(path);
		State env(cap, player);
		while (1)
		{
			env(); // get image from capture.
			for (env.isExistNext_1p())
			{

			}
		}
		delete cap		
	}
}