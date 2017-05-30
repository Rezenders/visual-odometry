#include "VideoSource.hpp"
#include "disparity.hpp"

int main() {
  VideoSource camera(640,480);
  Disparity disp_maker(disp_t::BMWLS);

  cv::Mat imBwFull(camera.getHeight(), 2 * camera.getWidtht(), CV_8UC1);
  cv::Mat imBwL, imBwR;


  // Creates a window
  cv::namedWindow("Original", cv::WINDOW_AUTOSIZE);
  cv::namedWindow("Disparity", cv::WINDOW_AUTOSIZE);

  while (true) {
    camera.grabRgbBw(imBwL, imBwR);
    cv::hconcat(imBwL, imBwR, imBwFull);

    disp_maker.apply_disparity(imBwL, imBwR);

    imshow("Original", imBwFull);
    imshow("Disparity", disp_maker.get_filtered_disp_vis());

    if (cv::waitKey(30) == 27)
      break;
  }

  return 1;
}
