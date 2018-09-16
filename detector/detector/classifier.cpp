#include "classifier.h"
#include <cassert>
#include <memory>
#include <algorithm>

svm_classifier::svm_classifier(int count, double features[][2], double labels[])
{
	problem.l = count;

	problem.y = new double[count];
	memcpy(problem.y, labels, sizeof(double) * count);

	problem.x = new svm_node*[count];
	for (int i = 0; i < count; ++i)
	{
		problem.x[i] = new svm_node[3];
		for (int j = 0; j < 2; ++j)
		{
			problem.x[i][j].index = j;
			problem.x[i][j].value = features[i][j];
		}
		problem.x[i][2].index = -1;
	}
	assert(NULL == svm_check_parameter(&problem, &SVM_DEFAULT_PARAM));
	model = svm_train(&problem, &SVM_DEFAULT_PARAM);
}

svm_classifier::~svm_classifier()
{
	svm_free_and_destroy_model(&model);
	delete[] problem.y;
	for (int i = 0; i < problem.l; ++i)
		delete[] problem.x[i];
	delete[] problem.x;
}

double svm_classifier::classify(double x, double y) const
{
	svm_node test[3] = { { 0,x },{ 1,y },{ -1,0 } };
	return svm_predict(model, test);
}
