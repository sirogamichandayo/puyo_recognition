#ifndef _SCREENSHOT_H_
#define _SCREENSHOT_H_

#include "../log/log.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdexcept>
#include <iostream>
#include <string>

namespace pic
{
	const int HD_WIDTH = 1920;
	const int HD_HEIGHT = 1080;

	const int PUYO_WIDTH = 63;
	const int PUYO_HEIGHT = 58;
	const int SMALL_PUYO_WIDTH = 50;
	const int SMALL_PUYO_HEIGHT = 46;

	const cv::Rect result_1p_rect = cv::Rect(280, 400, 350, 70); // Win, Lose, Draw
	const cv::Rect result_2p_rect = cv::Rect(1256, 400, 350, 70);
	const cv::Rect fight_1p_rect = cv::Rect(620, 950, 100, 52); // is fighting.
	const cv::Rect fight_2p_rect = cv::Rect(1190, 950, 100, 52);
	const cv::Rect finish_rect = cv::Rect(475, 420, 950, 220); // is finish of fight.
	const cv::Rect clear_rect  = finish_rect;
	const cv::Rect board_1p_rect = cv::Rect(280, 160, 384, 720);
	const cv::Rect board_2p_rect = cv::Rect(1256, 160, 384, 720);
	const cv::Rect next1_1p_rect = cv::Rect(431, 0, pic::PUYO_WIDTH, pic::PUYO_HEIGHT * 2);
	const cv::Rect next2_1p_rect = cv::Rect(765, 129, pic::SMALL_PUYO_WIDTH, pic::SMALL_PUYO_HEIGHT * 2);
	const cv::Rect next1_2p_rect = cv::Rect(837, 0, pic::PUYO_WIDTH, pic::PUYO_HEIGHT * 2);
	const cv::Rect next2_2p_rect = cv::Rect(799, 129, pic::SMALL_PUYO_WIDTH, pic::SMALL_PUYO_HEIGHT * 2);
	const cv::Rect is_next_1p_rect = next1_1p_rect;
	const cv::Rect is_next_2p_rect = next1_2p_rect;
	// board_and_next_2p;

	// const cv::Rect momoken = cv::Rect(0, 0, 1608, 905);
	// // const cv::vector<int> delta()
	// const cv::Rect hisya = cv::Rect(18, 104, 1570, 883);
	// const cv::Rect kamestry = cv::Rect(4, 3, 1746, 984);
	// const cv::Rect normal = cv::Rect(0, 0, 1920, 1080);

}; // namespace pic

namespace screenShot
{
	const int X = 1374;
	const int Y = 186;
	const int WIDTH = 1330;
	const int HEIGHT = 696;
};

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

	void operator >> (cv::Mat &cvImg)
	{
		if (init == true)
			init = false;
		else
			XDestroyImage(img);

		img = XGetImage(display, root, x, y, width, height, AllPlanes, ZPixmap);
		cvImg = cv::Mat(height, width, CV_8UC4, img->data);
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
