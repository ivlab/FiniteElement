/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Inspired by a class project "GPU Accellerated Implicit FEA" for CSCI 5980: Physics-Based Animation (Fall 2015) taught by Prof. Rahul Narain
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

/*
 * Algorithm for iterative backward euler was developed from the following source:
 *
 * Eftychios Sifakis , Jernej Barbic, FEM simulation of 3D deformable solids: a practitioner's guide to theory, discretization
 * and model reduction, ACM SIGGRAPH 2012 Courses, p.1-50, August 05-09, 2012, Los Angeles, California
 */

#include <integration/GpuIterativeBackwardEuler.cuh>
#include <iostream>


namespace ElasticMesh {

GpuIterativeBackwardEuler::GpuIterativeBackwardEuler(int iterations) : iterations(iterations), currentSize(-1), q0(0), v0(0), M(0,0), f(0), qk(0), vk(0), dx(0), df(0), b(0) {
}

GpuIterativeBackwardEuler::~GpuIterativeBackwardEuler() {
}

// GPU integrator initializer
struct IBE_initialize_functor
{
	double dt;
	IBE_initialize_functor(double dt) : dt(dt) {}
	
    template <typename Tuple>
    __host__ __device__
    void operator()(Tuple t)
    {
    	// qk = q0
        thrust::get<2>(t) = thrust::get<0>(t);
        // vk = v0
        thrust::get<3>(t) = thrust::get<1>(t);
        // dx = dt*v0
        thrust::get<4>(t) = dt*thrust::get<1>(t);
    }
};

// GPU calculation of b
struct IBE_calc_b_functor
{
	double dt;
	IBE_calc_b_functor(double dt) : dt(dt) {}
	
    template <typename Tuple>
    __host__ __device__
    void operator()(Tuple t)
    {
    	//Vector b = (1.0/dt)*M*(v0-vk) + f;
    	thrust::get<4>(t) = (1.0/dt)*thrust::get<0>(t)*(thrust::get<1>(t) - thrust::get<2>(t)) + thrust::get<3>(t);
    }
};

// GPU increment of q and v
struct IBE_inc_qv_functor
{
	double dt;
	IBE_inc_qv_functor(double dt) : dt(dt) {}
	
    template <typename Tuple>
    __host__ __device__
    void operator()(Tuple t)
    {
		//qk = qk + dx;
    	thrust::get<0>(t) = thrust::get<0>(t) + thrust::get<2>(t);
		//vk = vk + (1.0/dt)*dx;
    	thrust::get<1>(t) = thrust::get<1>(t) + (1.0/dt)*thrust::get<2>(t);
    }
};

void GpuIterativeBackwardEuler::step(PhysicalSystem<VectorXd, MatrixXd>* system,
		double dt) {
	// allocate gpu arrays
	int n = system->getDOFs();
	allocateArrays(system, n);

	// initialize vectors
	thrust::for_each(thrust::make_zip_iterator(thrust::make_tuple(q0.v.begin(), v0.v.begin(), qk.v.begin(), vk.v.begin(), dx.v.begin())),
	        thrust::make_zip_iterator(thrust::make_tuple(q0.v.end(), v0.v.end(), qk.v.end(), vk.v.end(), dx.v.end())),
			IBE_initialize_functor(dt));
	
	for (int k = 0; k < iterations; k++)
	{
		system->getForces(f);
		
		//Vector b = (1.0/dt)*M*(v0-vk) + f;
		thrust::for_each(thrust::make_zip_iterator(thrust::make_tuple(M.mat.begin(), v0.v.begin(), vk.v.begin(), f.v.begin(), b.v.begin())),
			thrust::make_zip_iterator(thrust::make_tuple(M.mat.end(), v0.v.end(), vk.v.end(), f.v.end(), b.v.end())),
			IBE_calc_b_functor(dt));
		
		//dx = cg.solve(Aw, b, dx);
		// TODO: Implement gpu conjugate gradient solver
		
		// Explicit (for now this should work like explicit integration in theory)
		// dx = (dt*dt/M)*b;
		thrust::fill(dx.v.begin(), dx.v.end(), dt*dt);
		thrust::transform(dx.v.begin(), dx.v.end(), M.mat.begin(), dx.v.begin(), thrust::divides<double>());
		dx *= b;

		//qk = qk + dx;
		//vk = vk + (1.0/dt)*dx;
		thrust::for_each(thrust::make_zip_iterator(thrust::make_tuple(qk.v.begin(), vk.v.begin(), dx.v.begin())),
				thrust::make_zip_iterator(thrust::make_tuple(qk.v.end(), vk.v.end(), dx.v.end())),
				IBE_inc_qv_functor(dt));
		
		system->setState(qk, vk);
	}
	
	// reset initial q and v
	q0 = qk; 
	v0 = vk;
}

void GpuIterativeBackwardEuler::allocateArrays(PhysicalSystem<VectorXd, MatrixXd>* system, int size) {
	// reinitialize gpu memory if the node size has changed
	if (size != currentSize)
	{
		q0 = VectorXd(size);
		v0 = VectorXd(size);
		M = MatrixXd(size, 1);
		f = VectorXd(size);
		qk = VectorXd(size);
		vk = VectorXd(size);
		dx = VectorXd(size);
		df = VectorXd(size);
		b = VectorXd(size);

		// copy values from cpu
		system->getState(q0, v0);
		system->getInertia(M);
		
		currentSize = size;
	}
}

} /* namespace ElasticMesh */
