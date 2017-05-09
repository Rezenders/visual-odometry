#include "opencv2/opencv.hpp"
#include "VideoSource.hpp"
#include "StereoEfficientLargeScale.h"

int main()
{
	VideoSourceCV camera(minoruCV);
	cv::Mat imBwFull(camera.getHeight(), 2*camera.getWidtht(), CV_8UC1), imRgbFull(camera.getHeight(), 2*camera.getWidtht(), CV_8UC3);
	cv::Mat imBwL,imBwR, imRgbL, imRgbR;
	cv::Mat disp, disp8;
	cv::Mat depth, depth8;

	StereoEfficientLargeScale elas(0,128);

	//Create windows
	cv::namedWindow("Original",cv::WINDOW_AUTOSIZE);
	cv::namedWindow("Disparity",cv::WINDOW_AUTOSIZE);
	// cv::namedWindow("Depth",cv::WINDOW_AUTOSIZE);

	while(true)
	{
		camera.GetAndFillFrameBWandRGB(imBwL, imBwR, imRgbL, imRgbR);

		//Used to horizontally concatenate both images
		cv::hconcat(imBwL, imBwR, imBwFull);

		//Display in the "Original" window
		imshow("Original",imBwFull);

		elas.elas.param.ipol_gap_width=5;
		elas.elas.param.speckle_size=100;
		// elas.elas.param.incon_window_size=;
		elas.elas.param.add_corners=1;
		// elas.elas.param.subsampling=1;
		//calcs the disparity
		elas(imBwL, imBwR, disp, 100);

		cv::normalize(disp, disp8, 0, 255, CV_MINMAX, CV_8U);
		// disp.convertTo(disp8,CV_8U,1.0/8);
		imshow("Disparity",disp8);

		// cv::reprojectImageTo3D(disp, depth, camera.getQ(), false, -1);
		// double Min,Max;
		// cv::minMaxIdx(depth,&Min,&Max);
		// depth.convertTo(depth8,CV_8U,255.0/(Max-Min),-255.0*Min/(Max-Min));
		//
		// imshow("Depth",depth8);

		//waits for esc key to exit
		if(cv::waitKey(30) == 27) break;
	}

	return 1;
}
