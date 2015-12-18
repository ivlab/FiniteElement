/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <fea/FEASystem.h>

using namespace Eigen;

namespace ElasticMesh {

FEASystem::FEASystem() {
}

FEASystem::~FEASystem() {
}

MathSystem<Eigen::VectorXd, Eigen::MatrixXd>* FEASystem::getMathSystem() {
	return &_mathSystem;
}

void FEASystem::init() {
	for (int f = 0; f < _elements.size(); f++)
	{
		_elements[f]->init();
	}
}

int FEASystem::getDOFs() {
	return 3*_nodes.size();
}

void FEASystem::getState(Eigen::VectorXd& q, Eigen::VectorXd& v) {
    for (int n = 0; n < _nodes.size(); n++) {
    	NodeRef node = _nodes[n];
        q.segment(n*3, 3) = node->x;
        v.segment(n*3, 3) = node->v;
    }
}

void FEASystem::setState(const Eigen::VectorXd& q,
		const Eigen::VectorXd& v) {
    for (int n = 0; n < _nodes.size(); n++) {
    	NodeRef node = _nodes[n];
        node->x = q.segment(n*3, 3);
        node->v = v.segment(n*3, 3);
    }
}

void FEASystem::getInertia(Eigen::MatrixXd& M) {
    M.setZero();
    for (int n = 0; n < _nodes.size(); n++)
        M.block(n*3,n*3, 3,3) = _nodes[n]->m*MatrixXd::Identity(3,3);
}

void FEASystem::getInverseInertia(Eigen::MatrixXd& Mi) {
    Mi.setZero();
    for (int n = 0; n < _nodes.size(); n++)
        Mi.block(n*3,n*3, 3,3) = 1.0/_nodes[n]->m*MatrixXd::Identity(3,3);
}

void FEASystem::getForces(Eigen::VectorXd& f) {
    f.setZero();
    for (int i = 0; i < _forces.size(); i++)
    {
        _forces[i]->addForces(f);
    }
}

void FEASystem::getJacobians(Eigen::MatrixXd& Jx,
		Eigen::MatrixXd& Jv) {
    Jx.setZero();
    Jv.setZero();
    for (int i = 0; i < _forces.size(); i++)
    {
        _forces[i]->addJacobians(Jx, Jv);
    }
}

void FEASystem::getForceDifferentials(Eigen::VectorXd& df, Eigen::VectorXd& dq) {
	df.setZero();
    for (int i = 0; i < _forces.size(); i++)
    {
        _forces[i]->addDifferentials(df, dq);
    }
}

void FEASystem::addNode(NodeRef node) {
	_nodes.push_back(node);
}

void FEASystem::addElement(ElementRef element) {
	_elements.push_back(element);
	_forces.push_back(ForceRef(element));
}

void FEASystem::addForce(ForceRef force) {
	_forces.push_back(force);
}

} /* namespace ElasticMesh */

