/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef NODE_H_
#define NODE_H_

#include <Eigen/Dense>
#include <memory>

namespace ElasticMesh {

class Node;
typedef std::shared_ptr<Node> NodeRef;

// Represents a node for an FEA system
class Node {
public:
	Node(int index, double mass, Eigen::Vector3d x, Eigen::Vector3d v);
	virtual ~Node();

	// Node index
	int i;
	// mass
	double m;
	// position
	Eigen::Vector3d x;
	// velocity
	Eigen::Vector3d v;
	// starting position
	Eigen::Vector3d initialPos;
};

} /* namespace ElasticMesh */

#endif /* NODE_H_ */
