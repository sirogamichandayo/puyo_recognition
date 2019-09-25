#ifndef _ENV_IMG_
#define _ENV_IMG_

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

template<typename T>
class EnvImg
{
public:
	template<typename T>
	explicit EnvImg(const T *const env) 
	{
		cap = env;
	}

	// TODO	>> operator

	void operator >> (cv::Mat& img)
	{
		*cap >> img;	
	}

private:
	T *cap;
}

#endif // _END_IMG_