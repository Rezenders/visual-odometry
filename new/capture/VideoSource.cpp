#include "VideoSource.hpp"

VideoSource::VideoSource() {
  std::cout << "Construtor Padrão de VideoSource" << std::endl;
}

VideoSource::VideoSource(int height, int width) {
  camSize.height = height;
  camSize.width = width;

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
  leftCam.set(CV_CAP_PROP_FPS,15);

  try {
    if (!leftCam.isOpened()) {
      throw std::runtime_error(std::string(" Couldn't open the left camera"));
    }
  } catch (std::exception &e) {
    std::cout << e.what();
    exit(0);
  }

  while (leftCam.grab()) {
    mutex_right.unlock();
    mutex_left.lock();

    leftCam.retrieve(tempImg, CV_8UC3);
    leftImg = tempImg;
  }
}

void VideoSource::function_cap_right() {
  cv::VideoCapture rightCam;
  cv::Mat tempImg;
  rightCam.open(n_right);
  rightCam.set(CV_CAP_PROP_FPS,15);

  try {
    if (!rightCam.isOpened()) {
      throw std::runtime_error(std::string(" Couldn't open the right camera"));
    }
  } catch (std::exception &e) {
    std::cout << e.what();
    exit(0);
  }

  while (rightCam.grab()) {
    mutex_left.unlock();
    mutex_right.lock();
    
    rightCam.retrieve(tempImg);
    rightImg = tempImg;
  }
}

void VideoSource::grabRgbBw(cv::Mat &imBwL, cv::Mat &imBwR, cv::Mat &imRgbL, cv::Mat &imRgbR){
  // mutex_left.lock();
  // mutex_right.lock();
  imRgbL = leftImg;
  imRgbR = rightImg;
  // mutex_left.unlock();
  // mutex_right.unlock();

  cv::cvtColor(imRgbL, imBwL, cv::COLOR_RGB2GRAY);
	cv::cvtColor(imRgbR, imBwR, cv::COLOR_RGB2GRAY);
}
