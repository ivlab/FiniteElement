/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef SOURCE_DIRECTORY__GPU_FEA_GPUFEASYSTEM_H_
#define SOURCE_DIRECTORY__GPU_FEA_GPUFEASYSTEM_H_

#include "math/GpuMath.cuh"
#include "integration/PhysicalSystem.h"
#include "fea/FEAInterface.h"
#include "integration/GpuIterativeBackwardEuler.cuh"
#include "fea/GpuForce.cuh"
#include <vector>

namespace ElasticMesh {

class GpuFEASystem : public PhysicalSystem<VectorXd, MatrixXd>, public FEAInterface {
public:
	GpuFEASystem();
	virtual ~GpuFEASystem();
	
	// Physical System interface
	MathSystem<VectorXd, MatrixXd>* getMathSystem() { return NULL; }
    int getDOFs();
    void getState(VectorXd &q, VectorXd &v);
    void setState(const VectorXd &q, const VectorXd &v);
    void getInertia(MatrixXd &M);
    void getInverseInertia(MatrixXd &Mi) {}
    void getForces(VectorXd &f);
    void getJacobians(MatrixXd &Jx, MatrixXd &Jv) {}
    void getForceDifferentials(VectorXd &df, VectorXd& dq);
    
    // FEAInterface interface
    void init();
	void setNodes(int num, double* x, double* v, double* m);
	void setElements(int num, int* indexes);
	double* getNodePositions();
	void step(double timestep);
	
	// Position, velocity, and mass vectors
	VectorXd X;
	VectorXd V;
	MatrixXd M;
	
private:
	int numNodes;
	double* x;
	double* v;
	double* m;
	int numElements;
	int* indices;
	GpuIterativeBackwardEuler solver;
	std::vector<GpuForce*> forces;
};

} /* namespace ElasticMesh */

#endif /* SOURCE_DIRECTORY__GPU_FEA_GPUFEASYSTEM_H_ */
