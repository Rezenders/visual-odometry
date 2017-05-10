#include "VideoSource.hpp"
#include <string>

void SetStringName(std::string &left, std::string &right,int num);

int main(){
  VideoSource cam(640, 480);

  cv::Mat imBwL, imBwR, imRgbL, imRgbR;
  cv::Mat imRgbFull(cam.getSize(), CV_8UC3);

  cv::namedWindow("Video", cv::WINDOW_AUTOSIZE);

  //strings com o caminho das imagens para serem salvas
	std::string leftImagePath, rightImagePath;

  while(true){
    cam.grabRawRgbBw(imBwL, imBwR, imRgbL, imRgbR);
    cv::hconcat(imRgbL, imRgbR, imRgbFull);
    imshow("Video", imRgbFull);


    int time_ms = cv::waitKey(30);
    //waits for esc key to exit
    if(time_ms == 32){//spacebar
      std::cout<<" Space bar was pressed "<<std::endl;
      static int image_number =1;
      SetStringName(leftImagePath, rightImagePath, image_number);
      std::cout<<rightImagePath<<std::endl;
      imwrite(leftImagePath, imRgbL);
      imwrite(rightImagePath, imRgbR);
      image_number++;
    }else
    if(time_ms == 27) break;//esc
  }
}

void SetStringName(std::string &left, std::string &right, int num)
{
	left.clear();
	right.clear();

	left.append("../../calib/calib_imgs/left/left");
	right.append("../../calib/calib_imgs/right/right");

	left.append(std::to_string(num));
	right.append(std::to_string(num));

	left.append(".png");
	right.append(".png");

}
