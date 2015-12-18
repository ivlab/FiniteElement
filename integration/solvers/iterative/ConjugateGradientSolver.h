/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef SOURCE_DIRECTORY__INTEGRATION_SOLVERS_ITERATIVE_CONJUGATEGRADIENTSOLVER_H_
#define SOURCE_DIRECTORY__INTEGRATION_SOLVERS_ITERATIVE_CONJUGATEGRADIENTSOLVER_H_

#include <iostream>

namespace ElasticMesh {

// Function has f.eval(Vector x) method
template<typename Vector, typename Function>
class LinearConjugateGradientSolver {
public:
	LinearConjugateGradientSolver(double tolerance, int n) : tolerance(tolerance), n(n) {}
	~LinearConjugateGradientSolver() {}

	// Solves f(x)*x0=b
	Vector solve(Function &f, Vector b, Vector x0);

private:
	double tolerance;
	int n;
};

template<typename Vector, typename Function>
Vector LinearConjugateGradientSolver<Vector, Function>::solve(Function &f, Vector b, Vector x0)
{
	// Conjugate gradient solver

	Vector r = b-f.eval(x0);
	Vector p = r;

	Vector x = x0;

	for (int k = 0; k < n; k++)
	{
		Vector Ap = f.eval(p);
		double rTr = (r.transpose()*r)(0);
		double pTAp = (p.transpose()*Ap)(0);
		double alpha = abs(pTAp) > 0.000001 ? rTr/pTAp : 0.0;
		x = x + alpha*p;
		r = r - alpha*Ap;

		if (r.norm() < tolerance)
		{
			break;
		}

		double B = (r.transpose()*r)(0);
		B = B/rTr;

		p = r + B*p;
	}

	return x;
}

} /* namespace ElasticMesh */

#endif /* SOURCE_DIRECTORY__INTEGRATION_SOLVERS_ITERATIVE_CONJUGATEGRADIENTSOLVER_H_ */
