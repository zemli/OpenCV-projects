#include <stdio.h>
#include <stdlib.h>
//#include <windows.h>
#include <deque>

#include "opencv/highgui.h"
#include "opencv2/opencv.hpp"
#include <opencv2/video/tracking.hpp>

#include "../stasm/stasm_lib.h"
#include "../stasm/stasm_landmarks.h"

#include "../VisualStudio/gazedir.h"
#include "../VisualStudio/drowsydet.h"

/*
Global variables
*/
static const char* path = "./img.jpg";
cv::VideoCapture cap(0);
int foundface;
float landmarks[2 * stasm_NLANDMARKS];
float landmarks0[2 * stasm_NLANDMARKS];
cv::Mat_<unsigned char> img, frame;

//Optical flow
cv::Mat_<unsigned char> prev_frame;
std::vector<cv::Point2f> features_prev, features_next;
//std::vector<float> features_next, features_prev;
std::vector<uchar> status;
std::vector<float> err;

//new Mat
cv::Mat cropped;
cv::Mat dst, detected_edges; 


/*
function declared
*/
int init();


void force_points_into_image(std::vector<cv::Point2f> &pointV, int ncols, int nrows);

/*
function defined in main.cpp
*/

int init(){
	if(!cap.isOpened()){
		std::cout<<"camera is not opened";
		return 0;
	}

	cap >> img; // get a new frame from camera
	cv::cvtColor(img, frame, CV_BGR2GRAY);

	if(!frame.data){
		std::cout<<"========== No image loaded !!!!!!! ===============";
		return 0;
	}

	cap >> img; // get a new frame from camera
	cv::cvtColor(img, frame, CV_BGR2GRAY);

	if(!stasm_search_single(&foundface, landmarks, (const char*)frame.data, frame.cols, frame.rows, path, "../data")){
		std::cout<<"stasm_search_single failed";
		return 0;
	}

	if (!foundface)
		std::cout<<"=============== No face found !!!!!!! ==============="<<std::endl;

	stasm_force_points_into_image(landmarks,frame.cols, frame.rows);
	//std::vector<cv::Point> features_prev(std::begin(landmarks), std::end(landmarks));
	for(int i = 0; i < stasm_NLANDMARKS * 2; i += 2){
		features_prev.push_back(cv::Point2f(landmarks[i], landmarks[i+1]));
		
	}

	//features_prev.assign(landmarks, landmarks + stasm_NLANDMARKS * 2);

	return 1;
}



void force_points_into_image(std::vector<cv::Point2f> &pointV, int ncols, int nrows){
	for (int i = 0; i < stasm_NLANDMARKS; i++)
    {
		pointV[i].x = MIN(MAX(pointV[i].x, 0.f), float(ncols-1));//MIN(MAX(x, min), max), force x to a value between min and max;
		pointV[i].y = MIN(MAX(pointV[i].y, 0.f), float(nrows-1));
    }
}