#include "VideoSource.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/ximgproc/disparity_filter.hpp"

int numDisparities = 64, SADWindowSize = 17;
int minDisparity = 0, speckleWindowSize = 100, speckleRange = 1000,
    textureThreshold = 500, uniquessRatio = 5, prefilterCap = 63,
    prefilterSize = 5;

// Class used to calculate the disparity
cv::Ptr<cv::StereoBM> left_matcher =
    cv::StereoBM::create(numDisparities, SADWindowSize);
cv::Ptr<cv::StereoMatcher> right_matcher =
    cv::ximgproc::createRightMatcher(left_matcher);
cv::Ptr<cv::ximgproc::DisparityWLSFilter> wls_filter;

int main() {
  VideoSource camera(640,480);
  cv::Mat imBwFull(camera.getHeight(), 2 * camera.getWidth(), CV_8UC1),
      imRgbFull(camera.getHeight(), 2 * camera.getWidth(), CV_8UC3);
  cv::Mat imBwL, imBwR, imRgbL, imRgbR;

  // images to store disparity
  cv::Mat left_disp, left_disp8;
  cv::Mat right_disp, right_disp8;

  cv::Mat filtered_disp, filtered_disp8;
  cv::Mat filtered_disp_vis;

  // Creates a window
  cv::namedWindow("Original", cv::WINDOW_AUTOSIZE);

  cv::namedWindow("Disparity", cv::WINDOW_AUTOSIZE);

  while (true) {
    camera.grabRgbBw(imBwL, imBwR, imRgbL, imRgbR);
    // Used to horizontally concatenate both images
    cv::hconcat(imBwL, imBwR, imBwFull);

    // Display in the "Original" window
    imshow("Original", imBwFull);

    // calc the disparity
    left_matcher->compute(imBwL, imBwR, left_disp);
    right_matcher->compute(imBwR, imBwL, right_disp);



    // filtro
    wls_filter = cv::ximgproc::createDisparityWLSFilter(left_matcher);
    wls_filter->setLambda(8000);
    wls_filter->setSigmaColor(2.0);
    wls_filter->filter(left_disp, imBwL, filtered_disp, right_disp);

    cv::ximgproc::getDisparityVis(filtered_disp, filtered_disp_vis, 5);

    imshow("Disparity", filtered_disp_vis);

    if (cv::waitKey(30) == 27)
      break;
  }

  return 1;
}
