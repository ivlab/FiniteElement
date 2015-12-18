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
 * Algorithms for St. Venant-Kirchhoff material model were developed from the following source:
 *
 * Eftychios Sifakis , Jernej Barbic, FEM simulation of 3D deformable solids: a practitioner's guide to theory, discretization
 * and model reduction, ACM SIGGRAPH 2012 Courses, p.1-50, August 05-09, 2012, Los Angeles, California
 */

#include <materials/StVenantKirchhoff.h>

using namespace Eigen;

namespace ElasticMesh {

StVenantKirchhoff::StVenantKirchhoff(double u, double lambda) : u(u), lambda(lambda) {
}

StVenantKirchhoff::~StVenantKirchhoff() {
}

Eigen::MatrixXd StVenantKirchhoff::piolaStress(
		const Eigen::MatrixXd& F) {

	MatrixXd I = MatrixXd::Identity(F.rows(), F.rows());
	MatrixXd E = 0.5*(F.transpose()*F-I);

	return F*(2*u*E + lambda*E.trace()*I);
}

Eigen::MatrixXd StVenantKirchhoff::piolaStressDeriv(const Eigen::MatrixXd& F,
		const Eigen::MatrixXd& dF) {

	MatrixXd I = MatrixXd::Identity(F.rows(), F.rows());
	MatrixXd E = 0.5*(F.transpose()*F-I);
	MatrixXd dE = 0.5*(dF.transpose()*F + F.transpose()*dF);

	return dF*(2.0*u*E + lambda*E.trace()*I) + F*(2.0*u*dE + lambda*dE.trace()*I);
}

} /* namespace ElasticMesh */
