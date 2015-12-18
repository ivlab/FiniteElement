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
#include "fea/FEASystem.h"
#include "integration/solvers/ForwardEuler.h"
#include "fea/forces/ConstantForce.h"

using namespace std;
using namespace ElasticMesh;
using namespace Eigen;

void printSystem(FEASystem& system);

int main () {
	FEASystem system;

	system.addNode(NodeRef(new Node(0, 1.0, Vector3d(1.0,1.0,0.0), Vector3d(0.0,0.0,0.0))));
	system.addForce(ForceRef(new ConstantForce(&system, Vector3d(0.0,-9.8,0.0))));

	double stepSize = 1.0/30.0;

	//system.init();
	ForwardEuler<VectorXd, MatrixXd> euler;
	euler.step(&system, stepSize);
	printSystem(system);
	euler.step(&system, stepSize);
	printSystem(system);
	euler.step(&system, stepSize);
	printSystem(system);
	euler.step(&system, stepSize);
	printSystem(system);
	euler.step(&system, stepSize);
	printSystem(system);
	euler.step(&system, stepSize);
	printSystem(system);
	euler.step(&system, stepSize);
	printSystem(system);
	euler.step(&system, stepSize);
	printSystem(system);
	euler.step(&system, stepSize);
	printSystem(system);
	euler.step(&system, stepSize);
	printSystem(system);

}

void printSystem(FEASystem& system)
{
	const std::vector<NodeRef>& nodes = system.getNodes();
	for (int f = 0; f < nodes.size(); f++)
	{
		cout << f << ": (" << nodes[f]->x[0] << ", " << nodes[f]->x[1] << ", " << nodes[f]->x[2] << ")" << endl;
	}
}
