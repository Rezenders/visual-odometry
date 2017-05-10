#include "VideoSource.hpp"

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
