#include "opencv2/imgproc/imgproc.hpp"
#include "opencv/highgui.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>


using namespace cv;

/// Global variables
Mat src;
VideoCapture cap(0);



const int alpha_slider_max = 100;
int value ;

Mat dest;
Mat src_f;
cv::Mat kernel;

///
void foo();

void on_trackbar( int, void*)
{
	int kernel_size = 30;
	double  th = 0, lm = 20.0, gm = 0.5, ps = 0;
	double sigma = value/10.0;
	kernel = cv::getGaborKernel(cv::Size(kernel_size,kernel_size), sigma, th, lm, gm, ps);
	cv::filter2D(src_f, dest, CV_32F, kernel);


	Mat viz;
	dest.convertTo(viz,CV_8U,1.0/255.0);     // move to proper[0..255] range to show it
	imshow("Linear Blend",viz);
}

void foo(){
	Mat in;
	cvtColor(src, in, COLOR_RGB2GRAY);// load grayscale

	in.convertTo(src_f,CV_32F);//convert data type to float 

	

	//std::cerr << dest(Rect(30,30,10,10)) << std::endl; // peek into the data
	
	namedWindow("Linear Blend", 1);
	createTrackbar( "Track Bar", "Linear Blend", &value, alpha_slider_max, on_trackbar );
	on_trackbar( value, 0);

	
	imshow("k",kernel);
	
}

/** @function main */
int main( int argc, char** argv )
{

	if(!cap.isOpened())
	{
		std::cout<<"camera is not opened";
		return -1;
	}

	while(true){
		/// Load an image
		cap>>src;

		if( !src.data )
		{ return -1; }

		foo();

		/// Wait until user exit program by pressing a key
		if(cv::waitKey(1) >= 0) break;
	}

  return 0;

  }