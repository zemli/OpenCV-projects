#include "opencv2/imgproc/imgproc.hpp"
#include "opencv/highgui.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>


using namespace cv;

/// Global variables

Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;

VideoCapture cap(0);
/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void CannyThreshold(int, void*)
{
  /// Reduce noise with a kernel 3x3
  blur( src_gray, detected_edges, Size(2,2) );

  /// Canny detector
  Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

  /// Using Canny's output as a mask, we display our result
  dst = Scalar::all(0);

  src.copyTo( dst, detected_edges);
  imshow( "Edge Dtection Result", dst );
 }


/** @function main */
int main( int argc, char** argv )
{

	if(!cap.isOpened())
	{
		std::cout<<"camera is not opened";
		return -1;
	}
	std::cout << "Canny edge detector is working" << std::endl;
	while(true){
		/// Load an image
		cap>>src;

		if( !src.data )
		{ return -1; }

		/// Create a matrix of the same type and size as src (for dst)
		dst.create( src.size(), src.type() );

		/// Convert the image to grayscale
		cvtColor( src, src_gray, CV_BGR2GRAY );

		/// Create a window
		namedWindow( "Edge Dtection Result", CV_WINDOW_AUTOSIZE );

		/// Create a Trackbar for user to enter threshold
		createTrackbar( "Min Threshold:", "Edge Dtection Result", &lowThreshold, max_lowThreshold, CannyThreshold );

		/// Show the image
		CannyThreshold(0, 0);

		/// Wait until user exit program by pressing a key
		if(cv::waitKey(1) >= 0) break;
	}

  return 0;

  }