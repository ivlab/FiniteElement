/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef LINEARSOLVER_H_
#define LINEARSOLVER_H_

namespace ElasticMesh {

// Linear solver for allowing implementations of linear systems
template<typename Vector, typename Matrix>
class LinearSolver
{
public:
	virtual ~LinearSolver() {}

	// Solves Ax=b
	virtual Vector solve(Matrix A, Vector b) = 0;
};

} /* namespace ElasticMesh */

#endif /* LINEARSOLVER2_H_ */
