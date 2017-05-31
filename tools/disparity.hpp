#ifndef DISPARITY_HPP
#define DISPARITY_HPP

#include <opencv2/opencv.hpp>
#include "opencv2/ximgproc/disparity_filter.hpp"

enum class disp_t {BM, BMG, BMWLS, ELAS};

class Disparity {
protected:
  disp_t disp_type;

  cv::Mat left_disp, right_disp;
  cv::Mat filtered_disp;

public:
  Disparity(){std::cout<<"Construtor padrão de Disparity"<<std::endl;};
  // Disparity(disp_t type);

  static Disparity *create(disp_t type);

  /**
  * \brief Calcula o mapa de disparidade para as imagens passadas
  * \param left_img imagem retificada da esquerda
  * \param right_img imagem retificada da direita
  **/
  virtual void apply_disparity(cv::Mat left_img, cv::Mat right_img) =0;

  /**
  * \brief Retorna o mapa de disparidade da imagem da esquerda
  **/
  inline cv::Mat get_left_disp(){return left_disp;};
  /**
  * \brief Retorna o mapa de disparidade da imagem da direita
  **/
  inline cv::Mat get_right_disp(){return right_disp;};
  /**
  * \brief Retorna o mapa de disparidade filtadro da imagem da esquerda
  **/
  inline cv::Mat get_filtered_disp(){return filtered_disp;};

  /**
  * \brief Retorna o mapa de disparidade da imagem esquerda  proprio para vizualização com uma escala
  * \param scale escala para o mapa de disparidade
  **/
  cv::Mat get_left_disp_vis(unsigned int scale = 5);
  /**
  * \brief Retorna o mapa de disparidade da imagem direita  proprio para vizualização com uma escala
  * \param scale escala para o mapa de disparidade
  **/
  cv::Mat get_right_disp_vis(unsigned int scale = 5);
  /**
  * \brief Retorna o mapa de disparidade filtadro da imagem esquerda proprio para vizualização com uma escala
  * \param scale escala para o mapa de disparidade
  **/
  cv::Mat get_filtered_disp_vis(unsigned int scale = 5);


};

class DisparityBM : public Disparity{

private:
  int numDisparities = 64, SADWindowSize = 15;
  int minDisparity = -30, speckleWindowSize = 0, speckleRange = 1000,
      textureThreshold = 3100, uniquessRatio = 1, prefilterCap = 63,
      prefilterSize = 255;

  cv::Ptr<cv::StereoBM> left_matcher =
      cv::StereoBM::create(numDisparities, SADWindowSize);
  cv::Ptr<cv::StereoMatcher> right_matcher =
      cv::ximgproc::createRightMatcher(left_matcher);

  unsigned int lambda =8000, sigma_color= 2.0;
  cv::Ptr<cv::ximgproc::DisparityWLSFilter> wls_filter;

public:
  DisparityBM(disp_t type);

  void set_all_params();
  void apply_disparity(cv::Mat left_img, cv::Mat right_img);

};

#endif
