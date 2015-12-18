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


#include <fea/elements/Triangle.h>
#include <iostream>
#include <cmath>

using namespace Eigen;

namespace ElasticMesh {

Triangle::Triangle(MaterialModelRef materialModel, NodeRef i, NodeRef j, NodeRef k) : Element(materialModel) {
	_nodes[0] = i;
	_nodes[1] = j;
	_nodes[2] = k;
}

Triangle::~Triangle() {
}

void Triangle::init() {
	// calculate rest shape
	Matrix2d Dm;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			Dm(i,j) = _nodes[j]->x[i] - _nodes[2]->x[i];
		}
	}

	// calculate reset inverse
	_B = Dm.inverse();

	// calculate triangle area
	Vector3d cp = _nodes[0]->x.cross(_nodes[1]->x);
	_w = 0.5*sqrt(cp.dot(cp));
}


void Triangle::addForces(VectorXd& f) {
	// calculate deformed shape
	Matrix2d Ds;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			Ds(i,j) = _nodes[j]->x[i] - _nodes[2]->x[i];
		}
	}

	// calculate deformation gradient
	Matrix2d F = Ds*_B;
	// calculate stres
	Matrix2d P = getModel()->piolaStress(F);
	// calculate forces
	Matrix2d H = -_w*P*_B.transpose();

	// add forces
	for (int i = 0; i < 2; i++)
	{
		Vector2d force = H.col(i);
		f.segment(3*_nodes[i]->i, 2) += force;

		f.segment(3*_nodes[2]->i, 2) += -force;
	}
}

void Triangle::addJacobians(MatrixXd& Jx, MatrixXd& Jv) {
}

void Triangle::addDifferentials(Eigen::VectorXd& df, Eigen::VectorXd& dq) {
}

} /* namespace ElasticMesh */

