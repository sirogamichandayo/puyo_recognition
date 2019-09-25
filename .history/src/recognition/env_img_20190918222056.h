#ifndef _ENV_IMG_
#define _ENV_IMG_

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

/*
VideoCapture
ScreenShot
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
	}
	
	

private:
	T cap;
};

#endif // _END_IMG_