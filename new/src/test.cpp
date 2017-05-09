#include "VideoSource.hpp"

int main(){
  VideoSource cam(480, 640);
  cv::Mat imBwFull(cam.getSize(), CV_8UC1);
  cv::Mat imBwL, imBwR, imRgbL, imRgbR;

  cv::namedWindow("Video", cv::WINDOW_AUTOSIZE);
  while (true) {
    cam.grabRgbBw(imBwL, imBwR, imRgbL, imRgbR);
    cv::hconcat(imBwL, imBwR, imBwFull);
    imshow("Video", imBwFull);

    //waits for esc key to exit
    if(cv::waitKey(30) == 27) break;
  }
}
