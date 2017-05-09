#include "format_conversion.hpp"

void FormatConversions::CvdYuv422ToElas(const CVD::BasicImage<CVD::yuv422>& from, image<uchar>& to)
{
	//yuv422 / vuy422 is along the lines of yuyv
	//which is 4 bytes for 2 pixels, i.e. 2 bytes per pixel //y1:0 uu:1 y2:2 vv:3
			
	size_t bytes_per_row = width * 2;
	
	for(int y=0; y < height; y++)
	{
		const unsigned char* yuv = reinterpret_cast<const unsigned char*>(from.data()) + bytes_per_row*y;
	
		for(int x=0; x < width; x+=2, yuv+=4)
		{
			to.data[(x+0) + y*width] = yuv[0];//y1
			to.data[(x+1) + y*width] = yuv[2];//y2
		}
	}
}

void FormatConversions::RgbToBw_VectorToCvd(std::vector<uint8_t> rgb, CVD::Image<CVD::byte> &imBW){

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{	
			imBW[i][j] = (rgb[0+(i*width +j)*3] + rgb[1+(i*width +j)*3] + rgb[2+(i*width +j)*3]) / 3;
		}
	}

}

void FormatConversions::Rgb_VectorToCvd(std::vector<uint8_t> rgb, CVD::Image<CVD::Rgb<CVD::byte> > &imRGB){
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){	
			imRGB[i][j].red   = rgb[0+(i*width +j)*3];
			imRGB[i][j].green = rgb[1+(i*width +j)*3];
			imRGB[i][j].blue  = rgb[2+(i*width +j)*3];
		}
	}
}

void FormatConversions::Bw_ElasToCVD(const image<uchar> &from, CVD::Image<CVD::byte> &imBW)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{	
			imBW[i][j] = from.data[i*width +j];
		}
	}
}

void FormatConversions::Bw_ElasToCVD(const image<uchar> &from1, const image<uchar> &from2, CVD::Image<CVD::byte> &imBW1)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{	
			imBW1[i][j] = from1.data[i*width +j];
			imBW1[i][j+width] = from2.data[i*width +j];
		}
	}
}

void FormatConversions::Bw_ElasToCVD(const image<uchar> &from1, const image<uchar> &from2, CVD::Image<CVD::byte> &imBW1, CVD::Image<CVD::byte> &imBW2)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{	
			imBW1[i][j] = from1.data[i*width +j];
			imBW2[i][j] = from2.data[i*width +j];
		}
	}
}

//~ cv::Mat FormatConversions::conversionCVDtoCV(CVD::Image<CVD::byte> from)
//~ {
	//~ cv::Mat to(height, width, CV_8UC1,from.data());
	//~ 
	//~ 
	//~ return to;
//~ }
