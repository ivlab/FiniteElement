/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef LINEARELASTICMODEL_H_
#define LINEARELASTICMODEL_H_

#include "fea/MaterialModel.h"

namespace ElasticMesh {

// Linear Elastic material model
class LinearElasticModel : public MaterialModel {
public:
	LinearElasticModel(double u, double lambda);
	virtual ~LinearElasticModel();

	Eigen::MatrixXd piolaStress(const Eigen::MatrixXd& F);
	Eigen::MatrixXd piolaStressDeriv(const Eigen::MatrixXd& F, const Eigen::MatrixXd& dF);

private:
	double u;
	double lambda;
};

} /* namespace ElasticMesh */

#endif /* LINEARELASTICMODEL_H_ */
