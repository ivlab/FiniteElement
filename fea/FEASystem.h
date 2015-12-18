/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef FEASYSTEM_H_
#define FEASYSTEM_H_

#include "integration/PhysicalSystem.h"
#include "Node.h"
#include <Eigen/Dense>
#include <vector>
#include "Force.h"
#include "Element.h"

namespace ElasticMesh {

class FEASystem : public PhysicalSystem<Eigen::VectorXd, Eigen::MatrixXd> {
public:
	FEASystem();
	virtual ~FEASystem();

	// initializes the system after nodes and elements have been added
	void init();

	// Physical system interface
	MathSystem<Eigen::VectorXd, Eigen::MatrixXd>* getMathSystem();
    int getDOFs();
    void getState(Eigen::VectorXd &q, Eigen::VectorXd &v);
    void setState(const Eigen::VectorXd &q, const Eigen::VectorXd &v);
    void getInertia(Eigen::MatrixXd &M);
    void getInverseInertia(Eigen::MatrixXd &Mi);
    void getForces(Eigen::VectorXd &f);
    void getJacobians(Eigen::MatrixXd &Jx, Eigen::MatrixXd &Jv);
    void getForceDifferentials(Eigen::VectorXd &df, Eigen::VectorXd& dq);

    // adds nodes to the system
    void addNode(NodeRef node);
    // adds elements to the system
    void addElement(ElementRef element);
    // adds forces to the system
    void addForce(ForceRef force);

    // gets the nodes
	const std::vector<NodeRef>& getNodes() const {
		return _nodes;
	}

private:
	SimpleMathSystem<Eigen::VectorXd, Eigen::MatrixXd> _mathSystem;
	std::vector<NodeRef> _nodes;
	std::vector<ElementRef> _elements;
	std::vector<ForceRef> _forces;
};

} /* namespace ElasticMesh */

#endif /* FEASYSTEM_H_ */
