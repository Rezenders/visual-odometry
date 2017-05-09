#include <iostream>
#include <cvd/fast_corner.h>
#include <cvd/videodisplay.h>
#include <cvd/videosource.h>
#include <cvd/gl_helpers.h>
#include "VideoSource.hpp"



int main()
{
	VideoSourceCVD camera(minoruCVD);
	CVD::ImageRef imageSize = camera.Size(), displaySize(2*VIDEO_W,VIDEO_H) ,translation(VIDEO_W, 0), init(0,0);
	
	CVD::Image< CVD::Rgb<CVD::byte> >  imRGB1(imageSize), imRGB2(imageSize);
	CVD::Image<CVD::byte>  imBW1(imageSize), imBW2(imageSize);
	
	CVD::VideoDisplay display(displaySize);
	display.set_title("MAPL HUEHUEBRBR");  
		
	while(true)
	{
		camera.GetAndFillFrameBWandRGB(imBW1, imBW2, imRGB1, imRGB2);

		
		std::vector<CVD::ImageRef> corners;
		
		CVD::fast_corner_detect_9_nonmax(imBW1, corners, 15);
		
		CVD::glRasterPos(init);
		CVD::glDrawPixels(imBW1);
		
		CVD::glRasterPos(translation);
		CVD::glDrawPixels(imRGB1);
		CVD::Rgb8 color_red(255,0,0);
		CVD::glColor3(color_red);
		glBegin(GL_POINTS);
		CVD::glVertex(corners);
		glEnd();
		glFlush();
				
	}
}
