/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef TETRAHEDRAL_H_
#define TETRAHEDRAL_H_

#include "fea/Element.h"
#include "fea/Node.h"

namespace ElasticMesh {

class Tetrahedral : public Element {
public:
	Tetrahedral(MaterialModelRef materialModel, NodeRef i, NodeRef j, NodeRef k, NodeRef l);
	virtual ~Tetrahedral();

	void init();
	void addForces(Eigen::VectorXd &f);
	void addJacobians(Eigen::MatrixXd &Jx, Eigen::MatrixXd &Jv);
	void addDifferentials(Eigen::VectorXd &df, Eigen::VectorXd& dq);
	NodeRef& getNode(int i)
	{
		return _nodes[i];
	}

private:
	NodeRef _nodes[4];
	Eigen::Matrix3d _B;
	double _w;
};

} /* namespace ElasticMesh */

#endif /* TETRAHEDRAL_H_ */
