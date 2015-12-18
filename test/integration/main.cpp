/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <iostream>
#include "integration/PhysicalSystem.h"
#include "integration/solvers/ForwardEuler.h"

using namespace std;
using namespace ElasticMesh;

class ValMathSystem : public MathSystem<double, double>
{
public:
	ValMathSystem() {}
	virtual ~ValMathSystem() {}

	double* allocateVector(int size)
	{
		return new double(0);
	}
	double* allocateMatrix(int rows, int cols)
	{
		return new double(0);
	}
	virtual void freeVector(double* vector)
	{
		delete vector;
	}
	virtual void freeMatrix(double* matrix)
	{
		delete matrix;
	}
};

class PSystem : public PhysicalSystem<double, double>
{
public:
	PSystem() : pos(0.0), velocity(5.0), m(1.0) {}
	virtual ~PSystem() {}

	// Return the mathematical system being used
	MathSystem<double, double>* getMathSystem() { return &_mathSystem; }
    // Return number of *positional* degrees of freedom (not velocity)
    int getDOFs() {return 1;}
    // Write position and velocity into vectors
    void getState(double &q, double &v) {
    	q = pos;
    	v = velocity;
    }
    // Read position and velocity from vectors
    void setState(const double &q, const double &v)
    {
    	pos = q;
    	velocity = v;
    }
    // Write mass matrix
    void getInertia(double &M) {
    	M = m;
    }
    // Write inverse mass matrix
    void getInverseInertia(double &Mi) {
    	Mi = m;
    }
    // Write forces
    void getForces(double &f) {
    	f = 0.0;
    }
    // Write Jacobians
    void getJacobians(double &Jx, double &Jv) {
    	Jx = 0.0;
    	Jv = 0.0;
    }
    // Write Differentials
    void getForceDifferentials(double &df, double& dq) {
    	df = 0.0;
    }

    void printState()
    {
    	cout << "Pos: " << pos << " " << "Velocity: " << velocity << endl;
    }

private:
    ValMathSystem _mathSystem;
    double pos;
    double velocity;
    double m;
};

int main () {
	cout << "Hello world." << endl;

	PSystem system;
	system.printState();

	ForwardEuler<double, double> euler;
	euler.step(&system, 0.1);
	system.printState();
	euler.step(&system, 0.1);
	system.printState();
	euler.step(&system, 0.1);
	system.printState();
	euler.step(&system, 0.1);
	system.printState();
}
