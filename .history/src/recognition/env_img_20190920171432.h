#ifndef _ENV_IMG_
#define _ENV_IMG_

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

/*
This class is intermediate between a class that outputs images and 
class that requires it.
*/

template<typename T>
class EnvImg
{
public:
	explicit EnvImg(const T &env) 
	{
		cap = env;
	}

	// TODO	>> operator
	void operator >> (cv::Mat& img)
	{
		cap >> img;	
	/*
	VideoCapture exist operator ">>"
	ScreenShot exist operator ">>"
	*/
	}
private:
	T cap;
};

#endif // _END_IMG_