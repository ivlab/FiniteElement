# FiniteElement

This project implements FEM using implicit and explicit integration on both the CPU and the GPU.
The GPU implementation is not quite completed. The parts in the code that have not been implemented 
are specified with the "TODO" keyword.

### Project Status

Here are the items that have been implemented:
 * Explicit Euler CPU integration for both Triangles and Tetrahedrals
 * Implicit Euler GPU integration for Tetrahedrals
 * GPU framework for implementing implicit euler integration.

TODO (Still working on these):
 * Computing elastic forces and material model on the GPU
 * Computing elastic differentials on the GPU
 * Conjugate Gradient solving on the GPU

If you would like to look at an example of how to use this library, please look at the simple FEA example program:
apps/simple/main.cpp

### Code Organization

 * apps - application folder
   * simple - simpleFEA program for displaying the three examples below
 * deps - dependencies which will automatically be downloaded
   * GLFW - graphics windowing library
 * fea - CPU FEA library
 * gpu - GPU FEA library
 * integration - integration and solver library
 * test - various tests for maintaining application

### Build and Install

Build the project:
  ```
  # configure cmake project
  make

  # build release
  make opt
  ```

Run the project
  ```
  # Tetrahedral example (implicit integration)
  ./build/Release/bin/simpleFEA 1

  # Cube example (implicit integration)
  ./build/Release/bin/simpleFEA 2

  # Stretch example (implicit integration)
  ./build/Release/bin/simpleFEA 3

  # Explicit integration (add explicit keyword to any example)
  ./build/Release/bin/simpleFEA 1 explicit

  # Gpu exectution (not fully functional)
  ./build/Release/bin/simpleFEA 3 gpu

