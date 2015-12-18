/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */



#include <fea/forces/ElasticForce.cuh>

namespace ElasticMesh {

ElasticForce::ElasticForce(GpuFEASystem* system, int n, int* inds) : indices(n*4), size(n) {
	thrust::copy(inds, inds+size*4, indices.v.begin());
}

ElasticForce::~ElasticForce() {
}

void ElasticForce::init() {
	// TODO: Implement elastic pre-compute method
	
	//float* pd_vec = thrust::raw_pointer_cast(d_vec.data());
	//kernel<<<100,1>>>(pd_vec);
}

void ElasticForce::addForces(VectorXd& f) {
	// TODO: Implement elastic calculate forces method
	// TODO: Implement gpu material model
	
	// TODO: Reduce forces from tetrahedrals
}

// TODO: add calculate of differentials

} /* namespace ElasticMesh */
