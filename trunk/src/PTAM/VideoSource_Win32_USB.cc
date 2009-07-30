
#define WIN32_LEAN_AND_MEAN
#include "VideoSource.h"
#include <Windows.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <cvd/utility.h>

using namespace CVD;
using namespace std;

VideoSource::VideoSource()
{ 
	CvCapture* capture;
	capture = cvCaptureFromCAM(0);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 640 );
	//cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 320 );
	//cvSetCaptureProperty(capture, CV_CAP_PROP_CONVERT_RGB,1);
	if( !capture )
	{
		fprintf(stderr,"Could not initialize capturing...\n");
		exit(1);
	}
	printf("Camera ready, press r to capture the first frame\n");
	mptr=capture;
	
	mirSize.x = 640; //cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH);
	mirSize.y = 480; //cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT);
	//mirSize.x = 320; //cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH);
	//mirSize.y = 240; //cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT);
};

void VideoSource::GetAndFillFrameBWandRGB(Image<CVD::byte> &imBW, Image<CVD::Rgb<CVD::byte> > &imRGB)
{
	CvCapture *pCamera = (CvCapture*) mptr;
	IplImage *frameRGB = cvQueryFrame( pCamera );
	if (!frameRGB) 
	{
		fprintf(stderr, "Could not capture image");
		exit(-1);
	}	
	//unsigned long nLength;
	IplImage *frameBW=cvCreateImage(cvGetSize(frameRGB),frameRGB->depth,1);
	cvCvtColor(frameRGB, frameRGB, CV_BGR2RGB);
	cvCvtColor(frameRGB, frameBW, CV_RGB2GRAY);
	//cvReleaseImage(&frame);
	//cvFlip(frameBW,NULL,0);
	//cvFlip(frameRGB,NULL,0);
	byte *pImageBW = (byte*)frameBW->imageData; //pCamera->GetRawData(&nLength);
	Rgb<byte> *pImageRGB = (Rgb<byte>*)frameRGB->imageData;
	BasicImage<byte> imCapturedBW(pImageBW, mirSize);
	BasicImage<Rgb<byte>> imCapturedRGB(pImageRGB, mirSize);	
	copy(imCapturedBW, imBW);
	copy(imCapturedRGB, imRGB);
	cvReleaseImage(&frameBW);
}

ImageRef VideoSource::Size()
{
	return mirSize;
}
