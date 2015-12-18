/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef MATERIALMODEL_H_
#define MATERIALMODEL_H_

#include <Eigen/Dense>
#include <memory>

namespace ElasticMesh {

class MaterialModel;
typedef std::shared_ptr<MaterialModel> MaterialModelRef;

class MaterialModel {
public:
	virtual ~MaterialModel() {}

	// calculates piola stress
	virtual Eigen::MatrixXd piolaStress(const Eigen::MatrixXd& F) = 0;
	// calculates piola stress derivative
	virtual Eigen::MatrixXd piolaStressDeriv(const Eigen::MatrixXd& F, const Eigen::MatrixXd& dF) = 0;
};

} /* namespace ElasticMesh */

#endif /* MATERIALMODEL_H_ */
