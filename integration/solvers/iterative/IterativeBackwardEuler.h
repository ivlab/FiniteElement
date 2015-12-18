/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */


/*
 * Algorithm for iterative backward euler was developed from the following source:
 *
 * Eftychios Sifakis , Jernej Barbic, FEM simulation of 3D deformable solids: a practitioner's guide to theory, discretization
 * and model reduction, ACM SIGGRAPH 2012 Courses, p.1-50, August 05-09, 2012, Los Angeles, California
 */

#ifndef SOURCE_DIRECTORY__INTEGRATION_SOLVERS_ITERATIVE_ITERATIVEBACKWARDEULER_H_
#define SOURCE_DIRECTORY__INTEGRATION_SOLVERS_ITERATIVE_ITERATIVEBACKWARDEULER_H_

#include <iostream>
#include "integration/solvers/Solver.h"
#include "integration/solvers/iterative/ConjugateGradientSolver.h"

namespace ElasticMesh {

// Function that takes the force differentials and the Mass for solving a linear system
template<typename Vector, typename Matrix>
class FunctionAw {
public:
	FunctionAw(PhysicalSystem<Vector, Matrix>* system, Matrix& M, int n, double dt, double g) : system(system), M(M), dt(dt), g(g)
	{
		// allocate differentials
		df = system->getMathSystem()->allocateVector(n);
	}

	~FunctionAw()
	{
		// deallocate differentials
		system->getMathSystem()->freeVector(df);
	}

	Vector eval(Vector x)
	{
		// Evaluates left side of the linear system (i.e. A of Ax = b)
		Vector w = -x;
		system->getForceDifferentials(*df, w);
		return (1.0 + g/dt)*(*df) + (1.0/(dt*dt))*M*x;
	}

private:
	PhysicalSystem<Vector, Matrix>* system;
	Matrix& M;
	Vector* df;
	double dt;
	double g;
};


template<typename Vector, typename Matrix>
class IterativeBackwardEuler : public Solver<Vector, Matrix> {
public:
	IterativeBackwardEuler(int iterations, double tolerance, int n, double g) : iterations(n), cg(tolerance, n), g(g) {}
	virtual ~IterativeBackwardEuler() {}

	void step(PhysicalSystem<Vector, Matrix>* system, double dt);

private:
	LinearConjugateGradientSolver<Matrix, FunctionAw<Vector, Matrix>> cg;
	int iterations;
	double g;
};

template<typename Vector, typename Matrix>
void IterativeBackwardEuler<Vector, Matrix>::step(PhysicalSystem<Vector, Matrix>* system, double dt) {
	// Allocate space
	int n = system->getDOFs();
	Vector& q0 = *(system->getMathSystem()->allocateVector(n));
	Vector& v0 = *(system->getMathSystem()->allocateVector(n));
	Matrix& M = *(system->getMathSystem()->allocateMatrix(n, n));
	Vector& f = *(system->getMathSystem()->allocateVector(n));

	// Get state and initialize variables
    system->getState(q0, v0);
    Vector qk = q0;
    Vector vk = v0;
    Vector dx = dt*v0;

    // Get intertia
    system->getInertia(M);

    // Create function for solving Ax=b (left hand side = A)
    FunctionAw<Vector, Matrix> Aw(system, M, n, dt, g);

    // Loop as many times as specified
	for (int k = 0; k < iterations; k++)
	{
		// Get forces
		system->getForces(f);

		// Calculate b
		Vector b = (1.0/dt)*M*(v0-vk) + f;

		// Solve Ax=b
		dx = cg.solve(Aw, b, dx);

		// Integrate
	    qk = qk + dx;
	    vk = vk + (1.0/dt)*dx;

	    // Update state
		system->setState(qk, vk);
	}

	// Deallocate variables
	system->getMathSystem()->freeVector(&q0);
	system->getMathSystem()->freeVector(&v0);
	system->getMathSystem()->freeMatrix(&M);
	system->getMathSystem()->freeVector(&f);
}

} /* namespace ElasticMesh */

#endif /* SOURCE_DIRECTORY__INTEGRATION_SOLVERS_ITERATIVE_ITERATIVEBACKWARDEULER_H_ */
