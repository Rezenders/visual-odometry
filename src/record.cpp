#include "VideoSource.hpp"
#include "disparity.hpp"
#include <opencv2/videoio.hpp>

int main() {
  VideoSource camera(640,480);
  Disparity disp_maker(disp_t::BMWLS);
  cv::VideoWriter outputRgb, outputBw, outputDisp;

  outputRgb.open("outputRgbBMWLS.avi",CV_FOURCC('W', 'M', 'V', '2'), camera.getFps(), cv::Size(camera.getWidth()*2,camera.getHeight()),true);
  outputBw.open("outputBwBMWLS.avi",CV_FOURCC('W', 'M', 'V', '2'), camera.getFps(), cv::Size(camera.getWidth()*2,camera.getHeight()),false);
  outputDisp.open("outputDispBMWLS.avi",CV_FOURCC('W', 'M', 'V', '2'), camera.getFps(), camera.getSize(),false);

  cv::Mat imBwFull(camera.getHeight(), 2 * camera.getWidth(), CV_8UC1),
            imRgbFull(camera.getHeight(), 2 * camera.getWidth(), CV_8UC3);
  cv::Mat imBwL, imBwR, imRgbL, imRgbR;


  // Creates a window
  cv::namedWindow("Original", cv::WINDOW_AUTOSIZE);
  cv::namedWindow("Disparity", cv::WINDOW_AUTOSIZE);

  while (true) {
    camera.grabRgbBw(imBwL, imBwR, imRgbL, imRgbR);

    cv::hconcat(imBwL, imBwR, imBwFull);
    cv::hconcat(imRgbL, imRgbR, imRgbFull);

    disp_maker.apply_disparity(imBwL, imBwR);

    outputRgb << imRgbFull;
    outputBw << imBwFull;
    outputDisp << disp_maker.get_filtered_disp_vis();

    imshow("Original", imBwFull);
    imshow("Disparity", disp_maker.get_filtered_disp_vis());

    if (cv::waitKey(30) == 27)
      break;
  }
  outputRgb.release();
  outputBw.release();
  outputDisp.release();


  return 1;
}
