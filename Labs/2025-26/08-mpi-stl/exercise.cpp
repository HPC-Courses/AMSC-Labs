// This example is modified starting from a MIT licensed NVIDIA one, 
// Original license follows.

/*
 * SPDX-FileCopyrightText: Copyright (c) 2022 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <cassert>      // For debugging assertions
#include <chrono>       // For time measurement and manipulation
#include <fstream>      // For file input and output operations
#include <iostream>     // For standard input and output streams
#include <mpi.h>       // For MPI (Message Passing Interface) functionalities
#include <vector>       // For using the std::vector container
// TODO (c++23)
       // For range-based operations and views
#include <algorithm>    // For standard algorithms like std::fill_n
#include <numeric>      // For numeric operations like std::transform_reduce
#include <execution>     // For parallel execution policies (e.g., std::execution::par)
// TODO
       // For multi-threading support
       // For atomic operations and types
       // For synchronization barriers in multi-threading

// Structure to hold problem parameters for the heat equation simulation
struct parameters {
  double dx, dt;      // Spatial and temporal step sizes
  long nx, ny, ni;    // Grid dimensions (nx: number of grid points in x, ny: number of grid points in y, ni: number of iterations)
  int rank = 0, nranks = 1; // MPI rank (unique identifier for each process) and total number of ranks (processes)

  // Static method to return the thermal diffusivity constant
  static constexpr double alpha() { return 1.0; } // Thermal diffusivity

  // Constructor to initialize parameters from command line arguments
  parameters(int argc, char *argv[]);
    
  // Accessor methods for various parameters
  long nit() { return ni; } // Returns the number of iterations
  long nout() { return 1000; } // Returns the output frequency (how often to output results)
  long nx_global() { return nx * nranks; } // Returns the global number of grid points in x across all ranks
  long ny_global() { return ny; } // Returns the global number of grid points in y (same for all ranks)
  double gamma() { return alpha() * dt / (dx * dx); } // Calculates the gamma value used in the stencil computation
  long n() { return ny * (nx + 2 /* 2 halo layers */); } // Returns the total number of grid points including halo layers
};

// Structure to define a 2D grid of indices for the simulation
struct grid {
  long x_begin, x_end; // Start and end indices in the x dimension
  long y_begin, y_end; // Start and end indices in the y dimension
};

// Function declarations for applying the stencil and initializing conditions
double apply_stencil(double* u_new, double* u_old, grid g, parameters p);
void initial_condition(double* u_new, double* u_old, long n);

// Function declarations for evolving the solution of different parts of the local domain
double inner(double* u_new, double* u_old, parameters p); // Evolve the interior part of the domain
double prev (double* u_new, double* u_old, parameters p); // Evolve the part of the domain that depends on the previous rank
double next (double* u_new, double* u_old, parameters p); // Evolve the part of the domain that depends on the next rank

