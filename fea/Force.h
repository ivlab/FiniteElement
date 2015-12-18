/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef FORCE_H_
#define FORCE_H_

#include <Eigen/Dense>
#include <memory>

namespace ElasticMesh {

class Force;
typedef std::shared_ptr<Force> ForceRef;

class Force {
public:
	virtual ~Force() {}

	// adds force computations
    virtual void addForces(Eigen::VectorXd &f) = 0;
    // add jacobieans
    virtual void addJacobians(Eigen::MatrixXd &Jx, Eigen::MatrixXd &Jv) = 0;
    // adds force differentials (used for iterative approach)
    virtual void addDifferentials(Eigen::VectorXd &df, Eigen::VectorXd& dq) = 0;
};

} /* namespace ElasticMesh */

#endif /* FORCE_H_ */
