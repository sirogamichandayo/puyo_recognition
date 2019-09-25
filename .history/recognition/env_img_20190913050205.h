#ifndef _ENV_IMG_
#define _ENV_IMG_

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

class EnvImg
{
public:
	template<typename T>
	explicit EnvImg(const T* env) 
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
}

#endif // _END_IMG_