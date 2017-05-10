#include "VideoSource.hpp"

retification::retification(){
  const char* filename = "../../../calib/cam_stereo.yml";
  retification::readParameters(filename);
}

retification::retification(const char* filename){
  retification::readParameters(filename);
}

retification::retification(cv::Size size, const char* filename){
  retification::setSize(size);
  retification::readParameters(filename);
}

void retification::readParameters(){
  const char* filename = "../../../calib/cam_stereo.yml";
  retification::readParameters(filename);
}

void retification::readParameters(cv::Size size, const char* filename){
  retification::setSize(size);
  retification::readParameters(filename);
}

void retification::readParameters(const char* filename){
  cv::FileStorage fs1(filename, cv::FileStorage::READ);

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

  cv::initUndistortRectifyMap(K1, D1, R1, P1, mySize, CV_32F, lmapx, lmapy);
  cv::initUndistortRectifyMap(K2, D2, R2, P2, mySize, CV_32F, rmapx, rmapy);

  fs1.release();
}

void retification::rectifyImage(cv::Mat srcL, cv::Mat srcR, cv::Mat &dstL, cv::Mat &dstR){
  dstL= srcL;
  dstR= srcR;
  retification::rectifyImage( dstL, dstR);
}

void retification::rectifyImage(cv::Mat &imRgbL, cv::Mat &imRgbR){
  cv::Mat imAuxL, imAuxR;

  cv::remap(imRgbL, imAuxL, lmapx, lmapy, cv::INTER_LINEAR);
  cv::remap(imRgbR, imAuxR, rmapx, rmapy, cv::INTER_LINEAR);

  imRgbL = imAuxL;
  imRgbR = imAuxR;
}

VideoSource::VideoSource() {
  std::cout << "Construtor Padrão de VideoSource" << std::endl;
}

VideoSource::VideoSource(int width, int height) {
  camSize.width = width;
  camSize.height = height;
  rect.setSize(camSize);

  leftImg = cv::Mat(camSize, CV_8UC3);
  rightImg = cv::Mat(camSize, CV_8UC3);

  thread_cap_left = std::thread(&VideoSource::function_cap_left, this);
  thread_cap_right = std::thread(&VideoSource::function_cap_right, this);

  thread_cap_left.detach();
  thread_cap_right.detach();

}

VideoSource::~VideoSource(){
  std::cout<<"VideoSource destructor"<<std::endl;
}

void VideoSource::function_cap_left() {
  cv::VideoCapture leftCam;
  cv::Mat tempImg;
  leftCam.open(n_left);
  leftCam.set(CV_CAP_PROP_FPS, 20);

  try {
    if (!leftCam.isOpened()) {
      throw std::runtime_error(std::string(" Couldn't open the left camera"));
    }
  } catch (std::exception &e) {
    std::cout << e.what();
    exit(0);
  }

  while (leftCam.isOpened()) {
    leftCam.grab();
    leftCam.retrieve(tempImg, CV_8UC3);
    mutex_right.lock();
    leftImg = tempImg;
    mutex_right.unlock();
  }
}

void VideoSource::function_cap_right() {
  cv::VideoCapture rightCam;
  cv::Mat tempImg;
  rightCam.open(n_right);
  rightCam.set(CV_CAP_PROP_FPS, 20);

  try {
    if (!rightCam.isOpened()) {
      throw std::runtime_error(std::string(" Couldn't open the right camera"));
    }
  } catch (std::exception &e) {
    std::cout << e.what();
    exit(0);
  }

  while (rightCam.isOpened()) {
    rightCam.grab();
    rightCam.retrieve(tempImg);
    mutex_left.lock();
    rightImg = tempImg;
    mutex_left.unlock();

  }
}

void VideoSource::grabRgbBw(cv::Mat &imBwL, cv::Mat &imBwR, cv::Mat &imRgbL, cv::Mat &imRgbR){
  mutex_left.lock();
  imRgbL = leftImg;
  mutex_left.unlock();
  mutex_right.lock();
  imRgbR = rightImg;
  mutex_right.unlock();

  rect.rectifyImage(imRgbL, imRgbR);

  cv::cvtColor(imRgbL, imBwL, cv::COLOR_RGB2GRAY);
	cv::cvtColor(imRgbR, imBwR, cv::COLOR_RGB2GRAY);
}

void VideoSource::grabRawRgbBw(cv::Mat &imBwL, cv::Mat &imBwR, cv::Mat &imRgbL, cv::Mat &imRgbR){
  mutex_left.lock();
  imRgbL = leftImg;
  mutex_left.unlock();
  mutex_right.lock();
  imRgbR = rightImg;
  mutex_right.unlock();

  cv::cvtColor(imRgbL, imBwL, cv::COLOR_RGB2GRAY);
	cv::cvtColor(imRgbR, imBwR, cv::COLOR_RGB2GRAY);
}
