#include "classifier.h"
#include <iostream>

int main(const int argc, const char *argv[])
{
	double features[][2] = {
		{-10,-10},
		{10,10}
	};
	double labels[] = { 0, 1 };

	svm_classifier sc(2, features, labels);
	std::cout << sc.classify(10, 10) << std::endl;
	return 0;
}
