#include "opencv2/opencv.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ximgproc/disparity_filter.hpp"
#include "VideoSource.hpp"

int numDisparities = 64,  SADWindowSize = 17 ;
int minDisparity = 0, speckleWindowSize =100, speckleRange =1000, textureThreshold =500, uniquessRatio = 5, prefilterCap =63,prefilterSize=5;

//Class used to calculate the disparity
cv::Ptr<cv::StereoBM> left_matcher = cv::StereoBM::create( numDisparities, SADWindowSize );
cv::Ptr<cv::StereoMatcher> right_matcher = cv::ximgproc::createRightMatcher(left_matcher);
cv::Ptr<cv::ximgproc::DisparityWLSFilter> wls_filter;

void setMinDisparity(int, void*)
{
	left_matcher->setMinDisparity(minDisparity);
}
void setSpeckleWindowSize(int, void*)
{
	left_matcher->setSpeckleWindowSize(speckleWindowSize);
}
void setSpeckleRange(int, void*)
{
	left_matcher->setSpeckleRange(speckleRange);
}
void setTextureThreshold(int, void*)
{
	left_matcher->setTextureThreshold(textureThreshold);
}
void setUniquessRatio(int, void*)
{
	left_matcher->setUniquenessRatio(uniquessRatio);
}
void setPrefilterCap(int, void*)
{
	if(prefilterCap==0)
	{
		left_matcher->setPreFilterCap(1);
	}else
	{
		left_matcher->setPreFilterCap(prefilterCap);
	}
}
void setPrefilterSize(int, void*)
{
	if(prefilterSize<=4)
	{
		left_matcher->setPreFilterSize(5);
	}else
	if(prefilterSize%2 ==0 && prefilterSize>5)
	{
		left_matcher->setPreFilterSize(prefilterSize-1);
	}else
	{
		left_matcher->setPreFilterSize(prefilterSize);
	}
}

void setAllParams()
{
	left_matcher->setMinDisparity(minDisparity);
	left_matcher->setSpeckleWindowSize(speckleWindowSize);
	left_matcher->setSpeckleRange(speckleRange);
	left_matcher->setTextureThreshold(textureThreshold);
	left_matcher->setUniquenessRatio(uniquessRatio);
	left_matcher->setPreFilterCap(prefilterCap);
	left_matcher->setPreFilterSize(prefilterSize);
}
int main()
{
	VideoSourceCV camera(minoruCV);
	cv::Mat imBwFull(camera.getHeight(), 2*camera.getWidtht(), CV_8UC1), imRgbFull(camera.getHeight(), 2*camera.getWidtht(), CV_8UC3);
	cv::Mat imBwL,imBwR, imRgbL, imRgbR;

	//images to store disparity
	cv::Mat left_disp, left_disp8;
	cv::Mat right_disp, right_disp8;

	cv::Mat filtered_disp, filtered_disp8;

	//parameters...
	//~ left_matcher->setDisp12MaxDiff(1);


	//Creates a window
	cv::namedWindow("Original",cv::WINDOW_AUTOSIZE);
	//~ cv::resizeWindow("Original", 2*camera.getWidtht(), camera.getHeight());

	cv::namedWindow("Disparity",cv::WINDOW_AUTOSIZE);
	cv::createTrackbar("minDisparity", "Disparity",&minDisparity, 100, setMinDisparity);
	cv::createTrackbar("speckleSize", "Disparity",&speckleWindowSize, 500, setSpeckleWindowSize);
	cv::createTrackbar("speckleRange", "Disparity",&speckleRange, 10000, setSpeckleRange);
	cv::createTrackbar("textureThreshold", "Disparity",&textureThreshold, 5000, setTextureThreshold);
	cv::createTrackbar("uniquessRatio", "Disparity",&uniquessRatio, 100,setUniquessRatio);
	cv::createTrackbar("prefilterCap", "Disparity",&prefilterCap, 63,setPrefilterCap);
	cv::createTrackbar("prefilterSize", "Disparity",&prefilterSize, 255,setPrefilterSize);
	setAllParams();

	cv::Mat depth, depth8;
	// cv::namedWindow("Depth",cv::WINDOW_AUTOSIZE);
	while(true)
	{
		camera.GetAndFillFrameBWandRGB(imBwL, imBwR, imRgbL, imRgbR);

		//Used to horizontally concatenate both images
		cv::hconcat(imBwL, imBwR, imBwFull);

		//Display in the "Original" window
		imshow("Original",imBwFull);

		//calc the disparity
		left_matcher->compute(imBwL, imBwR, left_disp);
		right_matcher->compute(imBwR,imBwL, right_disp);

		//filtro
		wls_filter = cv::ximgproc::createDisparityWLSFilter(left_matcher);
		//~ wls_filter->setDepthDiscontinuityRadius((int)ceil(0.5*SADWindowSize));
		wls_filter->setLambda(8000);
    wls_filter->setSigmaColor(1.5);
		wls_filter->filter(left_disp,imBwL,filtered_disp,right_disp, cv::Rect(), imBwR);

		cv::Mat filtered_disp_vis;
		cv::ximgproc::getDisparityVis(filtered_disp, filtered_disp_vis, 5);
		imshow("Disparity",filtered_disp_vis);

		// cv::reprojectImageTo3D(filtered_disp_vis, depth, camera.getQ(), false, -1);
		// double Min,Max;
		// cv::minMaxIdx(depth,&Min,&Max);
		// depth.convertTo(depth8,CV_8U,255.0/(Max-Min),-255.0*Min/(Max-Min));

		// imshow("Depth",depth);

		//waits for esc key to exit
		if(cv::waitKey(30) == 27) break;
	}

	return 1;
}
