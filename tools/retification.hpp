#ifndef RETIFICATION_HPP
#define RETIFICATION_HPP

#include <opencv2/opencv.hpp>

/**
* \class retification
* \brief Classe utilizada para retificar imagens
*/
class retification {

private:
  cv::Mat R1, R2, P1, P2, Q; // Retification Matrix, Projection Matrix,
                             // disparity-to-depth mapping matrix
  cv::Mat K1, K2, R; // Intrinsics, Rotation
  cv::Vec3d T;       // Translation
  cv::Mat D1, D2;    // distorcion
  cv::Mat lmapx, lmapy, rmapx, rmapy;

  cv::Size mySize = cv::Size(640, 480);

public:
  retification();
  retification(const char *filename);
  retification(cv::Size size, const char *filename);

  void readParameters();
  void readParameters(const char *filename);
  void readParameters(cv::Size size, const char *filename);

  inline cv::Mat getQ() { return Q; }

  /**
  * \brief aplica a retificação nas duas imagens passadas como parametro
  * \param &imRgbL Imagem da esquerda
  * \param &imRgbR Imagem da direita
  */
  void rectifyImage(cv::Mat &imRgbL, cv::Mat &imRgbR);
  /**
  * \brief aplica a retificação nas duas ultimas imagens passadas como parametro
  * \param srcL Imagem de origem da esquerda
  * \param srcR Imagem de origem da direita
  * \param &imRgbL Imagem destino da esquerda
  * \param &imRgbR Imagem destino da direita
  */
  void rectifyImage(cv::Mat srcL, cv::Mat srcR, cv::Mat &dstL, cv::Mat &dstR);

  inline void setSize(cv::Size size) { mySize = size; }
};

#endif
