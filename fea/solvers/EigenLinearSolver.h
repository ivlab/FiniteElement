/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef EIGENLINEARSOLVER_H_
#define EIGENLINEARSOLVER_H_

#define EIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET
#include "integration/solvers/LinearSolver.h"
#include <Eigen/Dense>

namespace ElasticMesh {

class EigenLinearSolver : public LinearSolver<Eigen::VectorXd, Eigen::MatrixXd> {
public:
	EigenLinearSolver();
	virtual ~EigenLinearSolver();

	// solves linear systems using Eigen
	Eigen::VectorXd solve(Eigen::MatrixXd A, Eigen::VectorXd b);
};

} /* namespace ElasticMesh */

#endif /* EIGENLINEARSOLVER_H_ */
