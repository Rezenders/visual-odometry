#ifndef VIDEOSOURCE_HPP
#define VIDEOSOURCE_HPP

#include <iostream>
#include <mutex>
#include <thread>

#include <opencv2/opencv.hpp>

/**
*\class VideoSourceCV
*\brief Classe utilizada para capturar e retificar imagens utilizando OpenCV
*/
class VideoSource {
private:
  int n_left = 1, n_right = 0;
  cv::Mat leftImg, rightImg;
  cv::Size camSize;

  std::thread /*thread_control,*/ thread_cap_left, thread_cap_right;
  std::mutex mutex_left, mutex_right;

  void function_cap_left();
  void function_cap_right();

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
  VideoSource(int height, int width);

  ~VideoSource();

  /**
  * \brief Pega as imagens capturadas
  * \param[out] &imBwL Matriz que vai conter a Imagem preto e branca do lado esquerdo
  * \param[out] &imBwR Matriz que vai conter a Imagem preto e branca do lado direito
  * \param[out] &imRgbL Matriz que vai conter a Imagem RGB do lado esquerdo
  * \param[out] &imRgbR Matriz que vai conter a Imagem RGB do lado direito
  */
  void grabRgbBw(cv::Mat &imBwL, cv::Mat &imBwR, cv::Mat &imRgbL, cv::Mat &imRgbR);

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
