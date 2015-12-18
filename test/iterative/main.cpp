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
#include "integration/solvers/iterative/ConjugateGradientSolver.h"
#include <Eigen/Dense>

using namespace std;
using namespace ElasticMesh;
using namespace Eigen;

class F {
public:
	F() {}
	~F() {}

	VectorXd eval(VectorXd x)
	{
		MatrixXd A(2,2);
		A(0,0) = 4;
		A(0,1) = 1;
		A(1,0) = 1;
		A(1,1) = 3;

		return A*x;
	}
};

int main () {

	F fun;

	LinearConjugateGradientSolver<VectorXd, F> solver(0.000003, 50);

	VectorXd b(2);
	b[0] = 1;
	b[1] = 2;
	VectorXd x0(2);
	x0[0] = 2;
	x0[1] = 1;
	VectorXd ans = solver.solve(fun, b, x0);

	cout << ans << endl;
}
