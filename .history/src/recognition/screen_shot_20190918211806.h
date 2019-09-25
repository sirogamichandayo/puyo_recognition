#ifndef _SCREENSHOT_H_
#define _SCREENSHOT_H_

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <string>

namespace pic
{
const int PUYO_WIDTH = 64;
const int PUYO_HEIGHT = 60;
const int SMALL_PUYO_WIDTH = 50;
const int SMALL_PUYO_HEIGHT = 47;


const std::vector<string> monitor = {"one", "three"};
const cv::Rect issue_1p = cv::Rect(280, 400, 350, 70); // Win, Lose, Draw
const cv::Rect issue_2p = cv::Rect(1256, 400, 350, 70);
const cv::Rect fight_1p = cv::Rect(620, 950, 100, 52); // is fighting.
const cv::Rect fight_2p = cv::Rect(1190, 950, 100, 52);
const cv::Rect finish = cv::Rect(475, 420, 950, 220); // is finish of fight.
const cv::Rect board_1p = cv::Rect(280, 160, 384, 720);
const cv::Rect board_2p = cv::Rect(1256, 160, 384, 720);
const cv::Rect next_1p = cv::Rect(720, 160, pic::PUYO_WIDTH, pic::PUYO_HEIGHT * 2);
const cv::Rect next_2p = cv::Rect(1135, 160, pic::PUYO_WIDTH, pic::PUYO_HEIGHT * 2);
const cv::Rect next2_1p = cv::Rect(771, 295, pic::SMALL_PUYO_WIDTH, pic::SMALL_PUYO_HEIGHT * 2);
const cv::Rect next2_2p = cv::Rect(1098, 295, pic::SMALL_PUYO_WIDTH, pic::SMALL_PUYO_HEIGHT * 2);
const cv::Rect is_next_1p = cv::Rect(720, 160, pic::PUYO_WIDTH, pic::PUYO_HEIGHT);
const cv::Rect is_next_2p = cv::Rect(1135, 160, pic::PUYO_WIDTH, pic::PUYO_HEIGHT);

const cv::Rect momoken = cv::Rect(0, 0, 1608, 905);
// const cv::vector<int> delta()
const cv::Rect hisya = cv::Rect(18, 104, 1570, 883);
const cv::Rect kamestry = cv::Rect(4, 3, 1746, 984);
const cv::Rect normal = cv::Rect(0, 0, 1080, 1920);

}; // namespace pic

class ScreenShot
{
public:
	ScreenShot(int x, int y, int width, int height) : x(x),
																										y(y),
																										width(width),
																										height(height)
	{
		display = XOpenDisplay(nullptr);
		root = DefaultRootWindow(display);

		init = true;
	};

	static ScreenShot getScreenShot(String str)
	{

		// find monitor setting from std::vector<string> pic::monitor
		auto it = std::find(pic::monitor.begin(), pic::monitor.end(), str);
		if (it == pic::monitor.end())
		{
			std::cout << "Could not be find monitor setting." << std::endl;
			std::cout << "Please confirm command line argument or  " << std::endl;
			std::cout << "<line 29> pic::vector<string> pic::monitor." << std::endl;
		}
		// 'it' points to the second element of the vector, 'index' is 1

		if (str == pic::monitor[0])
			return ScreenShot(0, 0, 1920, 1080);

		if (str == pic::monitor[1])
			return ScreenShot(1080, 0, 1920, 1080);

		// Compiler says "control may reach end of non-void function."
		// But there is no such pattern.
	};

	void operator >> (cv::Mat &cvImg)
	{
		if (init == true)
			init = false;
		else
			XDestroyImage(img);

		img = XGetImage(display, root, x, y, width, height, AllPlanes, ZPixmap);
		cvImg = Mat(height, width, CV_8UC4, img->data);
	};

	~ScreenShot()
	{
		if (init == false)
			XDestroyImage(img);

		XCloseDisplay(display);
	};

private:
	Display *display;
	Window root;
	int x, y, width, height;
	XImage *img;

	bool init;
};

#endif // _SCREENSHOT_H_
