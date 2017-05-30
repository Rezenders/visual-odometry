#include "disparity.hpp"

void Disparity::apply_disparity(cv::Mat left_img, cv::Mat right_img){
  left_matcher->compute(left_img, right_img, left_disp);
  right_matcher->compute(right_img, left_img, right_disp);

  if(disp_type == disp_t::BMWLS){
    wls_filter = cv::ximgproc::createDisparityWLSFilter(left_matcher);
    wls_filter->setLambda(lambda);
    wls_filter->setSigmaColor(sigma_color);
    wls_filter->filter(left_disp, left_img, filtered_disp, right_disp);
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
  if(filtered_disp.data == NULL){
    filtered_disp = left_disp;
  }
  cv::ximgproc::getDisparityVis(filtered_disp, aux_vis, scale);
  return aux_vis;
}