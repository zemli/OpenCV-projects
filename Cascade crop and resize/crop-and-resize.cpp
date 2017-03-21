#include <opencv2/opencv.hpp>
#include<sstream>
#include<string>


using namespace std;
using namespace cv;

Mat detectAndCrop(Mat frame);

String xmlPath = "C:/opencv/sources/data/haarcascades/";
String face_cascade_name = "haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;


int main(int, char**)
{
	String imgPath = "E:/image/*.png";
	vector<String> filenames;
	glob(imgPath, filenames);

	Mat myImage, cropped;

	for (size_t i = 0; i < filenames.size(); ++i)
	{
		myImage = imread(filenames[i]);
		imshow("original", myImage);
		//crop image
		if (!face_cascade.load(xmlPath + face_cascade_name)){ printf("--(!)Error loading\n"); return -1; };
		cropped = detectAndCrop(myImage);

		stringstream ss;
		ss << filenames[i].substr(0, 9) << "cropped-" << filenames[i].substr(9);
		string filename = ss.str();
		imwrite(filename, cropped);
		imshow("cropped", cropped);
		int c = cv::waitKey(1);
	}

	return 0;
}

Mat detectAndCrop(Mat frame)
{
	std::vector<Rect> faces;
	Mat frame_gray;

	if (frame.channels() > 1)
		cvtColor(frame, frame_gray, CV_BGR2GRAY);
	else frame_gray = frame;
	//equalize histogram
	//equalizeHist(frame_gray, frame_gray);
	imshow("gray", frame_gray);
	//detect face
	face_cascade.detectMultiScale(frame_gray, faces, 1, 3, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	//crop image
	Rect roi(faces[0].x, faces[0].y, faces[0].width, faces[0].width);
	Mat image_roi = frame_gray(roi);
	//resize
	Size size(224, 224);
	resize(image_roi, image_roi, size);
	return image_roi;
}
