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
#include <ranges>       // For range-based operations and views
#include <algorithm>    // For standard algorithms like std::fill_n
#include <numeric>      // For numeric operations like std::transform_reduce
#include <execution>     // For parallel execution policies (e.g., std::execution::par)
#include <thread>       // For multi-threading support
#include <atomic>       // For atomic operations and types
#include <barrier>      // For synchronization barriers in multi-threading

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

  // Initialize the MPI environment with support for multi-threading.
  // 'MPI_Init_thread' initializes the MPI environment and checks for the level of thread support.
  // 'MPI_THREAD_MULTIPLE' requests that multiple threads can make MPI calls concurrently.
  // The actual level of thread support provided is returned in 'mt'.
  MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &mt);

  // Check if the level of thread support is not equal to MPI_THREAD_MULTIPLE.
  // If the support level is lower than requested, output an error message and terminate the program.
  if (mt != MPI_THREAD_MULTIPLE) {
      std::cerr << "MPI cannot be called from multiple host threads" << std::endl;
      std::terminate(); // Exit the program if the required threading support is not available
  }

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
      
  // Create an atomic variable to store the energy, which can be safely modified by multiple threads.
  // The atomic type ensures that updates to the energy variable are thread-safe, preventing data races.
  std::atomic<double> energy = 0.;

  // Create a barrier for synchronizing three threads.
  // The barrier will ensure that all threads reach a certain point in the code before any of them can proceed.
  std::barrier bar(3);
  


  // Create a new thread named 'thread_prev' to perform the "prev" computation.
  // The thread captures the parameters 'p', pointers to the new and old temperature arrays ('u_new' and 'u_old'),
  // a reference to the atomic 'energy' variable, and the barrier 'bar' for synchronization.
  std::thread thread_prev([p, u_new = u_new.data(), u_old = u_old.data(), 
                             &energy, &bar]() mutable { // NOTE: the lambda mutates its captures
        // Loop over the number of time-steps specified in the parameters.
        for (long it = 0; it < p.nit(); ++it) {
            // Call the 'prev' function to compute the next state of the temperature field,
            // and accumulate the energy contribution into the atomic 'energy' variable.
            energy += prev(u_new, u_old, p);
            
            // Synchronize this thread with other threads using the barrier.
            // The first call ensures that all threads have completed their computations for this iteration.
            bar.arrive_and_wait();
            
            // The second call is necessary to synchronize with the "inner" thread,
            // ensuring that all threads reach this point before proceeding.
            bar.arrive_and_wait();
            
            // Swap the pointers of 'u_new' and 'u_old' to prepare for the next iteration.
            // This allows the next computation to use the updated temperature values.
            std::swap(u_new, u_old);
        }
    });
    
 
  // Create a new thread named 'thread_next' to perform the "next" computation.
  // The thread captures the parameters 'p', pointers to the new and old temperature arrays ('u_new' and 'u_old'),
  // a reference to the atomic 'energy' variable, and the barrier 'bar' for synchronization.
  std::thread thread_next([p, u_new = u_new.data(), u_old = u_old.data(), 
                             &energy, &bar]() mutable {
        // Loop over the number of iterations specified in the parameters.
        for (long it = 0; it < p.nit(); ++it) {
            // Call the 'next' function to compute the next state of the temperature field,
            // and accumulate the energy contribution into the atomic 'energy' variable.
            energy += next(u_new, u_old, p);

            // Synchronize this thread with other threads using the barrier.
            // The first call ensures that all threads have completed their computations for this iteration.
            bar.arrive_and_wait();

            // The second call is necessary to synchronize with the "inner" thread,
            // ensuring that all threads reach this point before proceeding.
            bar.arrive_and_wait();

            // Swap the pointers of 'u_new' and 'u_old' to prepare for the next iteration.
            // This allows the next computation to use the updated temperature values.
            std::swap(u_new, u_old);
        }
    });
    

  // Create a new thread named 'thread_inner' to perform the "inner" computation,
  // which includes the MPI reduction and I/O operations.
  // The thread captures the parameters 'p', pointers to the new and old temperature arrays ('u_new' and 'u_old'),
  // a reference to the atomic 'energy' variable, and the barrier 'bar' for synchronization.
  std::thread thread_inner([p, u_new = u_new.data(), u_old = u_old.data(), 
                              &energy, &bar]() mutable {
      // Loop over the number of iterations specified in the parameters.
      for (long it = 0; it < p.nit(); ++it) {
          // Call the 'inner' function to compute the next state of the temperature field,
          // and accumulate the energy contribution into the atomic 'energy' variable.
          energy += inner(u_new, u_old, p);

          // Synchronize this thread with other threads using the barrier.
          // This ensures that all threads have completed their computations for this iteration.
          bar.arrive_and_wait();
      
          // Only the "inner" thread performs the MPI reduction and I/O operations.
          // The MPI_Reduce function combines the energy values from all ranks into the rank 0 process.
          // If the current rank is 0, it uses MPI_IN_PLACE to update the energy directly.
          // Otherwise, it sends the energy value to rank 0.
          MPI_Reduce(p.rank == 0 ? MPI_IN_PLACE : &energy, &energy, 1, MPI_DOUBLE, MPI_SUM, 0,
                     MPI_COMM_WORLD);

          // If the current rank is 0 and the current iteration is a multiple of the output frequency,
          // print the current energy value to standard error.
          if (p.rank == 0 && it % p.nout() == 0) {
              std::cerr << "E(t=" << it * p.dt << ") = " << energy << std::endl;
          }

          // Swap the pointers of 'u_new' and 'u_old' to prepare for the next iteration.
          std::swap(u_new, u_old);
          
          // Reset the energy variable to 0 for the next iteration.
          energy = 0;
      
          // Synchronize all threads again to ensure they are ready for the next iteration.
          bar.arrive_and_wait();
      }
  });
  
  // Join all threads to ensure synchronization

  thread_prev.join();  // Wait for the 'thread_prev' to finish its execution.
  thread_next.join();  // Wait for the 'thread_next' to finish its execution.
  thread_inner.join();  // Wait for the 'thread_inner' to finish its execution.
  

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


  // Write output to file, writing both header information and the computed data from each rank to a file named "output". Use non-blocking I/O and ensure synchronization
  
  // Declare an MPI_File object to handle file operations
  MPI_File f;

  // Open a file named "output" for writing. The file will be created if it doesn't exist.
  // MPI_COMM_WORLD is the communicator that includes all processes.
  // MPI_MODE_CREATE indicates that the file should be created if it does not exist.
  // MPI_MODE_WRONLY indicates that the file is opened for writing only.
  // MPI_INFO_NULL indicates that no special file options are provided.
  MPI_File_open(MPI_COMM_WORLD, "output", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f);

  // Calculate the size of the header in bytes.
  // The header consists of two long integers (for total dimensions) and one double (for time).
  auto header_bytes = 2 * sizeof(long) + sizeof(double);

  // Calculate the number of values each rank will write to the file.
  // This is the product of the local grid dimensions (nx and ny).
  auto values_per_rank = p.nx * p.ny;

  // Calculate the number of bytes required for the values written by each rank.
  auto values_bytes_per_rank = values_per_rank * sizeof(double);

  // Set the total size of the file to accommodate the header and the data from all ranks.
  // The total size is the sum of the header size and the size of the data from all ranks.
  MPI_File_set_size(f, header_bytes + values_bytes_per_rank * p.nranks);

  // Create an array of MPI_Request objects to handle non-blocking I/O operations.
  // Initialize the requests to null.
  MPI_Request req[3] = {MPI_REQUEST_NULL, MPI_REQUEST_NULL, MPI_REQUEST_NULL};

  // Only the rank 0 process will write the header information to the file.
  if (p.rank == 0) {
      // Prepare an array to hold the total dimensions of the grid.
      long total[2] = {p.nx * p.nranks, p.ny}; // Total grid size across all ranks

      // Calculate the total simulation time based on the number of iterations and time step.
      double time = p.nit() * p.dt;

      // Perform a non-blocking write operation to write the total dimensions to the file at the beginning (offset 0).
      // The data type is MPI_UINT64_T for the long integers.
      MPI_File_iwrite_at(f, 0, total, 2, MPI_UINT64_T, &req[1]);

      // Perform a non-blocking write operation to write the simulation time to the file after the total dimensions.
      // The data type is MPI_DOUBLE for the time value.
      MPI_File_iwrite_at(f, 2 * sizeof(long), &time, 1, MPI_DOUBLE, &req[2]);
  }

  // Calculate the offset for the values that this rank will write to the file.
  // The offset is determined by the header size plus the number of values written by previous ranks.
  auto values_offset = header_bytes + p.rank * values_bytes_per_rank;

  // Perform a non-blocking write operation to write the local temperature data (u_new) to the file.
  // The data is written starting from the calculated offset.
  MPI_File_iwrite_at(f, values_offset, u_new.data() + p.ny, values_per_rank, MPI_DOUBLE, &req[0]);

  // Wait for all non-blocking I/O operations to complete.
  // If the current rank is 0, wait for all three requests; otherwise, wait for just the one request.
  MPI_Waitall(p.rank == 0 ? 3 : 1, req, MPI_STATUSES_IGNORE);

  // Close the file after all write operations are complete.
  MPI_File_close(&f);


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
  
  // Boundary conditions for the ranks at the ends of the domain
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
  // Create ranges for x and y indices
  auto xs = std::views::iota(g.x_begin, g.x_end);
  auto ys = std::views::iota(g.y_begin, g.y_end);
  auto ids = std::views::cartesian_product(xs, ys); // Create Cartesian product of x and y indices
  
  // Use parallel execution to apply the stencil and accumulate results
  return std::transform_reduce(
    std::execution::par, ids.begin(), ids.end(), 
    0., std::plus{}, [u_new, u_old, p](auto idx) {
      auto [x, y] = idx; // Decompose index tuple
      return stencil(u_new, u_old, x, y, p); // Apply stencil for each grid point
  });
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
  // Check if the current rank is greater than 0 (not the first rank)
  if (p.rank > 0) {
    // Send the bottom boundary cells to the previous rank
    MPI_Send(u_old + p.ny, p.ny, MPI_DOUBLE, p.rank - 1, 0, MPI_COMM_WORLD);
    // Receive the top boundary cells from the previous rank
    MPI_Recv(u_old + 0, p.ny, MPI_DOUBLE, p.rank - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
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
    // Receive the bottom boundary cells from the next rank
    MPI_Recv(u_old + (p.nx + 1) * p.ny, p.ny, MPI_DOUBLE, p.rank + 1, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    // Send the top boundary cells to the next rank
    MPI_Send(u_old + p.nx * p.ny, p.ny, MPI_DOUBLE, p.rank + 1, 1, MPI_COMM_WORLD);
  }
  
  // Define the grid for the boundary points that depend on the next rank
  grid g{.x_begin = p.nx, .x_end = p.nx + 1, .y_begin = 1, .y_end = p.ny - 1};
  
  // Apply the stencil to the boundary grid points and return the result
  return apply_stencil(u_new, u_old, g, p);
}
