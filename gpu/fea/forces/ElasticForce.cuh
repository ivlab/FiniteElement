/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef SOURCE_DIRECTORY__GPU_FEA_ELASTICFORCE_CUH_
#define SOURCE_DIRECTORY__GPU_FEA_ELASTICFORCE_CUH_

#include "math/GpuMath.cuh"
#include "fea/GpuFEASystem.cuh"
#include "fea/GpuForce.cuh"

namespace ElasticMesh {

class ElasticForce : public GpuForce {
public:
	ElasticForce(GpuFEASystem* system, int n, int* inds);
	virtual ~ElasticForce();
	
	void init();
	void addForces(VectorXd &f);

private:

	GpuFEASystem* system;
	VectorXi indices;
	int size;
};

} /* namespace ElasticMesh */

#endif /* SOURCE_DIRECTORY__GPU_FEA_ELASTICFORCE_CUH_ */
