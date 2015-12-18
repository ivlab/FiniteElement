/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <fea/GpuFEASystem.cuh>
#include <thrust/fill.h>
#include <thrust/device_vector.h>
#include <thrust/execution_policy.h>
#include "fea/forces/GpuConstantForce.cuh"
#include "fea/forces/ElasticForce.cuh"

namespace ElasticMesh {

GpuFEASystem::GpuFEASystem() : numNodes(0), solver(1), X(0), V(0), M(0,0) {

}

GpuFEASystem::~GpuFEASystem() {
	delete[] x;
	delete[] v;
	delete[] m;
	delete[] indices;
}

int GpuFEASystem::getDOFs()
{
	return 3*numNodes;
}

void GpuFEASystem::getState(VectorXd &q, VectorXd &v)
{
	// copies state arrays to gpu memory
	thrust::copy(x, x+numNodes*3, q.v.begin());
	thrust::copy(this->v, this->v+numNodes*3, v.v.begin());
}


void GpuFEASystem::setState(const VectorXd &q, const VectorXd &v)
{
	// copies gpu memory to cpu
	thrust::copy(q.v.begin(), q.v.end(), x);
	
	// maintains own GPU array
	X = q;
	V = v;
}


void GpuFEASystem::getInertia(MatrixXd &M)
{
	// copy mass array to GPU
	thrust::copy(m, m+numNodes*3, M.mat.begin());
}

void GpuFEASystem::getForces(VectorXd &f)
{
	// initializes gpu force memory to 0.0
	thrust::fill(thrust::device, f.v.begin(), f.v.end(), 0.0);

	// loop through forces
	for (int i = 0; i < forces.size(); i++)
	{
		forces[i]->addForces(f);
	}
}


void GpuFEASystem::getForceDifferentials(VectorXd &df, VectorXd& dq)
{
	// initializes deformation gpu memory to 0
	thrust::fill(thrust::device, df.v.begin(), df.v.end(), 0.0);
}

void GpuFEASystem::init()
{
	// creates state vectors on gpu
	X = VectorXd(numNodes*3);
	V = VectorXd(numNodes*3);
	M = MatrixXd(numNodes*3,1);
	getState(X, V);
	getInertia(M);
	
	// create gpu forces (adds elements)
	forces.push_back(new ElasticForce(this, numElements, indices));
}

void GpuFEASystem::setNodes(int num, double* x, double* v, double* m)
{
	// sets nodes that are passed in from interface
	numNodes = num;
	this->x = x;
	this->v = v;
	this->m = m;
}


void GpuFEASystem::setElements(int num, int* indexes)
{
	// sets elements that are passed in from interface
	numElements = num;
	indices = indexes;
}


double* GpuFEASystem::getNodePositions()
{
	// returns cpu node positions
	return x;
}

void GpuFEASystem::step(double timestep)
{
	// calls gpu integrator
	solver.step(this, timestep);
}


} /* namespace ElasticMesh */
