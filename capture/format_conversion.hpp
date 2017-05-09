#ifndef FORMAT_CONVERSION_HPP
#define FORMAT_CONVERSION_HPP

#include <cvd/image.h>
#include <cvd/byte.h>
#include <cvd/rgb.h>
#include <cvd/colourspaces.h>

//~ #include <opencv2/core/core.hpp>
//~ #include <opencv2/calib3d/calib3d.hpp>
//~ #include <opencv2/highgui/highgui.hpp>
//~ #include <opencv2/imgproc/imgproc.hpp>

#include "image.h"

/**
*\class FormatConversions
*\brief class to handle conversion between different types of images. ex:Image, image, Mat.
*/
class FormatConversions
{
	private:

	int width;
	int height;

	public:
	FormatConversions(){};
	/**
	* \brief Método utiliado para setar a altura e largura da imgagem.
	* \param w largura em pixels
	* \param h altura em pixels
	*/
	inline void init(int w, int h){ width=w; height=h;}

	/**
	*\brief Converte uma imagem da lib CVD no formato yuv422 para uma imagem da lib Libelas
	*\param from imagem no formato da libCVD com colorspace yuv422
	*\param to[out] imagem no formato da lib Libelas
	*/
	void CvdYuv422ToElas(const CVD::BasicImage<CVD::yuv422>& from, image<uchar>& to);

	/**
	* \brief Converte uma imagem rgb armazenada em um vector para uma preta e branca no formato da libCVD
	* \param std::vector<uint8_t> rgb Imagem rgb
	* \param[out] CVD::Image<CVD::byte> &imBW Variavel onde a imagem no formato da libCVD será salva
 	*/
	void RgbToBw_VectorToCvd(std::vector<uint8_t> rgb, CVD::Image<CVD::byte> &imBW);
	/**
	* \brief Converte uma imagem rgb armazenada em um vector para o formato da libCVD
	* \param std::vector<uint8_t> rgb Imagem rgb
	* \param[out] CVD::Image<CVD::byte> &imRGB Variavel onde a imagem no formato da libCVD será salva
	*/
	void Rgb_VectorToCvd(std::vector<uint8_t> rgb, CVD::Image<CVD::Rgb<CVD::byte> > &imRGB);

	/**
	* \brief Converte uma imagem preto e branco no formato da Libelas para outra preta e branca no formato da CVD
	* \param from imagem preta e branca do Libelas
	* \param[out] imBW imagem no formato CVD
	*/
	void Bw_ElasToCVD(const image<uchar> &from, CVD::Image<CVD::byte> &imBW);
	/**
	* \brief Converte a primeira imagen preto e branco no formato da Libelas para outra  preta e branca no formato da CVD
	* \param from imagem preta e branca do Libelas
	* \param[out] imBW imagem no formato CVD
	*/

	void Bw_ElasToCVD(const image<uchar> &from1, const image<uchar> &from2, CVD::Image<CVD::byte> &imBW1);
	/**
	* \brief Converte duas imagens preto e branco no formato da Libelas para outras 2 preta e branca no formato da CVD
	* \param from imagem preta e branca do Libelas
	* \param[out] imBW imagem no formato CVD
	*/
	void Bw_ElasToCVD(const image<uchar> &from1, const image<uchar> &from2, CVD::Image<CVD::byte> &imBW1 , CVD::Image<CVD::byte> &imBW2);

	//~ cv::Mat conversionCVDtoCV(const CVD::Image<CVD::byte> from);
};

#endif
