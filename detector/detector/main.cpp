#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include "winsw.h"
#include "classifier.h"
#include "Detector.h"

int main(int argc, char** argv) {
	int step = 0;
	int count = 0;

	double features[20][2] = { 0 };
	double labels[] = { 0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 };
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

		if (count >= 10)
		{
			count = 0;
			if (step < 2)
				step++;
			if (step == 2 && !cls)
			{
				cls = new svm_classifier(20, features, labels);
			}
		}
		
		if (step == 0 || step == 1) cv::waitKey(0);

		Position pupilsPosition = d.samplePupilsPosition();

		switch (step)
		{
		case 0:
		case 1:
			std::cout << "step: " << step << " - count: " << count << std::endl;
			features[step * 10 + count][0] = pupilsPosition.x;
			features[step * 10 + count][1] = pupilsPosition.y;
			break;
		case 2:
			int cl = cls->classify(pupilsPosition.x, pupilsPosition.y);
			std::cout << "predict: " <<  cl << std::endl;
			if(displays[cl] != cdisplay)
			{
				cdisplay = displays[cl];
				set_active_display(cdisplay);
			}
			break;
		}

		count++;
		
	}
	return 0;
}
