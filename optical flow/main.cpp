#include "../VisualStudio/main.h"






int main(int argc, char** argv){

	if(!init()){
		std::cout << "error in initialization" << std::endl;
		std::cin.get();// pause the console
		return -1;
	}
	
	try{

		while(true){
			prev_frame = frame.clone();
			cap >> img; // get a new frame from camera
			cv::cvtColor(img, frame, CV_BGR2GRAY);

			cv::calcOpticalFlowPyrLK(prev_frame, frame, features_prev, features_next, status, err);
			//std::copy(features_next.begin(), features_next.end(), landmarks);

			/*=============================================== show video ==============================================================*/
			force_points_into_image(features_next, frame.cols, frame.rows);
			for(int i = 0; i < stasm_NLANDMARKS; i++)
				frame(features_next[i]) = 255;

			//cv::flip(frame, output, 1);//mirror output
			cv::imshow("camera", frame);
			if(cv::waitKey(1) >= 0) break;
		}//while
	}//try
	catch( cv::Exception & e){
		std::cerr << e.msg << std::endl;
		std::cin.get();// pause the console
	}

	return 0;
}