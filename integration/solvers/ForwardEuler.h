/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef FORWARDEULER_H_
#define FORWARDEULER_H_

#include "Solver.h"

namespace ElasticMesh {

// Forward Euler implementation
template<typename Vector, typename Matrix>
class ForwardEuler : public Solver<Vector, Matrix> {
public:
	ForwardEuler() {}
	virtual ~ForwardEuler() {}

	void step(PhysicalSystem<Vector, Matrix>* system, double dt);
};

template<typename Vector, typename Matrix>
void ForwardEuler<Vector, Matrix>::step(PhysicalSystem<Vector, Matrix>* system, double dt) {

	// Allocate variables
	int n = system->getDOFs();
	Vector& q0 = *(system->getMathSystem()->allocateVector(n));
	Vector& v0 = *(system->getMathSystem()->allocateVector(n));
	Vector& f0 = *(system->getMathSystem()->allocateVector(n));
	Vector& q1 = *(system->getMathSystem()->allocateVector(n));
	Vector& v1 = *(system->getMathSystem()->allocateVector(n));
	Vector& a0 = *(system->getMathSystem()->allocateVector(n));
	Matrix& Mi = *(system->getMathSystem()->allocateMatrix(n, n));

	// Integrate
	system->getState(q0, v0);
	system->getInverseInertia(Mi);
	system->getForces(f0);
	a0 = Mi*f0;
	q1 = v0;
	q1 *= dt;
	q1 += q0;
	v1 = a0;
	v1 *= dt;
	v1 += v0;
	system->setState(q1,v1);

	// deallocate variables
	system->getMathSystem()->freeVector(&q0);
	system->getMathSystem()->freeVector(&v0);
	system->getMathSystem()->freeVector(&f0);
	system->getMathSystem()->freeVector(&q1);
	system->getMathSystem()->freeVector(&v1);
	system->getMathSystem()->freeVector(&a0);
	system->getMathSystem()->freeMatrix(&Mi);
}


} /* namespace ElasticMesh */

#endif /* FORWARDEULER_H_ */
