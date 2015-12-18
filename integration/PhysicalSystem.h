/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef PHYSICALSYSTEM_H_
#define PHYSICALSYSTEM_H_

#include "integration/MathSystem.h"

namespace ElasticMesh {

template<typename Vector, typename Matrix>
class PhysicalSystem {
public:
	virtual ~PhysicalSystem() {}

	// Return the mathematical system being used
	virtual MathSystem<Vector, Matrix>* getMathSystem() = 0;
    // Return number of *positional* degrees of freedom (not velocity)
    virtual int getDOFs() = 0;
    // Write position and velocity into vectors
    virtual void getState(Vector &q, Vector &v) = 0;
    // Read position and velocity from vectors
    virtual void setState(const Vector &q, const Vector &v) = 0;
    // Write mass matrix
    virtual void getInertia(Matrix &M) = 0;
    // Write inverse mass matrix
    virtual void getInverseInertia(Matrix &Mi) = 0;
    // Write forces
    virtual void getForces(Vector &f) = 0;
    // Write Jacobians
    virtual void getJacobians(Matrix &Jx, Matrix &Jv) = 0;
    // Write Differentials
    virtual void getForceDifferentials(Vector &df, Vector& dq) = 0;
};

} /* namespace ElasticMesh */

#endif /* PHYSICALSYSTEM_H_ */
