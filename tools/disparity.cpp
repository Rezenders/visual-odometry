#include "disparity.hpp"

Disparity  *Disparity::create(disp_t type){
  if(type == disp_t::BM || type == disp_t::BMG || type == disp_t::BMWLS){
    return new DisparityBM(type);
  }else{
    return new DisparityBM(type);
  }
}

cv::Mat Disparity::get_left_disp_vis(unsigned int scale){
  cv::Mat aux_vis;
  cv::ximgproc::getDisparityVis(left_disp, aux_vis, scale);
  return aux_vis;
}

cv::Mat Disparity::get_right_disp_vis(unsigned int scale){
  cv::Mat aux_vis;
  cv::ximgproc::getDisparityVis(right_disp, aux_vis, scale);
  return aux_vis;
}

cv::Mat Disparity::get_filtered_disp_vis(unsigned int scale){
  cv::Mat aux_vis;
  if(disp_type != disp_t::BMWLS){
    filtered_disp = left_disp;

  }
  cv::ximgproc::getDisparityVis(filtered_disp, aux_vis, scale);
  return aux_vis;
}

////////////////////////////////DisparityBM//////////////////

DisparityBM::DisparityBM(disp_t type){
  set_all_params();
  std::cout<<"Construtor de disparityBM"<<std::endl;
}

void DisparityBM::set_all_params(){
  left_matcher->setMinDisparity(minDisparity);
  left_matcher->setSpeckleWindowSize(speckleWindowSize);
  left_matcher->setSpeckleRange(speckleRange);
  left_matcher->setTextureThreshold(textureThreshold);
  left_matcher->setUniquenessRatio(uniquessRatio);
  left_matcher->setPreFilterCap(prefilterCap);
  left_matcher->setPreFilterSize(prefilterSize);
}

void DisparityBM::apply_disparity(cv::Mat left_img, cv::Mat right_img){
  switch(disp_type){
    case disp_t::BMG:
      cv::GaussianBlur(left_img, left_img, cv::Size(35,35),0,0);
      cv::GaussianBlur(right_img, right_img, cv::Size(35,35),0,0);
    break;
    default:
    break;
  }

  left_matcher->compute(left_img, right_img, left_disp);
  right_matcher->compute(right_img, left_img, right_disp);

  if(disp_type == disp_t::BMWLS){
    wls_filter = cv::ximgproc::createDisparityWLSFilter(left_matcher);
    wls_filter->setLambda(lambda);
    wls_filter->setSigmaColor(sigma_color);
    wls_filter->filter(left_disp, left_img, filtered_disp, right_disp);
  }
}
