#include <iostream>
#include <cvd/videodisplay.h>
#include <cvd/videosource.h>
#include <cvd/gl_helpers.h>
#include <cvd/image_io.h>
#include <stdint.h>
#include <thread>
#include <string>

#include "VideoSource.hpp"

void StringName(std::string &left, std::string &right,int num);

int main()
{
	VideoSourceCVD camera(minoruCVD);//inicializa a camera q esta sendo utilizada
	CVD::ImageRef imageSize(VIDEO_W,VIDEO_H), displaySize(2*VIDEO_W,VIDEO_H) ,translation(VIDEO_W, 0), init(0,0);
	//obejtos que guardam as imagens capturadas
	CVD::Image< CVD::Rgb<CVD::byte> > imRGBl(imageSize),imRGBr(imageSize);
	CVD::Image<CVD::byte> imBWl(imageSize),imBWr(imageSize);	
	
	CVD::VideoDisplay display(displaySize);
	display.set_title("Original");   
	display.select_events(KeyPressMask);
	
	XEvent event;
	
	int numImages =1;
	std::string leftImage,rightImage;
	
	while(true)
	{	
		camera.GetAndFillFrameBWandRGB(imBWl, imBWr, imRGBl, imRGBr);
		display.make_current();
		
		
		if(display.pending()>0)
		{
		 	display.get_event(&event);
			if (event.type == KeyPress)
			{
				if( event.xkey.keycode==65)
				{
					std::cout<<" Space bar was pressed "<<std::endl;
					StringName(leftImage,rightImage, numImages);
					img_save(imBWl, leftImage);
					img_save(imBWr, rightImage);
					numImages++;
				}else
				/* exit on ESC key press */
				if ( event.xkey.keycode == 0x09 )
				{
					glFlush();
					break;
				}
			}
				
		}	
  		
		CVD::glRasterPos(init);
		CVD::glDrawPixels(imBWl);
		CVD::glRasterPos(translation);
		CVD::glDrawPixels(imBWr);
						
		glFlush();
	}
	return 0;
}

void StringName(std::string &left, std::string &right, int num)
{
	left.clear();
	right.clear();
	
	left.append("../../calib/calib_imgs/left/left");
	right.append("../../calib/calib_imgs/right/right");
	
	left.append(std::to_string(num));
	right.append(std::to_string(num));

	left.append(".jpg");
	right.append(".jpg");	
	
}

