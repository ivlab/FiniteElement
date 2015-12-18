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
 * Algorithms for calculating forces and force differentials developed from the following source:
 *
 * Eftychios Sifakis , Jernej Barbic, FEM simulation of 3D deformable solids: a practitioner's guide to theory, discretization
 * and model reduction, ACM SIGGRAPH 2012 Courses, p.1-50, August 05-09, 2012, Los Angeles, California
 */

#include <fea/elements/Tetrahedral.h>
#include <iostream>

using namespace Eigen;

namespace ElasticMesh {

Tetrahedral::Tetrahedral(MaterialModelRef materialModel, NodeRef i, NodeRef j, NodeRef k, NodeRef l) : Element(materialModel) {
	_nodes[0] = i;
	_nodes[1] = j;
	_nodes[2] = k;
	_nodes[3] = l;
}

Tetrahedral::~Tetrahedral() {
}

void Tetrahedral::init() {
	// create rest state for Tetrahedral
	Matrix3d Dm;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Dm(i,j) = _nodes[j]->x[i] - _nodes[3]->x[i];
		}
	}

	// sets the rest inverse
	_B = Dm.inverse();

	// calculates volume of Tetrahedral
	_w = (1.0/6.0)*fabs(Dm.determinant());
}


void Tetrahedral::addForces(VectorXd& f) {
	// calculate deformation
	Matrix3d Ds;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Ds(i,j) = _nodes[j]->x[i] - _nodes[3]->x[i];
		}
	}

	// calculate deformation gradient
	Matrix3d F = Ds*_B;
	// calculate stress based on the force
	Matrix3d P = getModel()->piolaStress(F);
	// calculate forces
	Matrix3d H = -_w*P*_B.transpose();

	// add forces
	for (int i = 0; i < 3; i++)
	{
		Vector3d force = H.col(i);
		f.segment(3*_nodes[i]->i, 3) += force;

		f.segment(3*_nodes[3]->i, 3) += -force;
	}
}

void Tetrahedral::addJacobians(MatrixXd& Jx, MatrixXd& Jv) {
	// TODO: Implement method
}

void Tetrahedral::addDifferentials(Eigen::VectorXd& df, Eigen::VectorXd& dq) {
	// calculate deformed shape
	Matrix3d Ds;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Ds(i,j) = _nodes[j]->x[i] - _nodes[3]->x[i];
		}
	}

	// calculate deformation differentials
	Matrix3d dDs;
	for (int i = 0; i < 3; i++)
	{
		dDs.col(i) = df.segment(3*_nodes[i]->i, 3) - df.segment(3*_nodes[3]->i, 3); //dx[j][i] - dx[3][i];
	}

	// calculate deformation gradient
	Matrix3d F = Ds*_B;
	// calculate deformation differential gradient
	Matrix3d dF = dDs*_B;
	// calculate stress based on deformation gradient and deformation differentials
	Matrix3d dP = getModel()->piolaStressDeriv(F, dF);
	// calculate forces
	Matrix3d dH = -_w*dP*_B.transpose();

	// add forces
	for (int i = 0; i < 3; i++)
	{
		df.segment(3*_nodes[i]->i, 3) += dH.col(i);
		df.segment(3*_nodes[3]->i, 3) += -dH.col(i);
	}

}

} /* namespace ElasticMesh */