int main(int argc, char *argv[]) {
  // Parse CLI parameters
  parameters p(argc, argv);
  
  // Initialize MPI with multi-threading support
  // Declare an integer variable to hold the threading support level
  int mt;
  
  // TODO: Initialize the MPI environment with support for multi-threading.
  
  
  // TODO: Check the level of thread support
  
  
    // Get the total number of processes (ranks) in the communicator MPI_COMM_WORLD.
  // This value is stored in 'p.nranks', which will be used to determine how many processes are participating.
  MPI_Comm_size(MPI_COMM_WORLD, &p.nranks);
  
    // Get the rank (unique identifier) of the calling process within the communicator MPI_COMM_WORLD.
  // This value is stored in 'p.rank', which will be used to identify the specific process in the parallel computation.
  MPI_Comm_rank(MPI_COMM_WORLD, &p.rank);

  // Allocate memory for the new and old temperature fields.
  // 'u_new' will hold the updated temperature values, while 'u_old' will hold the previous values.
  // The size of the vectors is determined by the total number of grid points, which is obtained from the parameters.
  std::vector<double> u_new(p.n()), u_old(p.n());

  // Set the initial conditions for the temperature fields.
  // The 'initial_condition' function initializes 'u_new' and 'u_old' with the starting temperature values.
  initial_condition(u_new.data(), u_old.data(), p.n());

  // Prepare the time loop for the simulation.
  // Using 'clk_t' as an alias for 'std::chrono::steady_clock' to measure elapsed time.
  using clk_t = std::chrono::steady_clock;
  // Record the start time of the simulation.
  auto start = clk_t::now();
  
  //TODO: Create an atomic variable to store the energy, which can be safely modified by multiple threads.
  // The atomic type ensures that updates to the energy variable are thread-safe, preventing data races.

  // TODO: Create a barrier for synchronizing three threads.
  // The barrier will ensure that all threads reach a certain point in the code before any of them can proceed.
  

  // TODO: Create a new thread named 'thread_prev' to perform the "prev" computation.
    // The thread captures the parameters 'p', pointers to the new and old temperature arrays ('u_new' and 'u_old'),
  // a reference to the atomic 'energy' variable, and the barrier 'bar' for synchronization.
  //Note: std::barrier is different from MPI_Barrier
  
  // TODO: Create a new thread named 'thread_next' to perform the "next" computation.
    // The thread captures the parameters 'p', pointers to the new and old temperature arrays ('u_new' and 'u_old'),
  // a reference to the atomic 'energy' variable, and the barrier 'bar' for synchronization.
  
    // TODO:Create a new thread named 'thread_inner' to perform the "inner" computation,
  // which includes the MPI reduction and I/O operations.
  // The thread captures the parameters 'p', pointers to the new and old temperature arrays ('u_new' and 'u_old'),
  // a reference to the atomic 'energy' variable, and the barrier 'bar' for synchronization.
 
  
    
  // TODO: Join all threads to ensure synchronization

    // Calculate the elapsed time since the start of the simulation.
  // 'clk_t::now()' gets the current time, and 'start' is the time when the simulation began.
  // The duration is measured in seconds (as a double).
  auto time = std::chrono::duration<double>(clk_t::now() - start).count();


  // Calculate the size of the grid in gigabytes (GB).
  // The grid size is computed as the product of the number of grid points in the x and y dimensions (nx and ny),
  // multiplied by the size of a double (sizeof(double)), and multiplied by 2 to account for both the new and old temperature arrays.
  // The result is then converted to gigabytes by multiplying by 1e-9.
  auto grid_size = static_cast<double>(p.nx * p.ny * sizeof(double) * 2) * 1e-9; // GB
  
  // Calculate the memory bandwidth in gigabytes per second (GB/s).
  // This is done by taking the total grid size (in GB) multiplied by the number of iterations (p.nit()),
  // and dividing by the elapsed time (in seconds).
  auto memory_bw = grid_size * static_cast<double>(p.nit()) / time; // GB/s

  // Only the rank 0 process will output the performance metrics to standard error.
  if (p.rank == 0) {
      // Output the local domain size and memory bandwidth for the current rank.
      std::cerr << "Rank " << p.rank << ": local domain " << p.nx << "x" << p.ny << " (" << grid_size << " GB): " 
                << memory_bw << " GB/s" << std::endl;

      // Output the global domain size and total memory bandwidth across all ranks.
      std::cerr << "All ranks: global domain " << p.nx_global() << "x" << p.ny_global() << " (" 
                << (grid_size * p.nranks) << " GB): " << memory_bw * p.nranks << " GB/s" << std::endl; 
  }
  
  // I/O TODO START: Write output to file, writing both header information and the computed data from each rank to a file named "output". Use non-blocking I/O and ensure synchronization
  
  // Declare an MPI_File object to handle file operations


  // Open a file named "output" for writing. The file will be created if it doesn't exist.
  
  
  // Calculate the size of the header in bytes.
  // The header consists of two long integers (for total dimensions) and one double (for time).
  
  
  // Calculate the number of values each rank will write to the file.
  // This is the product of the local grid dimensions (nx and ny).
  
  // Calculate the number of bytes required for the values written by each rank.
  
  
  // Set the total size of the file to accommodate the header and the data from all ranks.
  // The total size is the sum of the header size and the size of the data from all ranks.
  
  
    // Create an array of MPI_Request objects to handle non-blocking I/O operations.
  // Initialize the requests to null.
  
    // Only the rank 0 process will write the header information to the file.
  if (p.rank == 0) {
      // Prepare an array to hold the total dimensions of the grid.
      long total[2] = {p.nx * p.nranks, p.ny}; // Total grid size across all ranks

      // Calculate the total simulation time based on the number of iterations and time step.
      double time = p.nit() * p.dt;

      // Perform a non-blocking write operation to write the total dimensions to the file at the beginning (offset 0).
      // The data type is MPI_UINT64_T for the long integers.
      
      // Perform a non-blocking write operation to write the simulation time to the file after the total dimensions.
      // The data type is MPI_DOUBLE for the time value.

  }
  
  // Calculate the offset for the values that this rank will write to the file.
  // The offset is determined by the header size plus the number of values written by previous ranks.


  // Perform a non-blocking write operation to write the local temperature data (u_new) to the file.
  // The data is written starting from the calculated offset.


  // Wait for all non-blocking I/O operations to complete.
  // If the current rank is 0, wait for all three requests; otherwise, wait for just the one request.


  // Close the file after all write operations are complete.
   
   
  // I/O TODO END

  MPI_Finalize();
  return 0;
}

