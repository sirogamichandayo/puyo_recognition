#include "../src/recognition/state_main.h"
#include "../src/recognition/screen_shot.h"
#include "../src/log/log.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"

int main(int argc, char **argv)
{
	ScreenShot scr(pic::gst::X, pic::gst::Y,
				   pic::gst::WIDTH, pic::gst::HEIGHT);
	State env(&scr, &pic::gst::PIC_RECT_LIST);
	
}
