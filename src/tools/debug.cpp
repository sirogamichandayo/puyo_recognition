#include "./debug.h"

// existExstension("XXXX", ".AAA") : return false.
// existExstension("XXXX.AAA", ".AAA") : return true.
// existExstension("XXXXB", "B") : return true. (not exstension)
// Use B is "/(directory)".
// TODO: solve error.
// existExstension("XX", ".AAA") // error
// if (orig_size < exte_size) error
bool debug::existExstension(const std::string& original, const std::string& extension)
{
	if ('.' != extension[0] && "/" != extension)
		LOG("Waring : Is \'.\' not necessary? Extension argument is " + extension);
			
	int orig_size = original.size();
	int exte_size = extension.size();
	return original.substr(orig_size-exte_size, orig_size) == extension;
}


void debug::initializeDir()
{
	if (fs::exists(debug::DIR_PATH))
	{
		fs::remove_all(debug::DIR_PATH);
	}

	if (debug::makeDir(debug::DIR_PATH) == -1)
		LOG("To make dir (name: \"" + debug::DIR_PATH + "\") failed.");
}

bool debug::makeDir(const std::string& dir_name)
{
	return fs::create_directory(dir_name);
}

bool debug::fileExists(const std::string& str)
{
	return fs::exists(str);
}

void debug::showForDebug(const std::vector<cv::Mat> &img_vec, const unsigned int wait, const bool is_hsv)
{
	for (const auto& img_ : img_vec)
	{
		debug::showForDebug(img_, wait, is_hsv);
	}
}
	
void debug::showForDebug(const cv::Mat &image, const unsigned int wait, const bool is_hsv)
{
	cv::Mat img_rgb;
	if (is_hsv)
		cv::cvtColor(image, img_rgb, CV_HSV2BGR);
	else 
		img_rgb = image;
	cv::imshow("debug", img_rgb);
	cv::waitKey(wait);
}
