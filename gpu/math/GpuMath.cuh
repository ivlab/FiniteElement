/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */



#ifndef SOURCE_DIRECTORY__GPU_MATH_GPUMATH_CUH_
#define SOURCE_DIRECTORY__GPU_MATH_GPUMATH_CUH_

#include <iostream>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/generate.h>
#include <thrust/reduce.h>
#include <thrust/functional.h>
#include <algorithm>
#include <cstdlib>

namespace ElasticMesh {

// Simple abstraction for gpu double vector
struct VectorXd
{
	VectorXd(int n) : v(n) {}
	VectorXd(const VectorXd& vec)
	{
		v = vec.v;
	}
	
	~VectorXd() {}

	thrust::device_vector<double> v;
	
	double* asArray()
	{
		return thrust::raw_pointer_cast(&v[0]);
	}
	
	void operator+=(const VectorXd& vec)
	{
		thrust::transform(vec.v.begin(), vec.v.end(), v.begin(), v.begin(), thrust::plus<double>());
	}
	
	void operator*=(const VectorXd& vec)
	{
		thrust::transform(vec.v.begin(), vec.v.end(), v.begin(), v.begin(), thrust::multiplies<double>());
	}
	
	void operator/=(const VectorXd& vec)
	{
		thrust::transform(vec.v.begin(), vec.v.end(), v.begin(), v.begin(), thrust::divides<double>());
	}
};

// Simple abstraction for gpu int vector
struct VectorXi
{
	VectorXi(int n) : v(n) {}
	VectorXi(const VectorXi& vec)
	{
		v = vec.v;
	}
	
	~VectorXi() {}

	thrust::device_vector<int> v;
	
	int* asArray()
	{
		return thrust::raw_pointer_cast(&v[0]);
	}
};

// Simple abstraction for a vector of gpu double matrixes
struct MatrixXd
{
	MatrixXd(int n, int m) : mat(n*m) {}
	MatrixXd(const MatrixXd& m)
	{
		mat = m.mat;
	}
	~MatrixXd() {}

	thrust::device_vector<double> mat;
	
	double* asArray()
	{
		return thrust::raw_pointer_cast(&mat[0]);
	}
};

} /* namespace ElasticMesh */

#endif /* SOURCE_DIRECTORY__GPU_MATH_GPUMATH_CUH_ */
