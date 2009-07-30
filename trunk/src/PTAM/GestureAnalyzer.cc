
#include <TooN/helpers.h>

#include "GestureAnalyzer.h"

#include "OpenCVInterf.h"	//Convert utils
#include "gesrec.h"		//gesture recognition header



//#define __DEBUG

#ifdef __DEBUG
#include "gvars3/instances.h"
using namespace CVD;
using namespace std;
using namespace GVars3;
#endif


#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif


pthread_mutex_t imageQueueMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t resultQueueMutex = PTHREAD_MUTEX_INITIALIZER;



void GestureAnalyzer::reset()
{
	mbResetDone = true;
	mbResetRequested = false;
}
// CHECK_RESET is a handy macro which makes the mapmaker thread stop
// what it's doing and reset, if required.
#define CHECK_RESET if(mbResetRequested) {reset(); continue;};


void GestureAnalyzer::run()
{
#ifdef WIN32
	// For some reason, I get tracker thread starvation on Win32 when
	// adding key-frames. Perhaps this will help:
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);
#endif
	while(!shouldStop())  // ShouldStop is a CVD::Thread func which return true if the thread is told to exit.
	{
		CHECK_RESET;
		sleep(5); // Sleep not really necessary, especially if mapmaker is busy
		CHECK_RESET;

		#ifdef __DEBUG
		//just for test
		//GUI.ParseLine("test");
		//std::cout<<"test"<<std::endl;
		#endif

		analyze();
		CHECK_RESET;
	}
}

GestureAnalyzer::GestureAnalyzer()
{
	reset();
	start();
}

void GestureAnalyzer::analyze() /* Invoked by run(), fake method generating */
{
	RGBImage image;
	GestureData gestureData;
	
	if(mqRGBImageQueue.size() > 0){

		//do synchronize here
		pthread_mutex_lock(&imageQueueMutex);

	#ifdef __DEBUG
			cout<<"image queue size:"<<mqRGBImageQueue.size()<<endl;
	#endif
			//get image to analyze
			image = mqRGBImageQueue.front();
			//pop out from the queue
			mqRGBImageQueue.pop_front();

			//in case that image processing is too slow
			if(mqRGBImageQueue.size() > 10){
				mqRGBImageQueue.clear();
			}

		//release image queue lock
		pthread_mutex_unlock(&imageQueueMutex);


		//Convert to ipl image
		IplImage* pCVImage = ::ToIplImage(image);
		cvCvtColor(pCVImage, pCVImage, CV_RGB2BGR);
		
		CvRect rect;

		if(gesARDetectHand(pCVImage, &rect))
			;
		//TODO:set valid
		//gestureData.mbIsValid = true;


		//int left = rect.x;
		//int top = rect.y;
		//int width = rect.width;
		//int height = rect.height;

		//gesture recognition
		gestureData.mCenterPosition = makeVector(  rect.x + rect.width/2 ,rect.y + rect.height/2,0);
		
		gestureData.left = rect.x;
		gestureData.top = rect.y;
		gestureData.width = rect.width;
		gestureData.height = rect.height;

		//cvSaveImage("result.jpg", pCVImage);

	
		//must release image here to avoid memory leak
		cvReleaseImage(&pCVImage);


		//do synchronize here
		pthread_mutex_lock(&resultQueueMutex);
			mqGestureDataQueue.push_back(gestureData);
		pthread_mutex_unlock(&resultQueueMutex);
	}
}

void GestureAnalyzer::addImage( RGBImage& imageRef ) /* Invoked by system thread to add camera image. */
{
	//do synchronize
	pthread_mutex_lock(&imageQueueMutex);

		//RGBImage* image = new RGBImage;
		//*image = imageRef;
		//RGBImage image = imageRef;
		mqRGBImageQueue.push_back(imageRef);

	//release lock
	pthread_mutex_unlock(&imageQueueMutex);
}

GestureData GestureAnalyzer::getGesture()
{
	GestureData gesture;
	if(mqGestureDataQueue.size() > 0){
	
	//do synchronize here
	pthread_mutex_lock(&resultQueueMutex);
#ifdef __DEBUG
	cout<<"result queue size:"<<mqGestureDataQueue.size()<<endl;
#endif
		//get gesture data
		gesture = mqGestureDataQueue.front();
		//pop from the queue
		mqGestureDataQueue.pop_front();
	//release lock
	pthread_mutex_unlock(&resultQueueMutex);

	//set as valid gesture
	gesture.mbIsValid = true;

	}
	else{
		gesture.mbIsValid = false;
	}

	return gesture;
}


