// Constructor for the parameters class that reads command line arguments to initialize problem size
parameters::parameters(int argc, char *argv[]) {
  // Check if the correct number of arguments is provided
  if (argc != 4) {
    std::cerr << "ERROR: incorrect arguments" << std::endl; // Print error message
    std::cerr << "  " << argv[0] << " <nx> <ny> <ni>" << std::endl; // Show usage
    std::terminate(); // Terminate the program
  }
  // Convert command line arguments to long long integers for grid sizes
  nx = std::stoll(argv[1]); // Number of grid points in x-direction
  ny = std::stoll(argv[2]); // Number of grid points in y-direction
  ni = std::stoll(argv[3]); // Number of iterations 
  
  // Calculate grid spacing and time step based on the number of grid points
  dx = 1.0 / nx; // Grid spacing in x-direction
  dt = dx * dx / (5. * alpha()); // Time step based on diffusion coefficient
}

// Finite-difference stencil function to update the grid values
double stencil(double *u_new, double *u_old, long x, long y, parameters p) {
  // Lambda function to calculate the index in a row-major order
  auto idx = [=](auto x, auto y) { 
      assert(x >= 0 && x < 2 * p.nx); // Ensure x is within bounds
      assert(y >= 0 && y < p.ny); // Ensure y is within bounds
      return x * p.ny + y; // Calculate index
  };

  // Apply boundary conditions
  if (y == 1) {
    u_old[idx(x, y - 1)] = 0; // Set lower boundary condition
  }
  if (y == (p.ny - 2)) {
    u_old[idx(x, y + 1)] = 0; // Set upper boundary condition
  }
  
  // TODO: notice boundary conditions for the ranks at the ends of the domain
  if (p.rank == 0 && x == 1) {
    u_old[idx(x - 1, y)] = 1; // Set left boundary condition for the first rank
  }
  if (p.rank == (p.nranks - 1) && x == p.nx) {
    u_old[idx(x + 1, y)] = 0; // Set right boundary condition for the last rank
  }

  // Update the new grid value using the finite-difference stencil
  u_new[idx(x, y)] = (1. - 4. * p.gamma()) * u_old[idx(x, y)] +
                     p.gamma() * (u_old[idx(x + 1, y)] + u_old[idx(x - 1, y)] +
                                  u_old[idx(x, y + 1)] + u_old[idx(x, y - 1)]);

  return u_new[idx(x, y)] * p.dx * p.dx; // Return the updated value scaled by dx^2
}

// Function to apply the stencil across the grid
double apply_stencil(double* u_new, double* u_old, grid g, parameters p) {
  // TODO: Create ranges for x and y indices

  // TODO: Create Cartesian product of x and y indices
  
  // Use parallel execution to apply the stencil and accumulate results

  return 0;
}

// Function to initialize the grid with initial conditions
void initial_condition(double* u_new, double* u_old, long n) {
  // Fill the old and new grid arrays with zeros in parallel
  std::fill_n(std::execution::par, u_old, n, 0.0);
  std::fill_n(std::execution::par, u_new, n, 0.0);
}

// Evolve the solution of the interior part of the domain
// which does not depend on data from neighboring ranks
double inner(double *u_new, double *u_old, parameters p) {
  // Define the grid for the interior points, excluding boundaries
  grid g{.x_begin = 2, .x_end = p.nx, .y_begin = 1, .y_end = p.ny - 1};
  
  // Apply the stencil to the interior grid points and return the result
  return apply_stencil(u_new, u_old, g, p);
}

// Evolve the solution of the part of the domain that 
// depends on data from the previous MPI rank (rank - 1)
double prev(double *u_new, double *u_old, parameters p) {

  // Check if the current rank is greater than 0
  if (p.rank > 0) {
    // TODO: Send the bottom boundary cells to the previous rank
    
    // TODO: Receive the top boundary cells from the previous rank
  }
  
  // Define the grid for the boundary points that depend on the previous rank
  grid g{.x_begin = 1, .x_end = 2, .y_begin= 1, .y_end = p.ny - 1};
  
  // Apply the stencil to the boundary grid points and return the result
  return apply_stencil(u_new, u_old, g, p);
}


// Evolve the solution of the part of the domain that 
// depends on data from the next MPI rank (rank + 1)
double next(double *u_new, double *u_old, parameters p) {
  // Check if the current rank is less than the last rank
  if (p.rank < p.nranks - 1) {
    // TODO: Receive the bottom boundary cells from the next rank
    
    // TODO: Send the top boundary cells to the next rank

  }
  
  // Define the grid for the boundary points that depend on the next rank
  grid g{.x_begin = p.nx, .x_end = p.nx + 1, .y_begin = 1, .y_end = p.ny - 1};
  
  // Apply the stencil to the boundary grid points and return the result
  return apply_stencil(u_new, u_old, g, p);
}
