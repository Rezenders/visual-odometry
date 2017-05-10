#include "VideoSource.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/ximgproc/disparity_filter.hpp"

void setMinDisparity(int, void *);
void setSpeckleWindowSize(int, void *);
void setSpeckleRange(int, void *);
void setTextureThreshold(int, void *);
void setUniquessRatio(int, void *);
void setPrefilterCap(int, void *);
void setPrefilterSize(int, void *);
void setAllParams();
void create_window_trackbar();

int numDisparities = 64, SADWindowSize = 15;
int minDisparity = 0, speckleWindowSize = 0, speckleRange = 1,
    textureThreshold = 450, uniquessRatio = 1, prefilterCap = 61,
    prefilterSize = 5;
cv::Ptr<cv::StereoBM> left_matcher =
    cv::StereoBM::create(numDisparities, SADWindowSize);
cv::Ptr<cv::StereoMatcher> right_matcher =
    cv::ximgproc::createRightMatcher(left_matcher);

int main() {
  VideoSource camera(640,480);

  cv::Mat imBwFull(camera.getHeight(), 2 * camera.getWidtht(), CV_8UC1),
      imRgbFull(camera.getHeight(), 2 * camera.getWidtht(), CV_8UC3);
  cv::Mat imBwL, imBwR, imRgbL, imRgbR, left_disp, right_disp, left_disp_vis,
      left_depth;

  create_window_trackbar();

  while (true) {
    camera.grabRgbBw(imBwL, imBwR, imRgbL, imRgbR);

    left_matcher->compute(imBwL, imBwR, left_disp);
    right_matcher->compute(imBwR, imBwL, right_disp);
    cv::ximgproc::getDisparityVis(left_disp, left_disp_vis, 1);
    // cv::GaussianBlur(left_disp_vis, left_disp_vis, cv::Size(9,9),0,0);
    // cv::equalizeHist(left_disp_vis, left_disp_vis);//

    imshow("Disparity", left_disp_vis);
    if (cv::waitKey(30) == 27)
      break;
  }

}

void create_window_trackbar() {
  cv::namedWindow("Disparity", cv::WINDOW_AUTOSIZE);
  cv::createTrackbar("minDisparity", "Disparity", &minDisparity, 100,
                     setMinDisparity);
  cv::createTrackbar("speckleSize", "Disparity", &speckleWindowSize, 500,
                     setSpeckleWindowSize);
  cv::createTrackbar("speckleRange", "Disparity", &speckleRange, 10000,
                     setSpeckleRange);
  cv::createTrackbar("textureThreshold", "Disparity", &textureThreshold, 5000,
                     setTextureThreshold);
  cv::createTrackbar("uniquessRatio", "Disparity", &uniquessRatio, 100,
                     setUniquessRatio);
  cv::createTrackbar("prefilterCap", "Disparity", &prefilterCap, 63,
                     setPrefilterCap);
  cv::createTrackbar("prefilterSize", "Disparity", &prefilterSize, 255,
                     setPrefilterSize);
  setAllParams();
}

void setMinDisparity(int, void *) {
  left_matcher->setMinDisparity(minDisparity - 30);
}
void setSpeckleWindowSize(int, void *) {
  left_matcher->setSpeckleWindowSize(speckleWindowSize);
}
void setSpeckleRange(int, void *) {
  left_matcher->setSpeckleRange(speckleRange);
}
void setTextureThreshold(int, void *) {
  left_matcher->setTextureThreshold(textureThreshold);
}
void setUniquessRatio(int, void *) {
  left_matcher->setUniquenessRatio(uniquessRatio);
}
void setPrefilterCap(int, void *) {
  if (prefilterCap == 0) {
    left_matcher->setPreFilterCap(1);
  } else {
    left_matcher->setPreFilterCap(prefilterCap);
  }
}
void setPrefilterSize(int, void *) {
  if (prefilterSize <= 4) {
    left_matcher->setPreFilterSize(5);
  } else if (prefilterSize % 2 == 0 && prefilterSize > 5) {
    left_matcher->setPreFilterSize(prefilterSize - 1);
  } else {
    left_matcher->setPreFilterSize(prefilterSize);
  }
}
void setAllParams() {
  left_matcher->setMinDisparity(minDisparity);
  left_matcher->setSpeckleWindowSize(speckleWindowSize);
  left_matcher->setSpeckleRange(speckleRange);
  left_matcher->setTextureThreshold(textureThreshold);
  left_matcher->setUniquenessRatio(uniquessRatio);
  left_matcher->setPreFilterCap(prefilterCap);
  left_matcher->setPreFilterSize(prefilterSize);
}
