/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include "integration/PhysicalSystem.h"

namespace ElasticMesh {

// Integration solver
template<typename Vector, typename Matrix>
class Solver {
public:
	virtual ~Solver() {}

	// Integration step
	virtual void step(PhysicalSystem<Vector, Matrix>* system, double dt) = 0;
};

} /* namespace ElasticMesh */

#endif /* SOLVER_H_ */
