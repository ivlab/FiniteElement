/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */


#ifndef SOURCE_DIRECTORY__GPU_INTEGRATION_GPUITERATIVEBACKWARDEULER_CUH_
#define SOURCE_DIRECTORY__GPU_INTEGRATION_GPUITERATIVEBACKWARDEULER_CUH_

#include "integration/solvers/Solver.h"
#include "math/GpuMath.cuh"
#include <thrust/for_each.h>
#include <thrust/device_vector.h>
#include <thrust/iterator/zip_iterator.h>

namespace ElasticMesh {

class GpuIterativeBackwardEuler : public Solver<VectorXd, MatrixXd> {
public:
	GpuIterativeBackwardEuler(int iterations);
	virtual ~GpuIterativeBackwardEuler();

	// performs integration step
	void step(PhysicalSystem<VectorXd, MatrixXd>* system, double dt);

private:
	// allocates gpu memory
	void allocateArrays(PhysicalSystem<VectorXd, MatrixXd>* system, int size);

	int iterations;
	int currentSize;
	
	// gpu vectors
	VectorXd q0;
	VectorXd v0;
	MatrixXd M;
	VectorXd f;
	VectorXd qk;
	VectorXd vk;
	VectorXd dx;
	VectorXd df;
	VectorXd b;
};

} /* namespace ElasticMesh */

#endif /* SOURCE_DIRECTORY__GPU_INTEGRATION_GPUITERATIVEBACKWARDEULER_CUH_ */
