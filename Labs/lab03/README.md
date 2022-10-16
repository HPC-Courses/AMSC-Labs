# Sparse Matrix
A sparse matrix or sparse array is a matrix in which most of the elements are zero. Sparse matrices are often used in numberical analysis, especially for finite element methods.

## Step 1 - The abstract class
There are many different ways to implement as sparse matrix. In this laboratory we will show a couple of naive implementations that rely on the STL. For this reason our starting point is defining an abstract class where we declare all the methods that we want to have. Namely:

* a getter for number of rows
* a getter for number of cols
* a getter for number of number of non-zero
* a print function
* a vmult method that implements vector multiplication
* an access method to read and write the elements of the matrix


## Step 2 - Matrix made with maps
After having defined our interface we can really implement our sparse matrix class. In this step the aim is to code a `MapMatrix` sparse matrix, in which each row is represented by a map and all the maps are collected in a `std::vector`. For debugging we suggest the following compiler flags
```
g++ hint.cpp -std=c++20 -O0 -Wall -Wextra -pedantic -fsanitize=address
```

## Step 3 - COO Matrix
Now we want to implement the more famous Coordinate list format. COO stores a list of (row, column, value) tuples. Ideally, the entries are sorted first by row index and then by column index, to improve random access times. To find the elements in a sorted vector exploit `std::lower_bound`.

## Step 4 - Benchmark and test code
Finally we want to implement some utilities to benchmark the performances of our classes and test if the implementation is correct. Namely we implement:

* a function to fill a matrix and given the final size $n$. Namely it puts on the main diagonal -2 and 1 on the diagonal above and below the main one
* a function `bool eq(const Vector &lhs, const Vector &rhs)` that checks if two vectors are equal

Then we will put a `CooMatrix` and a `MapMatrix` in a vector and loop over them checking:
* `matrix.print(std::cout)` effectively prints the expected tridiagonal matrix
* $A [0, 1, 2, ..., n] = [1, 0, ..., 0, -n]$
* the elapsed time by each operation

## Step 5 - Check different types of maps
Check what happens if instead of using a `std::map` we use `std::unordered_map`. Why does this happen?
