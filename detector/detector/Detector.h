#pragma once

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

struct Position {
	double x;
	double y;
};

class Detector {
private:
	cv::String faceCascadeFilePath = "haarcascade_frontalface_alt.xml";
	cv::CascadeClassifier faceCascade;
	cv::VideoCapture webcam;
	Position findEyesInFace(cv::Mat frame_gray, cv::Rect faceRegion);
	Position findEyes(cv::Mat frame);

public:
	Detector();
	Position samplePupilsPosition(int frames = 10);
};