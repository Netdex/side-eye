#pragma once
#include "svm.h"

class svm_classifier
{
	static constexpr svm_parameter svm_default_param()
	{
		svm_parameter sp = { 0 };
		sp.svm_type = C_SVC;
		sp.kernel_type = LINEAR;
		sp.cache_size = 256;
		sp.C = 1;
		sp.eps = 0.001;
		sp.shrinking = 1;
		return sp;
	}

	const svm_parameter SVM_DEFAULT_PARAM = svm_default_param();
	svm_model *model;
	svm_problem problem;

public:
	svm_classifier(int count, double features[][2], double labels[]);
	~svm_classifier();

	double classify(double x, double y) const;
};


