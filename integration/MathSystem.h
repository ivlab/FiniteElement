/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef STORAGESYSTEM_H_
#define STORAGESYSTEM_H_

namespace ElasticMesh {

// Math system is for allocation of vector and matrix variables
template<typename Vector, typename Matrix>
class MathSystem {
public:
	virtual ~MathSystem() {}

	// Allocates a vector
	virtual Vector* allocateVector(int size) = 0;
	// Allocates a matrix
	virtual Matrix* allocateMatrix(int rows, int cols) = 0;
	// Frees a vector
	virtual void freeVector(Vector *vector) = 0;
	// Frees a matrix
	virtual void freeMatrix(Matrix *matrix) = 0;
};

// Simple version of a math system which assumes a Vector and Matrix constructor
template<typename Vector, typename Matrix>
class SimpleMathSystem : public MathSystem<Vector, Matrix>
{
public:
	SimpleMathSystem() {}
	virtual ~SimpleMathSystem() {}

	Vector* allocateVector(int size)
	{
		return new Vector(size);
	}
	Matrix* allocateMatrix(int rows, int cols)
	{
		return new Matrix(rows, cols);
	}
	virtual void freeVector(Vector* vector)
	{
		delete vector;
	}
	virtual void freeMatrix(Matrix* matrix)
	{
		delete matrix;
	}
};

} /* namespace ElasticMesh */

#endif /* STORAGESYSTEM_H_ */
