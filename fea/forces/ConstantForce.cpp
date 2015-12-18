/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <fea/forces/ConstantForce.h>
#include "Node.h"

using namespace Eigen;

namespace ElasticMesh {

ConstantForce::ConstantForce(FEASystem* system, const Eigen::Vector3d f) : _system(system), _f(f) {
}

ConstantForce::~ConstantForce() {
}

void ConstantForce::addForces(VectorXd& f) {
	const std::vector<NodeRef>& nodes = _system->getNodes();
    for (int n = 0; n < nodes.size(); n++) {
    	NodeRef node = nodes[n];
        f.segment(3*n, 3) += _f*node->m;
    }
}

void ConstantForce::addJacobians(MatrixXd& Jx, MatrixXd& Jv) {
}

void ConstantForce::addDifferentials(Eigen::VectorXd& df, Eigen::VectorXd& dq) {
}

} /* namespace ElasticMesh */
