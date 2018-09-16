#include <iostream>
#include <vector>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "constants.h"
#include "findEyeCenter.h"
#include "findEyeCorner.h"
#include "classifier.h"
#include <opencv2/imgproc/imgproc.hpp>
#include "winsw.h"

cv::String faceCascadeFilePath = "haarcascade_frontalface_alt.xml";
cv::CascadeClassifier faceCascade;

struct Position {
	double x;
	double y;
};

Position findEyesInFace(cv::Mat frame_gray, cv::Rect faceRegion) {
	// Crop face
	cv::Mat face = frame_gray(faceRegion);

	// Find eye regions
	int eye_region_width = faceRegion.width * (kEyePercentWidth / 100.0);
	int eye_region_height = faceRegion.height * (kEyePercentHeight / 100.0);
	int eye_region_top = faceRegion.height * (kEyePercentTop / 100.0);
	cv::Rect leftEyeRegion(
		faceRegion.width * (kEyePercentSide / 100.0),
		eye_region_top,
		eye_region_width,
		eye_region_height
	);
	cv::Rect rightEyeRegion(
		faceRegion.width - faceRegion.width * (kEyePercentSide / 100.0) - eye_region_width,
		eye_region_top,
		eye_region_width,
		eye_region_height
	);

	// Find pupils (relative to eye region)
	cv::Point leftPupil = findEyeCenter(face, leftEyeRegion, "Left Eye");
	cv::Point rightPupil = findEyeCenter(face, rightEyeRegion, "Right Eye");

	// Draw detected features
	circle(face, cv::Point(leftPupil.x + leftEyeRegion.x, leftPupil.y + leftEyeRegion.y), 3, cv::Scalar(255, 255, 255));
	circle(face, cv::Point(rightPupil.x + rightEyeRegion.x, rightPupil.y + rightEyeRegion.y), 3, cv::Scalar(255, 255, 255));
	imshow("Processed", face);

	// Linearly normalize pupils' positions to [-1, 1]
	Position leftPupilNormalizedPosition{
		2.0 * leftPupil.x / leftEyeRegion.width - 1,
		2.0 * leftPupil.y / leftEyeRegion.height - 1
	};
	Position rightPupilNormalizedPosition{
		2.0 * rightPupil.x / rightEyeRegion.width - 1,
		2.0 * rightPupil.y / rightEyeRegion.height - 1
	};

	// TODO: maybe averaging them is not the best idea?
	// TODO: maybe a non-linear scale is better?
	return Position{
		(leftPupilNormalizedPosition.x + rightPupilNormalizedPosition.x) / 2,
		(leftPupilNormalizedPosition.y + rightPupilNormalizedPosition.y) / 2
	};
}

Position findEyes(cv::Mat frame) {
	// Convert to frame to grayscale
	cv::Mat frameGrayscale;
	cv::cvtColor(frame, frameGrayscale, CV_BGR2GRAY);

	// Detect faces
	std::vector<cv::Rect> faceRegions;
	faceCascade.detectMultiScale(frameGrayscale, faceRegions, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE | CV_HAAR_FIND_BIGGEST_OBJECT, cv::Size(150, 150));
	for (int i = 0; i < faceRegions.size(); i++) {
		rectangle(frame, faceRegions[i], cv::Scalar(255, 0, 0));
	}

	// Find eyes in the first face
	if (faceRegions.size() > 0) {
		return findEyesInFace(frameGrayscale, faceRegions[0]);
	}

	// No faces found
	return Position{ 0.0, 0.0 };
}

int main(int argc, const char** argv) {
	// Load cascade
	if (!faceCascade.load(faceCascadeFilePath)) {
		std::cerr << "Error loading face cascade, check that the file exists." << std::endl;
		return -1;
	};

	// Create and position windows
	cv::namedWindow("Webcam", CV_WINDOW_NORMAL);
	cv::moveWindow("Webcam", 0, 0);
	cv::namedWindow("Processed", CV_WINDOW_NORMAL);
	cv::moveWindow("Processed", 0, 400);
	cv::namedWindow("Left Eye", CV_WINDOW_NORMAL);
	cv::moveWindow("Left Eye", 400, 0);
	cv::namedWindow("Right Eye", CV_WINDOW_NORMAL);
	cv::moveWindow("Right Eye", 800, 0);

	int step = 0;
	int count = 0;

	double features[10][2] = { 0 };
	double labels[] = { 0,0,0,0,0,1,1,1,1,1 };
	std::vector<int> displays = query_display_ids();
	std::cout << "detect " << displays.size() << " displays" << std::endl;
	int cdisplay = displays[0];
	for(auto i : displays)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;

	// Start capturing
	createCornerKernels();
	cv::VideoCapture capture(0);
	svm_classifier *cls = nullptr;
	while (capture.isOpened()) {
		cv::Mat frame;
		capture.read(frame);
		cv::flip(frame, frame, 1);

		if (frame.empty()) {
			std::cerr << "Captured frame empty!" << std::endl;
			break;
		}

		if (count >= 5)
		{
			count = 0;
			if (step < 2)
				step++;
			if (step == 2 && !cls)
			{
				cls = new svm_classifier(10, features, labels);
			}
		}

		Position pupilsPosition = findEyes(frame);

		switch (step)
		{
		case 0:
		case 1:
			std::cout << "step: " << step << " - count: " << count << std::endl;
			features[step * 5 + count][0] = pupilsPosition.x;
			features[step * 5 + count][1] = pupilsPosition.y;
			cv::waitKey(0);
			break;
		case 2:
			int cl = cls->classify(pupilsPosition.x, pupilsPosition.y);
			std::cout << "predict: " <<  cl << std::endl;
			if(displays[cl] != cdisplay)
			{
				cdisplay = displays[cl];
				set_active_display(cdisplay);
			}
			cv::waitKey(50);
			break;
		}

		imshow("Webcam", frame);

		count++;

		
	}
	releaseCornerKernels();

	return 0;
}
