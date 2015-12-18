/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <fea/forces/AnchorForce.h>
#include "Node.h"

using namespace Eigen;

namespace ElasticMesh {

AnchorForce::AnchorForce(NodeRef node, Eigen::Vector3d x, double ks, double kd) : node(node), x(x), _ks(ks), _kd(kd) {
}

AnchorForce::~AnchorForce() {
}

void AnchorForce::addForces(VectorXd& f) {
    if (node == NULL)
        return;
    Vector3d dx = node->x - x;
    f.segment(3*node->i, 3) += -_ks*dx - _kd*node->v;
}

void AnchorForce::addJacobians(MatrixXd& Jx, MatrixXd& Jv) {
    if (node == NULL)
        return;
    Jx.block(3*node->i,3*node->i, 3,3) += -_ks*Matrix3d::Identity();
    Jv.block(3*node->i,3*node->i, 3,3) += -_kd*Matrix3d::Identity();
}

void AnchorForce::addDifferentials(Eigen::VectorXd& df, Eigen::VectorXd& dq) {
    if (node == NULL)
        return;
	//df.segment(3*node->i, 3) += -_ks*Matrix3d::Identity()*dq.segment(3*node->i, 3);
}

} /* namespace ElasticMesh */
