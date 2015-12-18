/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "fea/FEASystem.h"
#include "integration/solvers/ForwardEuler.h"
#include "integration/solvers/BackwardEuler.h"
#include "fea/forces/ConstantForce.h"
#include "fea/forces/AnchorForce.h"
#include "fea/elements/Triangle.h"
#include "fea/elements/Tetrahedral.h"
#include "fea/materials/LinearElasticModel.h"
#include "fea/solvers/EigenLinearSolver.h"
#include "fea/materials/StVenantKirchhoff.h"
#include "integration/solvers/iterative/IterativeBackwardEuler.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include "gpu/fea/FEAInterface.h"

using namespace std;
using namespace ElasticMesh;
using namespace Eigen;

// Variables used for mouse movement
AnchorForce* mouseForce;
NodeRef mouseNode;
bool xyPlane = false;
int windowWidth = 640;
int windowHeight = 480;

// Helper functions for graphics and interaction
static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void setVertexColor(NodeRef node);
void drawVertex(NodeRef node, double* pos);

int main(int argc, char** argv)
{
	// GPU specific flag for implementations that use the gpu
	bool useGpu = argc > 2 && string(argv[2]) == "gpu";
	FEAInterface* gpuFEASystem = FEAInterface::create();

	// Create GLFW window
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(windowWidth, windowHeight, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Create context and set interaction callbacks
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	// Initialize OpenGL
	glClearDepth(-1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GEQUAL);

	// FEA System initialization
	FEASystem system;

	// Helper variables for quick node and tet access
	const std::vector<NodeRef>& nodes = system.getNodes();
	std::vector<Tetrahedral*> tets;

	// Graphics and FEA parameters that can be overriden based on the example
	double scale = 1.0;
	double mass = 1.0;
	double stepSize = 1.0/60.0;
	double rotationAngle = 0.0;
	bool solid = false;

	// Used for the stretch example to specify edges of a cube
	std::vector<AnchorForce*> leftIndex;
	std::vector<AnchorForce*> rightIndex;

	// Example 1 - Movable Tetrahedral
	if (string(argv[1]) == "1")
	{
		// parameters
		solid = true;
		stepSize = 0.1;

		// Material model
		MaterialModelRef model = MaterialModelRef(new StVenantKirchhoff(10.0,1.0));

		// Add nodes
		system.addNode(NodeRef(new Node(0, mass, Vector3d(0.0,0.5,-0.5), Vector3d(0.0,0.0,0.0))));
		system.addNode(NodeRef(new Node(1, mass, Vector3d(-1.0,-0.5,-0.5), Vector3d(0.0,0.0,0.0))));
		system.addNode(NodeRef(new Node(2, mass, Vector3d(0.0,0.0,0.0), Vector3d(0.0,0.0,0.0))));
		system.addNode(NodeRef(new Node(3, mass, Vector3d(1.0,-0.5,-0.5), Vector3d(0.0,0.0,0.0))));

		// Add element
		tets.push_back(new Tetrahedral(model, nodes[0], nodes[1], nodes[2], nodes[3]));

		// Bind second node to the mouse
		mouseNode = nodes[2];
		mouseForce = new AnchorForce(mouseNode, mouseNode->x, 1.0, 1.0);
		system.addForce(ForceRef(mouseForce));
	}


	// Example 2 - Deformable cube
	else if (string(argv[1]) == "2")
	{
		// parameters
		mass = 0.5;
		solid = false;
		stepSize = 0.1;
		scale = 0.5;
		rotationAngle = 30.0;

		// Material model
		MaterialModelRef model = MaterialModelRef(new StVenantKirchhoff(1.0,1.0));

		// Add nodes
		system.addNode(NodeRef(new Node(0, mass, Vector3d(0.0, 0.0, 0.0), Vector3d(0.0,0.0,0.0))));
		system.addNode(NodeRef(new Node(1, mass, Vector3d(0.0, 0.0, 1.0), Vector3d(0.0,0.0,0.0))));
		system.addNode(NodeRef(new Node(2, mass, Vector3d(0.0, 1.0, 0.0), Vector3d(0.0,0.0,0.0))));
		system.addNode(NodeRef(new Node(3, mass, Vector3d(0.0, 1.0, 1.0), Vector3d(0.0,0.0,0.0))));
		system.addNode(NodeRef(new Node(4, mass, Vector3d(1.0, 0.0, 0.0), Vector3d(0.0,0.0,0.0))));
		system.addNode(NodeRef(new Node(5, mass, Vector3d(1.0, 0.0, 1.0), Vector3d(0.0,0.0,0.0))));
		system.addNode(NodeRef(new Node(6, mass, Vector3d(1.0, 1.0, 0.0), Vector3d(0.0,0.0,0.0))));
		system.addNode(NodeRef(new Node(7, mass, Vector3d(1.0, 1.0, 1.0), Vector3d(0.0,0.0,0.0))));

		// Add tets
		tets.push_back(new Tetrahedral(model, nodes[4- 1], nodes[3- 1], nodes[5- 1], nodes[1- 1]));
		tets.push_back(new Tetrahedral(model, nodes[4- 1], nodes[2- 1], nodes[5- 1], nodes[1- 1]));
		tets.push_back(new Tetrahedral(model, nodes[4- 1], nodes[7- 1], nodes[3- 1], nodes[5- 1]));
		tets.push_back(new Tetrahedral(model, nodes[4- 1], nodes[7- 1], nodes[8- 1], nodes[5- 1]));
		tets.push_back(new Tetrahedral(model, nodes[4- 1], nodes[6- 1], nodes[2- 1], nodes[5- 1]));
		tets.push_back(new Tetrahedral(model, nodes[4- 1], nodes[6- 1], nodes[8- 1], nodes[5- 1]));

		// Bind node 0 to the mouse
		mouseNode = nodes[0];
		mouseForce = new AnchorForce(mouseNode, mouseNode->x, 1.0, 1.0);
		system.addForce(ForceRef(mouseForce));
	}


	// Example 3 - Stretched cube
	else if (string(argv[1]) == "3")
	{
		// paramerts
		solid = false;
		scale = 0.25;
		mass = 0.1;
		rotationAngle = 30.0;

		// Material model
		MaterialModelRef model = MaterialModelRef(new StVenantKirchhoff(1.0,20.0));

		// Load nodes and elements from file
		std::ifstream nodefile("nodes.txt");
		int numNodes = 0;
		if (nodefile)
		{
			double a, b, c;
			while (nodefile >> a >> b >> c)
			{
				system.addNode(NodeRef(new Node(numNodes, mass, Vector3d(a-2.0,b-2.0,c), Vector3d(0.0,0.0,0.0))));
				if (a <= 0.0001)
				{
					AnchorForce* force = new AnchorForce(nodes[numNodes], nodes[numNodes]->x-Vector3d(0.0,0,0), 1.0, 1.0);
					leftIndex.push_back(force);
					system.addForce(ForceRef(force));
				}
				else if (a >= 3.999)
				{
					AnchorForce* force = new AnchorForce(nodes[numNodes], nodes[numNodes]->x+Vector3d(0.0,0,0), 1.0, 1.0);
					rightIndex.push_back(force);
					system.addForce(ForceRef(force));
				}

				numNodes++;
			}

			nodefile.close();
		}

		std::ifstream elementfile("elements.txt", ios::in);
		int numElements = 0;
		if (elementfile)
		{
			double a, b, c, d;
			while (elementfile >> a >> b >> c >> d)
			{
				tets.push_back(new Tetrahedral(model, nodes[a-1], nodes[b-1], nodes[c-1], nodes[d-1]));
				numElements++;
		 	}

			elementfile.close();
		}
	}

	// If we are using the GPU copy FEASystem to GPU project.  The GPU library uses
	// a different approach for storing and using memory on the GPU.
	if (useGpu)
	{
		// node and element initialization
		double *x = new double[nodes.size()*3];
		double *v = new double[nodes.size()*3];
		double *m = new double[nodes.size()*3];
		int *indices = new int[tets.size()*4];

		// Copy node data from objects to arrays
		for (int f = 0; f < nodes.size(); f++)
		{
			x[f*3+0] = nodes[f]->x[0]; x[f*3+1] = nodes[f]->x[1]; x[f*3+2] = nodes[f]->x[2];
			v[f*3+0] = nodes[f]->v[0]; v[f*3+1] = nodes[f]->v[1]; v[f*3+2] = nodes[f]->v[2];
			m[f*3+0] = nodes[f]->m; m[f*3+1] = nodes[f]->m; m[f*3+2] = nodes[f]->m;
		}

		// Set the nodes
		gpuFEASystem->setNodes(nodes.size(), x, v, m);

		// Copy element data to arrays
		for (int f = 0; f < tets.size(); f++)
		{
			for (int i = 0; i < 4; i++)
			{
				indices[f*4 + i] = tets[f]->getNode(i)->i;
			}
		}

		// Set the elements
		gpuFEASystem->setElements(tets.size(), indices);

		// Initialize the GPU FEA System
		gpuFEASystem->init();
	}
	// Otherwise, use the FEA System itself
	else
	{
		// Add the tetrahedrals to the CPU FEA system
		for (int f = 0; f < tets.size(); f++)
		{
			system.addElement(ElementRef(tets[f]));
		}

		// Initialize CPU System
		system.init();
	}

	// Create the CPU integrator that will be used
	Solver<VectorXd, MatrixXd>* integrator;
	if (argc > 2 && string(argv[2]) == "explicit")
	{
		// Explicit euler integrator
		integrator = new ForwardEuler<VectorXd, MatrixXd>();
	}
	else
	{
		// Implicit euler integrator
		integrator = new IterativeBackwardEuler<VectorXd, MatrixXd>(10, 0.000003, 20, 1.0);
	}

	// Initilization of graphics variables
	float totalStep = 0.0;
	int frame = 0;
	if (!solid)
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}

	// Graphics loop
	while (!glfwWindowShouldClose(window))
	{
		// If we are to use the GPU, increment the step through the GPU system
		// Otherwise, use the CPU integrator
		if (useGpu)
		{
			gpuFEASystem->step(stepSize);
		}
		else
		{
			integrator->step(&system, stepSize);
		}

		totalStep += stepSize;
		cout << totalStep << endl;

		// Graphics implementation
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float) height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(rotationAngle, 1.f, 1.f, 1.f);
		glScaled(scale, scale, scale);

		// Used for the stretch example to pull the two sides away from each other
		// The forces are released after 2 seconds
		if (totalStep < 2.0)
		{
			for (int f = 0; f < leftIndex.size(); f++)
			{
				leftIndex[f]->x[0] -= 0.01;
				leftIndex[f]->node->x[0] -= 0.01;
			}
			for (int f = 0; f < rightIndex.size(); f++)
			{
				rightIndex[f]->x[0] += 0.01;
				rightIndex[f]->node->x[0] += 0.01;
			}
		}
		else
		{
			for (int f = 0; f < leftIndex.size(); f++)
			{
				leftIndex[f]->node = NULL;
			}
			for (int f = 0; f < rightIndex.size(); f++)
			{
				rightIndex[f]->node = NULL;
			}
		}

		glColor3f(1.f, 0.f, 0.f);

		// Draw the tetrahedrals
		glBegin(GL_TRIANGLES);

		for (int f = 0; f < tets.size(); f++)
		{
			if (useGpu)
			{
				double* pos = gpuFEASystem->getNodePositions();

				drawVertex(tets[f]->getNode(0), pos);
				drawVertex(tets[f]->getNode(1), pos);
				drawVertex(tets[f]->getNode(2), pos);

				drawVertex(tets[f]->getNode(0), pos);
				drawVertex(tets[f]->getNode(1), pos);
				drawVertex(tets[f]->getNode(3), pos);

				drawVertex(tets[f]->getNode(3), pos);
				drawVertex(tets[f]->getNode(1), pos);
				drawVertex(tets[f]->getNode(2), pos);

				drawVertex(tets[f]->getNode(0), pos);
				drawVertex(tets[f]->getNode(3), pos);
				drawVertex(tets[f]->getNode(2), pos);
			}
			else
			{
				setVertexColor(tets[f]->getNode(0));
				glVertex3f(tets[f]->getNode(0)->x[0], tets[f]->getNode(0)->x[1], tets[f]->getNode(0)->x[2]);
				setVertexColor(tets[f]->getNode(1));
				glVertex3f(tets[f]->getNode(1)->x[0], tets[f]->getNode(1)->x[1], tets[f]->getNode(1)->x[2]);
				setVertexColor(tets[f]->getNode(2));
				glVertex3f(tets[f]->getNode(2)->x[0], tets[f]->getNode(2)->x[1], tets[f]->getNode(2)->x[2]);

				setVertexColor(tets[f]->getNode(0));
				glVertex3f(tets[f]->getNode(0)->x[0], tets[f]->getNode(0)->x[1], tets[f]->getNode(0)->x[2]);
				setVertexColor(tets[f]->getNode(1));
				glVertex3f(tets[f]->getNode(1)->x[0], tets[f]->getNode(1)->x[1], tets[f]->getNode(1)->x[2]);
				setVertexColor(tets[f]->getNode(3));
				glVertex3f(tets[f]->getNode(3)->x[0], tets[f]->getNode(3)->x[1], tets[f]->getNode(3)->x[2]);

				setVertexColor(tets[f]->getNode(3));
				glVertex3f(tets[f]->getNode(3)->x[0], tets[f]->getNode(3)->x[1], tets[f]->getNode(3)->x[2]);
				setVertexColor(tets[f]->getNode(1));
				glVertex3f(tets[f]->getNode(1)->x[0], tets[f]->getNode(1)->x[1], tets[f]->getNode(1)->x[2]);
				setVertexColor(tets[f]->getNode(2));
				glVertex3f(tets[f]->getNode(2)->x[0], tets[f]->getNode(2)->x[1], tets[f]->getNode(2)->x[2]);

				setVertexColor(tets[f]->getNode(0));
				glVertex3f(tets[f]->getNode(0)->x[0], tets[f]->getNode(0)->x[1], tets[f]->getNode(0)->x[2]);
				setVertexColor(tets[f]->getNode(3));
				glVertex3f(tets[f]->getNode(3)->x[0], tets[f]->getNode(3)->x[1], tets[f]->getNode(3)->x[2]);
				setVertexColor(tets[f]->getNode(2));
				glVertex3f(tets[f]->getNode(2)->x[0], tets[f]->getNode(2)->x[1], tets[f]->getNode(2)->x[2]);
			}
		}
		glEnd();

		glfwSwapBuffers(window);
		glfwPollEvents();

		frame++;
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	delete integrator;

	exit(EXIT_SUCCESS);
}


//----------------------- Graphics and Interaction helper functions ---------------------

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		mouseForce->x[2] += 0.1;
	}
	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		mouseForce->x[2] -= 0.1;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		xyPlane = true;
		mouseForce->node = mouseNode;
		mouseForce->x[2] = 0.0;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		xyPlane = false;
		mouseForce->node = NULL;
	}
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (xyPlane)
	{
		mouseForce->x[0] = 1.0-2.0*(windowWidth-xpos)/windowWidth;
		mouseForce->x[1] = 2.0*(windowHeight-ypos)/windowHeight-1.0;
	}
}

void setVertexColor(NodeRef node)
{
	Eigen::Vector3d n = (node->initialPos).normalized();
	n = (n+Vector3d(1.0,1.0,1.0))/2.0;
	glColor3f(n[0], n[1], n[2]);
}

void drawVertex(NodeRef node, double* pos)
{
	setVertexColor(node);
	glVertex3f(pos[node->i*3 + 0], pos[node->i*3 + 1], pos[node->i*3 + 2]);
}
