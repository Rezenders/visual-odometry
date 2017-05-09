#ifndef VIDEOSOURCE_HPP
#define VIDEOSOURCE_HPP


#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <pthread.h>
#include <cmath>
#include <vector>
#include <stdexcept>

#include "image.h"
#include "format_conversion.hpp"

#define VIDEO_W 640
#define VIDEO_H 480

enum Camera {minoruCV, minoruCVD};
//~ struct VideoSourceData;

/**
*\class VideoSourceCV
*\brief Classe utilizada para capturar e retificar imagens utilizando OpenCV
*/
class VideoSourceCV
{
	private:
	Camera myCam;
	cv::VideoCapture leftCam, rightCam;
	//~ cv::Mat imBwL, imBwR, imBwFull;
	cv::Size camSize;

	void init_from_minoruCV();
	void rectifyImage(cv::Mat &imRgbL, cv::Mat &imRgbR);

	cv::Mat R1, R2, P1, P2, Q; //Retification Matrix, Projection Matrix, disparity-to-depth mapping matrix
	cv::Mat K1, K2, R; // Intrinsics, Rotation
	cv::Vec3d T; //Translation
	cv::Mat D1, D2; //distorcion


	public:
  /**
  *\brief Construtor que receber como parametro qual camera está usando. No momento apenas minoruCV
  */
	VideoSourceCV(Camera cam);
	~VideoSourceCV();

  /**
  * \brief Métoddo para capturar imagens
  * \param[out] &imBwL Matriz que vai conter a Imagem preto e branca do lado esquerdo
  * \param[out] &imBwR Matriz que vai conter a Imagem preto e branca do lado direito
  * \param[out] &imRgbL Matriz que vai conter a Imagem RGB do lado esquerdo
  * \param[out] &imRgbR Matriz que vai conter a Imagem RGB do lado direito
  */
	void GetAndFillFrameBWandRGB(cv::Mat &imBwL, cv::Mat &imBwR, cv::Mat &imRgbL, cv::Mat &imRgbR);

  /**
  *\brief Retorna o tamanho da Imagem
  *\return Objeto cv::Size
  */
	inline cv::Size getSize(){return camSize;}
  /**
  *\brief Retorna a altura da Imagem
  *\return Variavel do tipo int
  */
  inline int getHeight(){return camSize.height;}
  /**
  *\brief Retorna a largura da Imagem
  *\return Variavel do tipo int
  */
	inline int getWidtht(){return camSize.width;}

  /**
  * \brief Retorna a matrix de transformação de perspectiva
  */
	inline cv::Mat getQ(){return Q;}

  /**
  *\brief Lê os parametros, de um arquivo, relativos a retificação
  */
  void readParameters();
};

#endif
