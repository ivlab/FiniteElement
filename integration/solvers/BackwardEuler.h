/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef BACKWARDEULER_H_
#define BACKWARDEULER_H_

#include "Solver.h"
#include "LinearSolver.h"

namespace ElasticMesh {

// Backward euler implementation
template<typename Vector, typename Matrix>
class BackwardEuler : public Solver<Vector, Matrix> {
public:
	BackwardEuler(LinearSolver<Vector, Matrix>* solver) : solver(solver) {}
	virtual ~BackwardEuler() {}

	void step(PhysicalSystem<Vector, Matrix>* system, double dt);

private:
	LinearSolver<Vector, Matrix>* solver;
};

template<typename Vector, typename Matrix>
void BackwardEuler<Vector, Matrix>::step(PhysicalSystem<Vector, Matrix>* system, double dt) {
    int n = system->getDOFs();
	Vector& q0 = *(system->getMathSystem()->allocateVector(n));
	Vector& v0 = *(system->getMathSystem()->allocateVector(n));
	Matrix& M = *(system->getMathSystem()->allocateMatrix(n, n));
	Vector& f = *(system->getMathSystem()->allocateVector(n));
	Matrix& Jx = *(system->getMathSystem()->allocateMatrix(n, n));
	Matrix& Jv = *(system->getMathSystem()->allocateMatrix(n, n));

	// Uses jacobians and does a linear solve
    system->getState(q0, v0);
    system->getInertia(M);
    system->getForces(f);
    system->getJacobians(Jx, Jv);
    Matrix A = M - Jx*dt*dt - Jv*dt;
    Vector b = (f + Jx*v0*dt)*dt;
    Vector dv = solver->solve(A, b);
    Vector v1 = v0 + dv;
    system->setState(q0 + v1*dt, v1);

	system->getMathSystem()->freeVector(&q0);
	system->getMathSystem()->freeVector(&v0);
	system->getMathSystem()->freeMatrix(&M);
	system->getMathSystem()->freeVector(&f);
	system->getMathSystem()->freeMatrix(&Jx);
	system->getMathSystem()->freeMatrix(&Jv);
}


} /* namespace ElasticMesh */

#endif /* BACKWARDEULER_H_ */
