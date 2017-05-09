#include "VideoSource.hpp"

VideoSourceCV::VideoSourceCV(Camera cam)
{
	myCam=cam;
	if(myCam==minoruCV)
	{
		init_from_minoruCV();
	}
	else
	{
		std::cout<<"Wrong camera specified "<<std::endl;
	}
}

VideoSourceCV::~VideoSourceCV()
{
	leftCam.release();
	rightCam.release();
	std::cout<<"VideoSourceCV destruction"<<std::endl;
}

void VideoSourceCV::init_from_minoruCV()
{
	std::cout << "  VideoSourceCVD_Linux: Opening video source(minoruCV)..." << std::endl;

	leftCam.open(1);
	rightCam.open(0);
	try
	{
		if(!leftCam.isOpened())
		{
			throw std::runtime_error(std::string(" Couldn't open the left camera"));
		}else
		if(!rightCam.isOpened())
		{
			throw std::runtime_error(std::string(" Couldn't open the right camera"));
		}
	}catch (std::exception& e)
    {
        std::cout << e.what();
        exit(0);
    }

	std::cout << "  ... got video source." << std::endl;

	leftCam.set(CV_CAP_PROP_FPS,15);
	rightCam.set(CV_CAP_PROP_FPS,15);

	camSize.height= 480;
	camSize.width= 640;

	readParameters();
}

void VideoSourceCV::readParameters()
{
	const char* calib_file = "../../calib/cam_stereo.yml";
	cv::FileStorage fs1(calib_file, cv::FileStorage::READ);


	fs1["K1"] >> K1;
	fs1["K2"] >> K2;
	fs1["D1"] >> D1;
	fs1["D2"] >> D2;
	fs1["R"] >> R;
	fs1["T"] >> T;

	fs1["R1"] >> R1;
	fs1["R2"] >> R2;
	fs1["P1"] >> P1;
	fs1["P2"] >> P2;
	fs1["Q"] >> Q;
}


void VideoSourceCV::rectifyImage(cv::Mat &imRgbL, cv::Mat &imRgbR)
{
	cv::Mat lmapx, lmapy, rmapx, rmapy;
	cv::Mat imAuxL, imAuxR;

	cv::initUndistortRectifyMap(K1, D1, R1, P1, imRgbL.size(), CV_32F, lmapx, lmapy);
	cv::initUndistortRectifyMap(K2, D2, R2, P2, imRgbR.size(), CV_32F, rmapx, rmapy);
	cv::remap(imRgbL, imAuxL, lmapx, lmapy, cv::INTER_LINEAR);
	cv::remap(imRgbR, imAuxR, rmapx, rmapy, cv::INTER_LINEAR);

	imRgbL = imAuxL;
	imRgbR = imAuxR;
}

void VideoSourceCV::GetAndFillFrameBWandRGB(cv::Mat &imBwL, cv::Mat &imBwR, cv::Mat &imRgbL, cv::Mat &imRgbR)
{
	cv::Mat auxRgbL, auxRgbR;
	if(!leftCam.grab() || !rightCam.grab())
	{
		std::cout<<" Couldn't grab frame"<<std::endl;
	}

	//Retrieves both images
	leftCam.retrieve(auxRgbL,CV_8UC3);
	rightCam.retrieve(auxRgbR,CV_8UC3);

	rectifyImage(auxRgbL, auxRgbR);

	imRgbL = auxRgbL;
	imRgbR = auxRgbR;
	cv::cvtColor(imRgbL, imBwL, cv::COLOR_RGB2GRAY);
	cv::cvtColor(imRgbR, imBwR, cv::COLOR_RGB2GRAY);
}
