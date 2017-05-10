#include "retification.hpp"

retification::retification() {
  const char *filename = "../../calib/cam_stereo.yml";
  retification::readParameters(filename);
}

retification::retification(const char *filename) {
  retification::readParameters(filename);
}

retification::retification(cv::Size size, const char *filename) {
  retification::setSize(size);
  retification::readParameters(filename);
}

void retification::readParameters() {
  const char *filename = "../../calib/cam_stereo.yml";
  retification::readParameters(filename);
}

void retification::readParameters(cv::Size size, const char *filename) {
  retification::setSize(size);
  retification::readParameters(filename);
}

void retification::readParameters(const char *filename) {
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

void retification::rectifyImage(cv::Mat srcL, cv::Mat srcR, cv::Mat &dstL,
                                cv::Mat &dstR) {
  dstL = srcL;
  dstR = srcR;
  retification::rectifyImage(dstL, dstR);
}

void retification::rectifyImage(cv::Mat &imRgbL, cv::Mat &imRgbR) {
  cv::Mat imAuxL, imAuxR;

  cv::remap(imRgbL, imAuxL, lmapx, lmapy, cv::INTER_LINEAR);
  cv::remap(imRgbR, imAuxR, rmapx, rmapy, cv::INTER_LINEAR);

  imRgbL = imAuxL;
  imRgbR = imAuxR;
}
