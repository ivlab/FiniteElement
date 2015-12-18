/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <solvers/EigenLinearSolver.h>
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>

using namespace Eigen;

namespace ElasticMesh {

EigenLinearSolver::EigenLinearSolver() {

}

EigenLinearSolver::~EigenLinearSolver() {
}

Eigen::VectorXd EigenLinearSolver::solve(Eigen::MatrixXd A, Eigen::VectorXd b) {
	// Use sparce matrix Conjugate Gradient to solve Ax=b
    SparseMatrix<double> spA = A.sparseView();
    ConjugateGradient< SparseMatrix<double> > solver;
    solver.setTolerance(1e-3);
    return solver.compute(spA).solve(b);
}

} /* namespace ElasticMesh */
