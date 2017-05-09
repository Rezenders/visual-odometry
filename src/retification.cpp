#include "opencv2/opencv.hpp"
#include "VideoSource.hpp"
//~ #include "StereoEfficientLargeScale.h"

int main()
{
	VideoSourceCV camera(minoruCV);
	cv::Mat imBwFull(camera.getHeight(), 2*camera.getWidtht(), CV_8UC1), imRgbFull(camera.getHeight(), 2*camera.getWidtht(), CV_8UC3);
	cv::Mat imBwL,imBwR, imRgbL, imRgbR;
	
	//Creates a window
	cv::namedWindow("Original",cv::WINDOW_AUTOSIZE);
	cv::resizeWindow("Original", 2*camera.getWidtht(), camera.getHeight());
	
	while(true)
	{
		camera.GetAndFillFrameBWandRGB(imBwL, imBwR, imRgbL, imRgbR);
			
		//Used to horizontally concatenate both images
		cv::hconcat(imBwL, imBwR, imBwFull);
		
		//Display in the "Original" window
		imshow("Original",imBwFull);
		
		//waits for esc key to exit
		if(cv::waitKey(30) == 27) break;
	}
		
	return 1;
}
