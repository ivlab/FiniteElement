/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef CONSTANTFORCE_H_
#define CONSTANTFORCE_H_

#include "fea/FEASystem.h"

namespace ElasticMesh {

// represents a constant force
class ConstantForce : public Force {
public:
	// accepts a directional force
	ConstantForce(FEASystem* system, const Eigen::Vector3d f);
	virtual ~ConstantForce();

	void addForces(Eigen::VectorXd &f);
	void addJacobians(Eigen::MatrixXd &Jx, Eigen::MatrixXd &Jv);
	void addDifferentials(Eigen::VectorXd &df, Eigen::VectorXd& dq);

private:
	FEASystem* _system;
	Eigen::Vector3d _f;
};

} /* namespace ElasticMesh */

#endif /* CONSTANTFORCE_H_ */
