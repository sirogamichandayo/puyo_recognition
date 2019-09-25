#ifndef _SCREENSHOT_H_
#define _SCREENSHOT_H_

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <X11/Xlib.h>
#include <X11/xutil.h>
#include <iostream>

namespace pic
{
	const std::vector<string> monitor = {"one", "three"};
	const cv::Rect issue_1p = cv::Rect(280, 400, 350, 70); // Win, Lose, Draw
	const cv::Rect issue_2p = cv::Rect(1256, 400, 350, 70);
	const cv::Rect fight_1p = cv::Rect(620, 950, 100, 52); // is fighting.
	const cv::Rect fight_2p = cv::Rect(1190, 950, 100, 52);
	const cv::Rect finish   = cv::Rect(475, 420, 950, 220);// is finish of fight.
	const cv::Rect board_1p;
	const cv::Rect board_2p;
	const cv::Rect next_1p = cv::Rect(720, 160, pic::PUYO_WIDTH, pic::PUYO_HEIGHT*2);
	const cv::Rect next_2p = cv::Rect(1135, 160, pic::PUYO_WIDTH, pic::PUYO_HEIGHT*2);
	const cv::Rect next2_1p = cv::;
	const cv::Rect next2_2p;

	const int PUYO_WIDTH = 64;
	const int PUYO_HEIGHT = 60;
	const int WIDTH = 1920;
	const int HEIGHT   = 1080;
	const int P_WIDTH  = 384;
	const int P_HEIGHT = 720;
	const int TOP_LEFT_x_1p = 280;
	const int TOP_LEFT_y_1p = 160;
	const int TOP_LEFT_x_2p = 1256;
	const int TOP_LEFT_y_2p = 160;
	const int JUDGE_x_1p = TOP_LEFT_x_1p;
	const int JUDGE_x_2p = TOP_LEFT_x_2p;
	const int JUDGE_y    = 380;
	const int JUDGE_WIDTH  = P_WIDTH;
	const int JUDGE_HEIGHT = P_HEIGHT / 5;	
}


class ScreenShot
{
public:
  ScreenShot(int x, int y, int width, int height):
		x(x),
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
			cout << "Could not be find monitor setting." << endl;
			cout << "Please confirm command line argument or  " << endl;
			cout << "<line 29> pic::vector<string> pic::monitor." << endl;
		}
		// 'it' points to the second element of the vector, 'index' is 1

		if (str == pic::monitor[0])
			return ScreenShot(0, 0, 1920, 1080);

		if (str == pic::monitor[1])
			return ScreenShot(1080, 0, 1920, 1080);

		// Compiler says "control may reach end of non-void function."
		// But there is no such pattern.
	};

  void operator() (Mat& cvImg)
	{
		if (init==true)
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
  Display* display;
  Window root;
  int x,y,width,height;
  XImage* img;

  bool init;
};

#endif // _SCREENSHOT_H_
