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
#include "gpu/fea/FEAInterface.h"

#ifndef USE_CUDA
extern "C"
void testCuda(int v)
{
	std::cout << "Cuda not enabled: " << v << std::endl;
}

extern "C"
void runCuda()
{
	std::cout << "Cuda not enabled" << std::endl;
}

extern "C"
ElasticMesh::FEAInterface* createFEASystem()
{
	std::cout << "Cuda not enable, so cannot create FEA Interface" << std::endl;
	return NULL;
}
#endif
