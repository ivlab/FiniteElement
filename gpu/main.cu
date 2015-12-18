/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <iostream>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/generate.h>
#include <thrust/reduce.h>
#include <thrust/functional.h>
#include <algorithm>
#include <cstdlib>
#include "math/GpuMath.cuh"
#include "fea/GpuFEASystem.cuh"
#include "integration/GpuIterativeBackwardEuler.cuh"
#include "gpu/fea/FEAInterface.h"
#include "gpu/fea/GpuFEASystem.cuh"

using namespace ElasticMesh;

extern "C"
void testCuda(int v)
{
	std::cout << "Using cuda: " << v << std::endl;
}

extern "C"
void runCuda()
{
}

// Entry into cuda code
extern "C"
ElasticMesh::FEAInterface* createFEASystem()
{
	std::cout << "Using cuda" << std::endl;
	
	// returns new gpu FEA system if cuda is enabled
	return new GpuFEASystem();
}