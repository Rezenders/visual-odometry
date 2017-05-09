#include "VideoSource.hpp"

VideoSourceCVD::VideoSourceCVD(Camera cam)
{
	conversion.init(VIDEO_W,VIDEO_H);
	myCam = cam;
	if(myCam == minoruCVD)
	{
		init_from_minoruCVD();
	}
	else
	{
		std::cout<<"Wrong camera specified "<<std::endl;
	}
};

VideoSourceCVD::~VideoSourceCVD()
{
	std::cout<<"VideoSourceCVD destruction"<<std::endl;

}

void VideoSourceCVD::init_from_minoruCVD()
{
	std::cout << "  VideoSourceCVD_Linux: Opening video source(minoruCVD)..." << std::endl;

	std::string QuickCamFile = "/dev/video1";
	std::string QuickCamFile1 = "/dev/video0";
	CVD::ImageRef irSize = CVD::ImageRef(VIDEO_W,VIDEO_H);
	int nFrameRate =  15;
	try
	{
		CVD::V4LBuffer<CVD::yuv422>* pvb = new CVD::V4LBuffer<CVD::yuv422>(QuickCamFile, irSize, -1, false, nFrameRate);
		CVD::V4LBuffer<CVD::yuv422>* pvb1 = new CVD::V4LBuffer<CVD::yuv422>(QuickCamFile1, irSize, -1, false, nFrameRate);

		mirSize = pvb->size();
		mptr = pvb;
		mptr1 = pvb1;
	}
	catch (const CVD::Exceptions::All& e)
	{
		std::cout << "ERROR " << e.what << std::endl;
	}

	std::cout << "  ... got video source." << std::endl;
}

void VideoSourceCVD::GetAndFillFrameBWandRGB(CVD::Image<CVD::byte> &imBW, CVD::Image<CVD::Rgb<CVD::byte> > &imRGB)
{
	 if (myCam == minoruCVD)
	{
		CVD::V4LBuffer<CVD::yuv422>* pvb = (CVD::V4LBuffer<CVD::yuv422>*) mptr;
		CVD::VideoFrame<CVD::yuv422> *pVidFrame = pvb->get_frame();
		convert_image(*pVidFrame, imBW);
		convert_image(*pVidFrame, imRGB);
		pvb->put_frame(pVidFrame);
	}else
	{
		std::cout<<"In GetAndFillFrameBWandRGB no camera specified " <<std::endl;
	}
}
void VideoSourceCVD::GetAndFillFrameBWandRGB(std::vector< CVD::Image<CVD::byte> > &imBW, std::vector< CVD::Image<CVD::Rgb<CVD::byte> > > &imRGB)
{
	CVD::ImageRef imageSize(VIDEO_W, VIDEO_H);
	CVD::Image<CVD::byte> auxBW(imageSize);

	imBW.clear();

	 if (myCam == minoruCVD)
	{
		CVD::V4LBuffer<CVD::yuv422>* pvb = (CVD::V4LBuffer<CVD::yuv422>*) mptr;
		CVD::V4LBuffer<CVD::yuv422>* pvb1 = (CVD::V4LBuffer<CVD::yuv422>*) mptr1;

		CVD::VideoFrame<CVD::yuv422> *pVidFrame = pvb->get_frame();
		CVD::VideoFrame<CVD::yuv422> *pVidFrame1 = pvb1->get_frame();

		while (pvb->frame_pending())
		{
			pvb->put_frame(pVidFrame);
			pVidFrame = pvb->get_frame();
		}

		while (pvb1->frame_pending())
		{
			pvb1->put_frame(pVidFrame1);
			pVidFrame1 = pvb1->get_frame();
		}

		convert_image(*pVidFrame, auxBW);
		convert_image(*pVidFrame, imRGB.at(0));
		imBW.push_back(auxBW);
		pvb->put_frame(pVidFrame);

		convert_image(*pVidFrame1, auxBW);
		convert_image(*pVidFrame1, imRGB.at(1));
		imBW.push_back(auxBW);
		pvb1->put_frame(pVidFrame1);
	}else
	{
		std::cout<<"In GetAndFillFrameBWandRGB no camera specified " <<std::endl;
	}
}

void VideoSourceCVD::GetAndFillFrameBWandRGB(CVD::Image<CVD::byte> &imBW1, CVD::Image<CVD::byte> &imBW2, CVD::Image<CVD::Rgb<CVD::byte> > &imRGB1, CVD::Image<CVD::Rgb<CVD::byte> > imRGB2)
{
	 if (myCam == minoruCVD)
	{

		CVD::V4LBuffer<CVD::yuv422>* pvb = (CVD::V4LBuffer<CVD::yuv422>*) mptr;
		CVD::V4LBuffer<CVD::yuv422>* pvb1 = (CVD::V4LBuffer<CVD::yuv422>*) mptr1;

		CVD::VideoFrame<CVD::yuv422> *pVidFrame = pvb->get_frame();
		CVD::VideoFrame<CVD::yuv422> *pVidFrame1 = pvb1->get_frame();

		while (pvb->frame_pending())
		{
			pvb->put_frame(pVidFrame);
			pVidFrame = pvb->get_frame();
		}

		while (pvb1->frame_pending())
		{
			pvb1->put_frame(pVidFrame1);
			pVidFrame1 = pvb1->get_frame();
		}

		convert_image(*pVidFrame, imBW1);
		convert_image(*pVidFrame, imRGB1);
		pvb->put_frame(pVidFrame);

		convert_image(*pVidFrame1, imBW2);
		convert_image(*pVidFrame1, imRGB2);
		pvb1->put_frame(pVidFrame1);
	}else
	{
		std::cout<<"In GetAndFillFrameBWandRGB no camera specified " <<std::endl;
	}
}

void VideoSourceCVD::GetAndFillFrameBWandRGB(image<uchar> &imBW1, image<uchar> &imBW2, std::vector< CVD::Image<CVD::Rgb<CVD::byte> > > &imRGB)
{
	if (myCam == minoruCVD)
	{

		CVD::V4LBuffer<CVD::yuv422>* pvb = (CVD::V4LBuffer<CVD::yuv422>*) mptr;
		CVD::V4LBuffer<CVD::yuv422>* pvb1 = (CVD::V4LBuffer<CVD::yuv422>*) mptr1;

		CVD::VideoFrame<CVD::yuv422> *pVidFrame = pvb->get_frame();
		CVD::VideoFrame<CVD::yuv422> *pVidFrame1 = pvb1->get_frame();

		while (pvb->frame_pending())
		{
			pvb->put_frame(pVidFrame);
			pVidFrame = pvb->get_frame();
		}

		while (pvb1->frame_pending())
		{
			pvb1->put_frame(pVidFrame1);
			pVidFrame1 = pvb1->get_frame();
		}

		conversion.CvdYuv422ToElas(*pVidFrame, imBW1);
		convert_image(*pVidFrame, imRGB.at(0));
		pvb->put_frame(pVidFrame);

		conversion.CvdYuv422ToElas(*pVidFrame1, imBW2);
		convert_image(*pVidFrame1, imRGB.at(1));
		pvb1->put_frame(pVidFrame1);
	}else
	{
		std::cout<<"In GetAndFillFrameBWandRGB no camera specified " <<std::endl;
	}
}
///////////////////////////////////////////////OPENCV////////////////////////////////////////////////////////////////

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

	leftCam.open(2);
	rightCam.open(1);
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
