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
		if (T == VideoCapture)
		{
			isVideocapture = true;
			cap = env;
		} else if (T == ScreenShot) 
		{
			isScreenshot = true;
			sct = env;
		}
	}

	// TODO	>> operator

	void operator() (cv::Mat& img)
	{
		if (isVideoCapture)	
			*cap >> img;	
		else if (isScreenShot)
			*sct(img);
	}

private:
	bool isVideoCapture;
	bool isScreenShot;
	VideoCapture *cap;
	ScreenShot *sct;
	T *cap;
}

#endif // _END_IMG_