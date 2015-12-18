/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef SOURCE_DIRECTORY__GPU_FEA_FEAINTERFACE_H_
#define SOURCE_DIRECTORY__GPU_FEA_FEAINTERFACE_H_

namespace ElasticMesh {
class FEAInterface;
}

extern "C"
ElasticMesh::FEAInterface* createFEASystem();

namespace ElasticMesh {

// Interface into GPU FEA implementation
class FEAInterface {
public:
	virtual ~FEAInterface() {}

	// initializes GPU FEA System
	virtual void init() = 0;
	// sets nodes
	virtual void setNodes(int num, double* x, double* v, double* m) = 0;
	// sets elements
	virtual void setElements(int num, int* indexes) = 0;
	// gets node positions
	virtual double* getNodePositions() = 0;
	// integrates system
	virtual void step(double timestep) = 0;

	// creates a GPU FEA system
	static FEAInterface* create()
	{
		return createFEASystem();
	}
};

} /* namespace ElasticMesh */


#endif /* SOURCE_DIRECTORY__GPU_FEA_FEAINTERFACE_H_ */
