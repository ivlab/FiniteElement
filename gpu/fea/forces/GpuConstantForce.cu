/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <fea/forces/GpuConstantForce.cuh>

namespace ElasticMesh {

GpuConstantForce::GpuConstantForce(GpuFEASystem* system, double x, double y, double z) : system(system), x(x), y(y), z(z) {
}

GpuConstantForce::~GpuConstantForce() {
}

void GpuConstantForce::addForces(VectorXd& f) {
	// TODO: Implement method
//	VectorXd temp(f.v.size());
//	thrust::fill(temp.v.begin(), temp.v.end(), -9.8);
//	thrust::transform(temp.v.begin(), temp.v.end(), system->M.mat.begin(), temp.v.begin(), thrust::multiplies<double>());
//	f += temp;
}

} /* namespace ElasticMesh */
