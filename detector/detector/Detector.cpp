#include <iostream>
#include <vector>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "constants.h"
#include "findEyeCenter.h"
#include "findEyeCorner.h"
#include "Detector.h"

Detector::Detector() {
	// Load cascade
	if (!faceCascade.load(faceCascadeFilePath)) {
		std::cerr << "Error loading face cascade, check that the file exists." << std::endl;
		exit(-1);
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

	// Initialize
	createCornerKernels();
	webcam = cv::VideoCapture(0);
	releaseCornerKernels();
}

Position Detector::findEyesInFace(cv::Mat frame_gray, cv::Rect faceRegion) {
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

Position Detector::findEyes(cv::Mat frame) {
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

Position Detector::samplePupilsPosition(int frames) {
	Position sample{0.0, 0.0};

	for (int i = 0; i < frames  && webcam.isOpened(); i++) {
		cv::Mat frame;
		webcam.read(frame);
		cv::flip(frame, frame, 1);

		if (frame.empty()) {
			std::cerr << "Captured frame empty!" << std::endl;
			break;
		}

		Position pupilsPosition = findEyes(frame);
		sample.x += pupilsPosition.x;
		sample.y += pupilsPosition.y;

		imshow("Webcam", frame);

		cv::waitKey(25);
	}

	sample.x /= frames;
	sample.y /= frames;

	return sample;
}