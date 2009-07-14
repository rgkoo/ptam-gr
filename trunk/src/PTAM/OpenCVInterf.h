#ifndef CONVERT_UTILS_HEADER
#define CONVERT_UTILS_HEADER

/**
*	CV convert utilities.
*/

#include <cv.h>
#include <highgui.h>
#include <cvd\image.h>

#include "KeyFrame.h"

inline CvSize ToCvSize(CVD::ImageRef irSize)
{
	return cvSize(irSize.x,irSize.y);
}

inline IplImage* ToIplImage(KeyFrame& kfFrame, int nLevel = 0)
{
	IplImage* image = cvCreateImage(ToCvSize(kfFrame.aLevels[nLevel].im.size()), 8, 1);
	assert(image);
	memcpy(image->imageData,kfFrame.aLevels[nLevel].im.data(),kfFrame.aLevels[nLevel].im.totalsize()*sizeof(uchar));
	return image;
}

inline IplImage* ToIplImage(Level& lFrame)
{
	IplImage* image = cvCreateImageHeader(ToCvSize(lFrame.im.size()), 8, 1);
	cvSetImageData(image,lFrame.im.data(),image->widthStep);
	IplImage* image2 = cvCreateImage(cvSize(image->width,image->height), image->depth, image->nChannels); 
	cvCopy(image,image2);
	cvReleaseImageHeader(&image);
	//cvSaveImage("d:\\badliu.bmp",image2);
	return image2;
}

inline IplImage* ToIplImage(CVD::Image<CVD::Rgb<CVD::byte> >& cvdImage){
	IplImage* image = cvCreateImageHeader(ToCvSize(cvdImage.size()), 8, 3);
	cvSetImageData(image,	cvdImage.data(),	image->widthStep);
	IplImage* image2 = cvCreateImage(cvSize(image->width,image->height), image->depth, image->nChannels); 
	cvCopy(image,image2);
	cvReleaseImageHeader(&image);

	return image2;
}

#endif