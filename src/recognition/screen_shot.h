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
	const int RESULT_1P_RECT_I = 0;
	const int RESULT_2P_RECT_I = 1;
	const int FIGHT_1P_RECT_I = 2;
	const int FIGHT_2P_RECT_I = 3;
	const int FINISH_RECT_I = 4;
	const int CLEAR_RECT_I = 5;
	const int BOARD_1P_RECT_I = 6;
	const int BOARD_2P_RECT_I = 7;
	const int NEXT1_1P_RECT_I = 8;
	const int NEXT2_1P_RECT_I = 9;
	const int NEXT1_2P_RECT_I = 10;
	const int NEXT2_2P_RECT_I = 11;
	const int IS_NEXT_1P_RECT_I = 12;
	const int IS_NEXT_2P_RECT_I = 13;
	
	const int HD_WIDTH = 1920;
	const int HD_HEIGHT = 1080;
	namespace gst
	{
		const int PUYO_WIDTH = 63;
		const int PUYO_HEIGHT = 58;
		const int SMALL_PUYO_WIDTH = 50;
		const int SMALL_PUYO_HEIGHT = 46;

		const int X = 1374;
		const int Y = 186;
		const int WIDTH = 1330;
		const int HEIGHT = 696;

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
	}

	namespace full_hd
	{
		const int PUYO_WIDTH = 64;
		const int PUYO_HEIGHT = 60;
		const int SMALL_PUYO_WIDTH = 51;
		const int SMALL_PUYO_HEIGHT = 47;

		const int X = 1360;
		const int Y = 159;
		const int WIDTH = 1360;
		const int HEIGHT = 720;

		const cv::Rect RESULT_1P_RECT = cv::Rect(280, 400, 350, 70); // Win, Lose, Draw
		const cv::Rect RESULT_2P_RECT = cv::Rect(1256, 400, 350, 70);
		const cv::Rect FIGHT_1P_RECT = cv::Rect(620, 950, 100, 52); // is fighting.
		const cv::Rect FIGHT_2P_RECT = cv::Rect(1190, 950, 100, 52);
		const cv::Rect FINISH_RECT = cv::Rect(475, 420, 950, 220); // is finish of fight.
		const cv::Rect CLEAR_RECT  = FINISH_RECT;
		const cv::Rect BOARD_1P_RECT = cv::Rect(0, 0, 384, 720);
		const cv::Rect BOARD_2P_RECT = cv::Rect(976, 0, 384, 720);
		const cv::Rect NEXT1_1P_RECT = cv::Rect(440, 0, pic::gst::PUYO_WIDTH, pic::gst::PUYO_HEIGHT * 2);
		const cv::Rect NEXT2_1P_RECT = cv::Rect(492, 134, pic::gst::SMALL_PUYO_WIDTH, pic::gst::SMALL_PUYO_HEIGHT * 2);
		const cv::Rect NEXT1_2P_RECT = cv::Rect(855, 0, pic::gst::PUYO_WIDTH, pic::gst::PUYO_HEIGHT * 2);
		const cv::Rect NEXT2_2P_RECT = cv::Rect(815, 134, pic::gst::SMALL_PUYO_WIDTH, pic::gst::SMALL_PUYO_HEIGHT * 2);
		const cv::Rect IS_NEXT_1P_RECT = cv::Rect(440, 0, pic::gst::PUYO_WIDTH, pic::gst::PUYO_HEIGHT);
		const cv::Rect IS_NEXT_2P_RECT = cv::Rect(855, 0, pic::gst::PUYO_WIDTH, pic::gst::PUYO_HEIGHT);

		const std::vector<cv::Rect> PIC_RECT_LIST
			{RESULT_1P_RECT, RESULT_2P_RECT,
			 FIGHT_1P_RECT, FIGHT_2P_RECT,
			 FINISH_RECT, CLEAR_RECT,
			 BOARD_1P_RECT, BOARD_2P_RECT,
			 NEXT1_1P_RECT, NEXT2_1P_RECT,
			 NEXT1_2P_RECT, NEXT2_2P_RECT,
			 IS_NEXT_1P_RECT, IS_NEXT_2P_RECT,
			};
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