//#include <stdio.h>
//
//char output[40];
//
//void histogram(IplImage* pImg, IplImage* histImg);
//int testImgDetectHandRange(int argc, char** argv);
//int testCamDetectHandRange();
//int testImgFindContours(int argc, char** argv);
//
//void histogram(IplImage* pImg, IplImage* histImg)
//{
//	CvHistogram* hist;
//	int bin_w;
//	float max_val;
//
//	hist = gesSampleSkinHistogram(pImg, cvRect(0,0,pImg->width,pImg->height));
//	cvZero(histImg);
//	cvGetMinMaxHistValue(hist, 0, &max_val, 0, 0);
//	cvConvertScale(hist->bins, hist->bins, max_val?255./max_val:0., 0);
//	bin_w = histImg->width/256;
//
//	for(int i = 0;i < 256;i++)
//	{
//		double val = (cvGetReal1D(hist->bins, i)*histImg->height/255);
//		CvScalar color = CV_RGB(255, 255, 0);
//		cvRectangle(histImg, cvPoint(i*bin_w,histImg->height), cvPoint((i+1)*bin_w,(int)(histImg->height-val)), color, 1, 8, 0);
//	}
//
//	cvReleaseHist(&hist);
//}
//
//int testImgDetectHandRange(int argc, char** argv)
//{
//	IplImage* sampleImg;//样本图片
//	IplImage* pImg = 0;//要检测的图片
//	IplImage* outImg;//输出的结果图片
//	CvScalar s;
//	CvSeq* comp;//连通部件
//	CvMemStorage* storage;//动态内存
//	//CvHistogram* hist;
//
//	//载入图像
//	if( argc == 3 && 
//		(pImg = cvLoadImage( argv[1], 1)) != 0 &&
//		(sampleImg = cvLoadImage(argv[2], 1)) != 0)
//	{
//		cvNamedWindow( "Image", 1 );//创建窗口
//		cvShowImage( "Image", pImg );//显示图像
//
//		//初始化动态内存与连通部件
//		storage = cvCreateMemStorage(0);
//		comp = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvConnectedComp), storage);
//
//		//获得样本图片的直方图
//		/*sampleImg = cvLoadImage("skinsample.JPG", 1);
//		hist = gesSampleSkin(sampleImg, cvRect(0,0,sampleImg->width,sampleImg->height));
//
//		//生成输出图片
//		outImg = cvCloneImage(pImg);
//		gesDetectHandHistogram(pImg, outImg, hist, cvRect(0,0,sampleImg->width,sampleImg->height));*/
//
//		//获得样本图片的肤色范围
//		gesSampleSkinRange(sampleImg, &s);
//		sprintf_s(output, "%.2f %.2f %.2f\n", s.val[0], s.val[1], s.val[2]);
//		MessageBox(NULL, output, NULL, MB_OK);
//
//		//生成输出图片
//		outImg = cvCloneImage(pImg);
//		gesDetectHandRange(pImg, outImg, storage, comp, &s, 1);
//		//gesDetectHandRange(pImg, outImg);
//
//		cvNamedWindow("Output", 1);
//		cvShowImage("Output", outImg);
//
//		cvWaitKey(0); //等待按键
//
//		//cvReleaseHist(&hist);
//		cvReleaseMemStorage(&storage);
//		cvDestroyWindow("Output");
//		cvReleaseImage(&outImg);
//		cvDestroyWindow( "Image" );//销毁窗口
//		cvReleaseImage( &pImg ); //释放图像
//		return 0;
//	}
//
//	return -1;
//}
//
//int testCamDetectHandRange(int argc, char** argv)
//{
//	CvCapture* capture = 0;
//	IplImage* output = 0;
//	IplImage* sampleImg;//样本图片
//	CvScalar s;
//	CvSeq* comp;//连通部件
//	CvMemStorage* storage;//动态内存
//
//	if(argc == 2)
//	{
//		if((sampleImg = cvLoadImage(argv[1], 1)) == 0)
//		{
//			fprintf(stderr, "Could not open sample image\n");
//			return 0;
//		}
//		else
//		{
//			//获得样本图片的肤色范围
//			gesSampleSkinRange(sampleImg, &s);
//			cvReleaseImage(&sampleImg);
//		}
//	}
//
//	capture = cvCaptureFromCAM(0);
//	if(!capture)
//	{
//		fprintf(stderr, "Could not initialize capturing...\n");
//		return -1;
//	}
//
//	cvNamedWindow("Input", 1);
//	cvNamedWindow("Output", 1);
//
//	//初始化动态内存与连通部件
//	storage = cvCreateMemStorage(0);
//	comp = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvConnectedComp), storage);
//
//	//循环捕捉,直到用户按键跳出循环体
//	for( ; ; )
//	{
//		IplImage* input = 0;
//
//		input = cvQueryFrame(capture);
//		if(!input)
//		{
//			break;
//		}
//
//		cvReleaseImage(&output);
//		output = cvCloneImage(input);
//		if(argc == 1)
//		{
//			gesDetectHandRange(input, output, storage, comp);
//		}
//		else
//		{
//			gesDetectHandRange(input, output, storage, comp, &s, 1);
//		}
//
//		cvShowImage("Input", input);
//		cvShowImage("Output", output);
//		if(cvWaitKey(10) >= 0)
//		{
//			break;
//		}
//	}
//
//	cvReleaseCapture(&capture);
//	cvReleaseMemStorage(&storage);
//	cvDestroyWindow("Input");
//	cvDestroyWindow("Output");
//
//	return 1;
//}
//
//int testImgFindContours(int argc, char** argv)
//{
//	IplImage* pImg = 0;//要检测的图片
//	IplImage* outImg;//输出的结果图片
//
//	//载入图像
//	if( argc == 2 && 
//		(pImg = cvLoadImage( argv[1], 1)) != 0)
//	{
//		cvNamedWindow( "Image", 1 );//创建窗口
//		cvShowImage( "Image", pImg );//显示图像
//
//		outImg = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);
//		gesFindContours(pImg, outImg);
//
//		cvNamedWindow("Output", 1);
//		cvShowImage("Output", outImg);
//
//		cvWaitKey(0); //等待按键
//
//		cvDestroyWindow("Output");
//		cvReleaseImage(&outImg);
//		cvDestroyWindow( "Image" );//销毁窗口
//		cvReleaseImage( &pImg ); //释放图像
//		return 0;
//	}
//
//	return 0;
//}
//
//int testCamFindContours()
//{
//	CvCapture* capture = 0;
//	IplImage* output = 0;
//	IplImage* input = 0;
//
//	capture = cvCaptureFromCAM(0);
//	if(!capture)
//	{
//		fprintf(stderr, "Could not initialize capturing...\n");
//		return -1;
//	}
//	cvNamedWindow("Input", 1);
//	cvNamedWindow("Output", 1);
//
//	//捕捉第一帧以初始化输出
//	input = cvQueryFrame(capture);
//	if(!input)
//	{
//		return 0;
//	}
//	output = cvCreateImage(cvGetSize(input), IPL_DEPTH_8U, 1);
//
//	//循环捕捉,直到用户按键跳出循环体
//	for( ; ; )
//	{
//		input = cvQueryFrame(capture);
//		if(!input)
//		{
//			break;
//		}
//
//		cvZero(output);
//		gesFindContours(input, output);
//
//		cvShowImage("Input", input);
//		cvShowImage("Output", output);
//		if(cvWaitKey(10) >= 0)
//		{
//			break;
//		}
//	}
//
//	cvReleaseCapture(&capture);
//	cvDestroyWindow("Input");
//	cvDestroyWindow("Output");
//	cvReleaseImage(&output);
//
//	return 0;
//}
//
//int getImgFromCAM(int argc, char** argv)
//{
//	CvCapture* capture = 0;
//
//	capture = cvCaptureFromCAM(0);
//	if(!capture)
//	{
//		fprintf(stderr, "Could not initialize capturing...\n");
//		return -1;
//	}
//
//	cvNamedWindow("Input", 1);
//
//	//循环捕捉,直到用户按键跳出循环体
//	for( ; ; )
//	{
//		IplImage* input = 0;
//
//		input = cvQueryFrame(capture);
//		if(!input)
//		{
//			break;
//		}
//
//		cvShowImage("Input", input);
//		if(cvWaitKey(10) >= 0)
//		{
//			if(argc == 2)
//			{
//				if(!cvSaveImage(argv[1], input))
//				{
//					printf("Could not save : %s\n", argv[1]);
//				}
//			}
//			break;
//		}
//	}
//
//	cvReleaseCapture(&capture);
//	cvDestroyWindow("Input");
//
//	return 1;
//}