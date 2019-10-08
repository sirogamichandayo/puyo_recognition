#include "./debug.h"

bool debug::existExstension(const std::string& original, const std::string& extension)
{
	if ('.' != extension[0] && "/" != extension)
		LOG("Waring : Is \'.\' not necessary? Extension argument is " + extension);
			
	int orig_size = original.size();
	int exte_size = extension.size();
	return original.substr(orig_size-exte_size, orig_size) == extension;
}
	
bool debug::makeDir(const std::string& dir_name)
{
	char *path = new char[256];
	strcpy(path, (debug::DIR_PATH + dir_name).c_str());
	bool result = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
	delete []path;
	return result;
}

void debug::showForDebug(const std::vector<cv::Mat> &img_vec, int wait, bool is_hsv)
{
	cv::Mat image;
	for (const auto& img_ : img_vec)
	{
		debug::showForDebug(img_, wait, is_hsv);
	}
}
	
void debug::showForDebug(const cv::Mat &image, int wait, bool is_hsv)
{
	cv::Mat img_rgb;
	cv::cvtColor(image, img_rgb, CV_HSV2BGR);
	cv::imshow("debug", img_rgb);
	cv::waitKey(1000);
}
