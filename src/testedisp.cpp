#include <iostream>
#include <cvd/videodisplay.h>
#include <cvd/videosource.h>
#include <cvd/gl_helpers.h>
#include <cvd/esm.h>
#include <cvd/vision.h>
#include <stdint.h>

#include "VideoSource.hpp"
#include "elas.h"
#include <TooN/TooN.h>

int main()
{
	VideoSourceCVD camera(minoruCVD);//inicializa a camera q esta sendo utilizada
	CVD::ImageRef imageSize(VIDEO_W,VIDEO_H), displaySize(2*VIDEO_W,VIDEO_H) ,translation(VIDEO_W, 0), init(0,0);
	//obejtos que guardam as imagens capturadas
	std::vector< CVD::Image< CVD::Rgb<CVD::byte> >  > imRGB(2, imageSize);
	
	image<uchar> imBW1(VIDEO_W, VIDEO_H), imBW2(VIDEO_W, VIDEO_H);
	CVD::Image<CVD::byte> IBW1(imageSize),IBW2(imageSize), IBW_CORRECTED(imageSize);	

	CVD::Image< CVD::byte> disparityLeft(imageSize), disparityRight(imageSize);
	int32_t dims[3] = {VIDEO_W,VIDEO_H,VIDEO_W}; 
	
	CVD::VideoDisplay display(displaySize);
	display.set_title("Disparity");   
	CVD::VideoDisplay display2(displaySize);
	display2.set_title("RAW IMAGE");   

	//~ TooN::Matrix<3,3,double> homography_params;
	//~ CVD::Homography<2> homography;

	CVD::ESMEstimator< CVD::Homography<2>, CVD::StaticAppearance> distorcion;
	
	TooN::Matrix<3> prefix = TooN::Identity;
	//~ prefix(0,0) = 2.0 / VIDEO_W;
	//~ prefix(1,1) = 2.0 / VIDEO_W;
	//~ prefix(0,2) = -1;
	//~ prefix(1,2) = double(VIDEO_H) / double(VIDEO_W);
	
	
	
	while(true)
	{
		camera.GetAndFillFrameBWandRGB(imBW1, imBW2, imRGB);
		camera.conversion.Bw_ElasToCVD(imBW1, imBW2, IBW1, IBW2);
		
		float* D1_data = (float*)malloc(VIDEO_W*VIDEO_H*sizeof(float));
		float* D2_data = (float*)malloc(VIDEO_W*VIDEO_H*sizeof(float));
		
		Elas::parameters param(Elas::ROBOTICS);
		Elas elas(param);
		elas.process(imBW1.data , imBW2.data, D1_data, D2_data, dims);
		
				
		// find maximum disparity for scaling output disparity images to [0..255]//TODO::MELHORAR ESSA MERDA
		float disp_max =0;
		for (int32_t i=0; i<VIDEO_W*VIDEO_H; i++) 
		{
			if (D1_data[i]>disp_max) disp_max = D1_data[i];
			if (D2_data[i]>disp_max) disp_max = D2_data[i];
		}
		
		for (int i = 0; i < VIDEO_H; i++)
		{
			for (int j = 0; j < VIDEO_W; j++)
			{	
				disparityLeft[i][j] = (uint8_t)std::max(255.0*D1_data[i*VIDEO_W + j]/disp_max,0.0);
				disparityRight[i][j] = (uint8_t)std::max(255.0*D2_data[i*VIDEO_W + j]/disp_max,0.0);
			}
		}
		
		display.make_current();
		CVD::glRasterPos(init);
		CVD::glDrawPixels(disparityLeft);
		CVD::glRasterPos(translation);
		CVD::glDrawPixels(disparityRight);
 
 
		//TESTE DA HOMOGRAFIA
		std::cout << "H1 H2 H3 H4 H5 H6 H7 H8 H9 " << "err^2 pixels RMSE delta iterations [appearance]\n";
		distorcion.reset();
		//~ distorcion.transform.set_prefix(prefix);
        distorcion.optimize(IBW1, IBW2);
        std::cout << distorcion.transform.get_matrix()[0] << distorcion.transform.get_matrix()[1] << distorcion.transform.get_matrix()[2];
        std::cout << distorcion.result;
		
		//~ std::cout << distorcion.appearance;//?????
		
		std::cout << std::endl;
 
		CVD::transform(IBW1, IBW_CORRECTED, distorcion.transform.get_matrix());
 
 
		display2.make_current();
		CVD::glRasterPos(init);
		CVD::glDrawPixels(IBW_CORRECTED);
		//~ CVD::glDrawPixels(IBW1);
		CVD::glRasterPos(translation);
		CVD::glDrawPixels(IBW2);
						
	
		glEnd();
			
		glFlush();
		free(D1_data);
		free(D2_data);
		//~ free(I1);
		//~ free(I2);
	}
}
