#include "VideoSource.hpp"

int main(){
  VideoSource cam(480, 640);
  cv::Mat imBwL, imBwR, imRgbL, imRgbR;

  cam.grabRgbBw(imBwL, imBwR, imRgbL, imRgbR);
}
