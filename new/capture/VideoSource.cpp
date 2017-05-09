#include "VideoSource.hpp"

VideoSource::VideoSource() {
  std::cout << "Construtor Padrão de VideoSource" << std::endl;
}

VideoSource::VideoSource(int height, int width) {
  camSize.height = height;
  camSize.width = width;

  thread_cap_left = std::thread(&VideoSource::function_cap_left, this);
  thread_cap_right = std::thread(&VideoSource::function_cap_right, this);
}

VideoSource::~VideoSource(){
  thread_cap_left.join();
  thread_cap_right.join();
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

  while (leftCam.isOpened()) {
    mutex_left.lock();
    try {
      if (!leftCam.grab()) {
        throw std::runtime_error(std::string(" Couldn't grab the left camera"));
      }
    } catch (std::exception &e) {
      std::cout << e.what();
      exit(0);
    }
    leftCam.retrieve(tempImg, CV_8UC3);
    leftImg = tempImg;
    mutex_left.unlock();
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

  while (rightCam.isOpened()) {
    mutex_right.lock();
    try {
      if (!rightCam.grab()) {
        throw std::runtime_error(
            std::string(" Couldn't grab the right camera"));
      }
    } catch (std::exception &e) {
      std::cout << e.what();
      exit(0);
    }
    rightCam.retrieve(tempImg, CV_8UC3);
    rightImg = tempImg;
  }
  mutex_right.unlock();
}
