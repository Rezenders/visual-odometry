#include <iostream>
#include <opencv2/opencv.hpp>

cv::VideoCapture vcStream;
cv::Mat mImage;
volatile bool bTerminated = false;
pthread_mutex_t pmImgMutex;

void* readThread(void*)
{
	cv::Mat mTempImg;
	while (!bTerminated)
	{
		vcStream.read(mTempImg);
		pthread_mutex_lock (&pmImgMutex);
		mImage = mTempImg;
		pthread_mutex_unlock (&pmImgMutex);
	}
	pthread_exit((void*) 0);
	return NULL;
}

int main()
{
	pthread_t ptReader;
	pthread_attr_t ptaJoinable;
	cv::Mat mLocalImg;

	const std::string sURL = "/dev/video0";

	if (!vcStream.open("/dev/video2"))
	{
		std::cerr << "ERRO: não foi possível abrir o fluxo de vídeo" << std::endl;
		return -1;
	}

    pthread_attr_init(&ptaJoinable);
    pthread_attr_setdetachstate(&ptaJoinable, PTHREAD_CREATE_JOINABLE);
	pthread_mutex_init(&pmImgMutex, NULL);
	int rc = pthread_create(&ptReader, &ptaJoinable, readThread, NULL);
	pthread_attr_destroy(&ptaJoinable);

	if (rc)
	{
		std::cerr << "ERRO: não foi possível criar a thread de leitura" << std::endl;
		return -1;
	}

	while (cv::waitKey(30) != 27)
	{
		//vcStream.read(mImage);

		pthread_mutex_lock (&pmImgMutex);
		mLocalImg = mImage;
		pthread_mutex_unlock (&pmImgMutex);
		if (!mLocalImg.empty())
			cv::imshow("Fluxo de vídeo",mLocalImg);
	}
	bTerminated = true;

	pthread_join(ptReader, NULL);
    pthread_mutex_destroy(&pmImgMutex);
	return 0;
}
