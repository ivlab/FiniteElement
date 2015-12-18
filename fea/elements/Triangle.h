/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */


#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "fea/Element.h"
#include "fea/Node.h"

namespace ElasticMesh {

class Triangle : public Element {
public:
	Triangle(MaterialModelRef materialModel, NodeRef i, NodeRef j, NodeRef k);
	virtual ~Triangle();

	void init();
	void addForces(Eigen::VectorXd &f);
	void addJacobians(Eigen::MatrixXd &Jx, Eigen::MatrixXd &Jv);
	void addDifferentials(Eigen::VectorXd &df, Eigen::VectorXd& dq);

private:
	NodeRef _nodes[3];
	Eigen::Matrix2d _B;
	double _w;
};

} /* namespace ElasticMesh */

#endif /* TETRAHEDRAL_H_ */
