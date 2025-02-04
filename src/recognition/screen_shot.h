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
#include <map>

namespace pic
{
	constexpr int RESULT_1P_RECT_I = 0;
	constexpr int RESULT_2P_RECT_I = 1;
	constexpr int FIGHT_1P_RECT_I = 2;
	constexpr int FIGHT_2P_RECT_I = 3;
	constexpr int FINISH_RECT_I = 4;
	constexpr int CLEAR_RECT_I = 5;
	constexpr int BOARD_1P_RECT_I = 6;
	constexpr int BOARD_2P_RECT_I = 7;
	constexpr int NEXT1_1P_RECT_I = 8;
	constexpr int NEXT2_1P_RECT_I = 9;
	constexpr int NEXT1_2P_RECT_I = 10;
	constexpr int NEXT2_2P_RECT_I = 11;
	constexpr int IS_NEXT_1P_RECT_I = 12;
	constexpr int IS_NEXT_2P_RECT_I = 13;
	
	constexpr int HD_WIDTH = 1920;
	constexpr int HD_HEIGHT = 1080;
	namespace gst
	{
		const std::string DIR_PATH = "../data/gst/puyo_img/";
		
		constexpr int PUYO_WIDTH = 63;
		constexpr int PUYO_HEIGHT = 58;
		constexpr int SMALL_PUYO_WIDTH = 50;
		constexpr int SMALL_PUYO_HEIGHT = 46;

		constexpr int X = 294 + 1080;
		constexpr int Y = 186;
		constexpr int WIDTH = 1330;
		constexpr int HEIGHT = 696;

		const cv::Rect RESULT_1P_RECT = cv::Rect(280, 400, 350, 70); // Win, Lose, Draw
		const cv::Rect RESULT_2P_RECT = cv::Rect(1256, 400, 350, 70);
		const cv::Rect FIGHT_1P_RECT = cv::Rect(620, 950, 100, 52); // is fighting.
		const cv::Rect FIGHT_2P_RECT = cv::Rect(1190, 950, 100, 52);
		const cv::Rect FINISH_RECT = cv::Rect(475, 420, 950, 220); // is finish of fight.
		const cv::Rect CLEAR_RECT  = FINISH_RECT;
		const cv::Rect BOARD_1P_RECT = cv::Rect(0, 0, 378, 696);
		const cv::Rect BOARD_2P_RECT = cv::Rect(952, 0, 378, 696);
		const cv::Rect NEXT1_1P_RECT = cv::Rect(431, 0, pic::gst::PUYO_WIDTH, pic::gst::PUYO_HEIGHT * 2);
		const cv::Rect NEXT2_1P_RECT = cv::Rect(482, 129, pic::gst::SMALL_PUYO_WIDTH, pic::gst::SMALL_PUYO_HEIGHT * 2);
		const cv::Rect NEXT1_2P_RECT = cv::Rect(837, 0, pic::gst::PUYO_WIDTH, pic::gst::PUYO_HEIGHT * 2);
		const cv::Rect NEXT2_2P_RECT = cv::Rect(799, 129, pic::gst::SMALL_PUYO_WIDTH, pic::gst::SMALL_PUYO_HEIGHT * 2);
		const cv::Rect IS_NEXT_1P_RECT = cv::Rect(431, 0, pic::gst::PUYO_WIDTH, pic::gst::PUYO_HEIGHT);
		const cv::Rect IS_NEXT_2P_RECT = cv::Rect(837, 0, pic::gst::PUYO_WIDTH, pic::gst::PUYO_HEIGHT);

		const std::vector<cv::Rect> PIC_RECT_LIST
			{RESULT_1P_RECT, RESULT_2P_RECT,
			 FIGHT_1P_RECT, FIGHT_2P_RECT,
			 FINISH_RECT, CLEAR_RECT,
			 BOARD_1P_RECT, BOARD_2P_RECT,
			 NEXT1_1P_RECT, NEXT2_1P_RECT,
			 NEXT1_2P_RECT, NEXT2_2P_RECT,
			 IS_NEXT_1P_RECT, IS_NEXT_2P_RECT
			};

		const cv::Mat RED_PUYO_IMG = cv::imread(pic::gst::DIR_PATH + "red.png");
		const cv::Mat YELLOW_PUYO_IMG = cv::imread(pic::gst::DIR_PATH + "yellow2.png");
	}

	namespace full_hd
	{
		const std::string DIR_PATH = "../data/full_hd/puyo_img/";
		
		constexpr int PUYO_WIDTH = 64;
		constexpr int PUYO_HEIGHT = 60;
		constexpr int SMALL_PUYO_WIDTH = 51;
		constexpr int SMALL_PUYO_HEIGHT = 47;

		constexpr int X = 280 + 1080;
		constexpr int Y = 159;
		constexpr int WIDTH = 1360;
		constexpr int HEIGHT = 720;

		const cv::Rect RESULT_1P_RECT = cv::Rect(280, 400, 350, 70); // Win, Lose, Draw
		const cv::Rect RESULT_2P_RECT = cv::Rect(1256, 400, 350, 70);
		const cv::Rect FIGHT_1P_RECT = cv::Rect(620, 950, 100, 52); // is fighting.
		const cv::Rect FIGHT_2P_RECT = cv::Rect(1190, 950, 100, 52);
		const cv::Rect FINISH_RECT = cv::Rect(475, 420, 950, 220); // is finish of fight.
		const cv::Rect CLEAR_RECT  = FINISH_RECT;
		const cv::Rect BOARD_1P_RECT = cv::Rect(0, 0, 384, 720);
		const cv::Rect BOARD_2P_RECT = cv::Rect(976, 0, 384, 720);
		const cv::Rect NEXT1_1P_RECT = cv::Rect(440, 4, pic::full_hd::PUYO_WIDTH, pic::full_hd::PUYO_HEIGHT * 2);
		const cv::Rect NEXT2_1P_RECT = cv::Rect(492, 134, pic::full_hd::SMALL_PUYO_WIDTH, pic::full_hd::SMALL_PUYO_HEIGHT * 2);
		const cv::Rect NEXT1_2P_RECT = cv::Rect(855, 4, pic::full_hd::PUYO_WIDTH, pic::full_hd::PUYO_HEIGHT * 2);
		const cv::Rect NEXT2_2P_RECT = cv::Rect(815, 134, pic::full_hd::SMALL_PUYO_WIDTH, pic::full_hd::SMALL_PUYO_HEIGHT * 2);
		const cv::Rect IS_NEXT_1P_RECT = cv::Rect(440, 4, pic::full_hd::PUYO_WIDTH, pic::full_hd::PUYO_HEIGHT);
		const cv::Rect IS_NEXT_2P_RECT = cv::Rect(855, 4, pic::full_hd::PUYO_WIDTH, pic::full_hd::PUYO_HEIGHT);

		const std::vector<cv::Rect> PIC_RECT_LIST
			{RESULT_1P_RECT, RESULT_2P_RECT,
			 FIGHT_1P_RECT, FIGHT_2P_RECT,
			 FINISH_RECT, CLEAR_RECT,
			 BOARD_1P_RECT, BOARD_2P_RECT,
			 NEXT1_1P_RECT, NEXT2_1P_RECT,
			 NEXT1_2P_RECT, NEXT2_2P_RECT,
			 IS_NEXT_1P_RECT, IS_NEXT_2P_RECT,
			};

		const cv::Mat RED_PUYO_IMG = cv::imread(pic::full_hd::DIR_PATH + "red.png");
		const cv::Mat YELLOW_PUYO_IMG = cv::imread(pic::full_hd::DIR_PATH + "yellow.png");

	}
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
