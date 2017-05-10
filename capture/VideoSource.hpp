#ifndef VIDEOSOURCE_HPP
#define VIDEOSOURCE_HPP


#include <iostream>
#include <mutex>
#include <thread>

#include <opencv2/opencv.hpp>

/**
* \class retification
* \brief Classe utilizada para retificar imagens
*/
class retification {
private:
  cv::Mat R1, R2, P1, P2, Q; //Retification Matrix, Projection Matrix, disparity-to-depth mapping matrix
	cv::Mat K1, K2, R; // Intrinsics, Rotation
	cv::Vec3d T; //Translation
	cv::Mat D1, D2; //distorcion
  cv::Mat lmapx, lmapy, rmapx, rmapy;

  cv::Size mySize = cv::Size(640, 480);
public:
  retification();
  retification(const char* filename);
  retification(cv::Size size, const char* filename);

  void readParameters();
  void readParameters(const char* filename);
  void readParameters(cv::Size size, const char* filename);

  inline cv::Mat getQ(){return Q;}

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


  inline void setSize(cv::Size size){mySize = size;}
};

/**
*\class VideoSource
*\brief Classe utilizada para capturar já retificadas imagens utilizando OpenCV
*/
class VideoSource {
private:
  int n_left = 1, n_right = 0;
  cv::Mat leftImg, rightImg;
  cv::Size camSize;

  std::thread thread_cap_left, thread_cap_right;
  std::mutex mutex_left, mutex_right;

  void function_cap_left();
  void function_cap_right();

  retification rect;
public:
  /**
  * \brief Construtor padrão
  */
  VideoSource();

  /**
  * \brief Construtor que recebe altura e largura como parametro
  * \param height altura em pixels
  * \param width largura em pixels
  */
  VideoSource(int width, int height);

  ~VideoSource();

  /**
  * \brief Pega as imagens capturadas e retificadas
  * \param[out] &imBwL Matriz que vai conter a Imagem preto e branca do lado esquerdo
  * \param[out] &imBwR Matriz que vai conter a Imagem preto e branca do lado direito
  * \param[out] &imRgbL Matriz que vai conter a Imagem RGB do lado esquerdo
  * \param[out] &imRgbR Matriz que vai conter a Imagem RGB do lado direito
  */
  void grabRgbBw(cv::Mat &imBwL, cv::Mat &imBwR, cv::Mat &imRgbL, cv::Mat &imRgbR);
  /**
  * \brief Pega as imagens capturadas e não retificadas
  * \param[out] &imBwL Matriz que vai conter a Imagem preto e branca do lado esquerdo
  * \param[out] &imBwR Matriz que vai conter a Imagem preto e branca do lado direito
  * \param[out] &imRgbL Matriz que vai conter a Imagem RGB do lado esquerdo
  * \param[out] &imRgbR Matriz que vai conter a Imagem RGB do lado direito
  */
  void grabRawRgbBw(cv::Mat &imBwL, cv::Mat &imBwR, cv::Mat &imRgbL, cv::Mat &imRgbR);

  /**
  *\brief Retorna o tamanho da Imagem
  *\return Objeto cv::Size
  */
  inline cv::Size getSize() { return camSize; }
  /**
  *\brief Retorna a altura da Imagem
  *\return Variavel do tipo int
  */
  inline int getHeight() { return camSize.height; }
  /**
  *\brief Retorna a largura da Imagem
  *\return Variavel do tipo int
  */
  inline int getWidtht() { return camSize.width; }
};

#endif
