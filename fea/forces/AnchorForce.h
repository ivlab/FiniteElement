/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef ANCHORFORCE_H_
#define ANCHORFORCE_H_

#include "fea/FEASystem.h"
#include <Eigen/Dense>

namespace ElasticMesh {

// applies a spring force between an anchor and a node
class AnchorForce : public Force {
public:
	AnchorForce(NodeRef node, Eigen::Vector3d x, double ks, double kd);
	virtual ~AnchorForce();

	void addForces(Eigen::VectorXd &f);
	void addJacobians(Eigen::MatrixXd &Jx, Eigen::MatrixXd &Jv);
	void addDifferentials(Eigen::VectorXd &df, Eigen::VectorXd& dq);

	Eigen::Vector3d x;
	NodeRef node;

private:
	double _ks, _kd;
};

} /* namespace ElasticMesh */

#endif /* ANCHORFORCE_H_ */
