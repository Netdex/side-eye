#include <iostream>
#include "Detector.h"

int main(int argc, char** argv) {
	Detector d;
	while (true) {
		Position p = d.samplePupilsPosition(10);
		std::cout << p.x << ", " << p.y << std::endl;
	}
	return 0;
}