#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include "winsw.h"
#include "classifier.h"
#include "Detector.h"

int main(int argc, char** argv) {
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
	Detector d;
	svm_classifier *cls = nullptr;
	while (true) {

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

		Position pupilsPosition = d.samplePupilsPosition();

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

		count++;
		
	}
	return 0;
}
