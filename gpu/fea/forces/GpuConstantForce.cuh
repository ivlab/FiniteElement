/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef SOURCE_DIRECTORY__GPU_FEA_FORCES_GPUCONSTANTFORCE_CUH_
#define SOURCE_DIRECTORY__GPU_FEA_FORCES_GPUCONSTANTFORCE_CUH_

#include "fea/GpuFEASystem.cuh"
#include "fea/GpuForce.cuh"

namespace ElasticMesh {

// represents a constant force
class GpuConstantForce : public GpuForce {
public:
	GpuConstantForce(GpuFEASystem* system, double x, double y, double z);
	virtual ~GpuConstantForce();

	void addForces(VectorXd &f);

private:
	GpuFEASystem* system;
	double x, y, z;
};

} /* namespace ElasticMesh */

#endif /* SOURCE_DIRECTORY__GPU_FEA_FORCES_GPUCONSTANTFORCE_CUH_ */
