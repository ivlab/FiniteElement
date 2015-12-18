/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef MATERIAL_MODEL_H_
#define MATERIAL_MODEL_H_

#include <vector>
#include "Force.h"
#include "MaterialModel.h"

namespace ElasticMesh {

class Element;
typedef std::shared_ptr<Element> ElementRef;

// Represents an Element for an FEA system
class Element : public Force {
public:
	Element(MaterialModelRef model) : _model(model) {}
	virtual ~Element() {}

	// initialization method which creates the rest configuration for an element
	virtual void init() = 0;

	// gets the material model
	const MaterialModelRef& getModel() const {
		return _model;
	}

	// sets the material model
	void setModel(const MaterialModelRef& model) {
		_model = model;
	}

private:
	MaterialModelRef _model;
};

} /* namespace ElasticMesh */

#endif /* MATERIAL_MODEL_H_ */
