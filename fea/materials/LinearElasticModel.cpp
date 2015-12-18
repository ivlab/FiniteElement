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
 * Algorithms for linear elasticity material model were developed from the following source:
 *
 * Eftychios Sifakis , Jernej Barbic, FEM simulation of 3D deformable solids: a practitioner's guide to theory, discretization
 * and model reduction, ACM SIGGRAPH 2012 Courses, p.1-50, August 05-09, 2012, Los Angeles, California
 */

#include <materials/LinearElasticModel.h>
#include <iostream>

using namespace Eigen;

namespace ElasticMesh {

LinearElasticModel::LinearElasticModel(double u, double lambda) : u(u), lambda(lambda) {
}

LinearElasticModel::~LinearElasticModel() {
}

Eigen::MatrixXd LinearElasticModel::piolaStress(
		const Eigen::MatrixXd& F) {

	MatrixXd I = MatrixXd::Identity(F.rows(), F.rows());
	return u*(F + F.transpose() - 2.0*I) + lambda*(F-I).trace()*I;
}

Eigen::MatrixXd LinearElasticModel::piolaStressDeriv(const Eigen::MatrixXd& F,
		const Eigen::MatrixXd& dF) {
	// Todo: Implement method
	return MatrixXd::Identity(F.rows(), F.rows());
}

} /* namespace ElasticMesh */
