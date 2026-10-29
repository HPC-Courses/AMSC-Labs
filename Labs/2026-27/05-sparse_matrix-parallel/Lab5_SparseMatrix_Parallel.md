# Laboratory 05 — Sparse matrices and parallel algorithms

### 30/10/2026

This lab puts into practice the last part of the lecture on *The Standard Library*: the algorithms and their parallel versions (slides 78–95). Keep the slides at hand: as in [Lab 3](../03-classes-inheritance/Lab3_Classes_Inheritance.md) and [Lab 4](../04-class_templates-stl/Lab4_ClassTemplates_STL.md), the theory is only recalled in a few lines at the start of each section.

We start from where Lab 4 stopped: we finish the sparse matrix, whose compressed format already needs two algorithms, `std::sort` and `std::lower_bound`. Then we run some algorithms in parallel by adding a single argument, and we see what can go wrong.

Each exercise has an `assignment`, to be completed, and a `solution`. The ✅ **Checkpoint** at the end of each section shows the output you should get. Whatever you do not finish in class is listed in the [Homework](#homework), together with a parallel matrix-vector product and some exercises on the algorithms.

## Outline

- [0. Setup](#0-setup)
- [1. Back to the sparse matrix](#1-back-to-the-sparse-matrix)
- [2. Parallel algorithms](#2-parallel-algorithms)
- [Homework](#homework)

---

## 0. Setup

Update the repository and move to the directory of this lab, inside the container:
```bash
cd ~/shared-folder/AMSC-Labs
git pull
cd Labs/2026-27/05-sparse_matrix-parallel
ls algorithms parallel
```
In section 1 we go back to the sparse matrix of Lab 4, in `../04-class_templates-stl/sparse_matrix/assignment`, where you left it.

The parallel algorithms of section 2 and of the homework work **inside the container** (g++ and the library TBB). They do not compile with the Apple clang of macOS. Check how many cores the container can use:
```bash
nproc
```
If it prints 1 or 2, give more CPUs to Docker (Docker Desktop: *Settings → Resources*), otherwise you will see no speedup.

---

## 1. Back to the sparse matrix

In Lab 4 we wrote the interface `SparseMatrix<T>` (Step 1) and started the uncompressed format `MapMatrix<T, Map>` (Step 2). Today we finish Step 2, then we do Step 3 and Step 4.
```bash
cd ../04-class_templates-stl/sparse_matrix/assignment
make run
```

### Step 2 — Finish the `MapMatrix`

Complete the tasks of [Lab 4, Step 2](../04-class_templates-stl/Lab4_ClassTemplates_STL.md#step-2--the-uncompressed-format-a-map-per-row) that you have left: `fill_tridiagonal` in `main.cpp`, then `nnz()`, the read and the write access, `vmult(x)` and `print_entries(os)` in `map_matrix.hpp`. Remember:
- inside `MapMatrix`, write `this->nrows()`, not `nrows()`: the base class depends on `T`, and the compiler does not look into it until `T` is known (slide 19);
- the read access, `T operator()(i, j) const`, must **not** insert anything: use `find`, not `[]`;
- the write access must check `i` and `j` **before** `rows_[i][j]`, and throw `std::out_of_range`;
- `vmult` throws `std::invalid_argument` if the size of `x` is wrong, then loops over the rows, and over the entries of each row with `for (const auto &[j, a_ij] : rows_[i])`.

Then answer the two questions of Lab 4: who should have stopped the `heap-buffer-overflow` of `A(5, 0)`? Why does `cA(0, 4)` not change `nnz`, while `A(4, 0)` does?

✅ **Checkpoint.**
```
Step 2: MapMatrix
nrows: 5 | ncols: 5 | nnz: 13
0, 0, -2
0, 1, 1
1, 0, 1
1, 1, -2
1, 2, 1
...
4, 3, 1
4, 4, -2
vmult: PASSED
cA(0, 4) = 0, nnz = 13
A(4, 0) = 0, nnz = 14
A(5, 0) = 1.: MapMatrix: index out of range
```

<details>
<summary>Answers</summary>

- `A(5, 0)`: the write access, which must check the indices and throw `std::out_of_range`. Without the check, `rows_[5]` reads outside the vector `rows_`.
- `A(4, 0)`: `A` is not `const`, so `A(4, 0)` calls the non-const `operator()`, the write access, which inserts the entry `(4, 0)` with value 0: the trap of `m[key]` in Lab 4, section 1. `cA` is a `const` reference, so only the const version can be called. To prevent the mistake, the class could give a different name to the write access, e.g. `A.insert(i, j, value)`, and keep `operator()` for reading only.

</details>

### Step 3 — `std::map` or `std::unordered_map`?

Changing the container of the rows takes one word: the second template argument, `MapMatrix<double, std::unordered_map>`. Remember that `Map` is a **template**, not a type: you write `std::unordered_map`, not `std::unordered_map<std::size_t, double>`.

**Your tasks**:
1. In `main`, step 3: build `B`, a 5×5 `MapMatrix<double, std::unordered_map>`, then fill it, print it and check it as `A`. In which order are the entries of each row printed? Is `vmult` still correct?
2. At the end of `main`, run the benchmark with `std::unordered_map` too, then `make benchmark`. The benchmark fills a matrix with $n = 5\cdot10^5$ rows in a random order, as in a finite element code, and times 10 products. Which `Map` is faster to fill? Which one is faster in `vmult`?

✅ **Checkpoint.**
```
Step 3: unordered_map
nrows: 5 | ncols: 5 | nnz: 13
0, 1, 1
0, 0, -2
1, 2, 1
...
vmult: PASSED
```
The times of the benchmark are in the table of Step 4.

<details>
<summary>Answers</summary>

The `std::unordered_map` prints the entries of each row in the order of its hash table; `vmult` does not care about the order. Filling is slower with the `unordered_map`: with 3 entries per row, $O(\log 3)$ and $O(1)$ are the same thing, and what counts is the constant: computing the hash, and the array of buckets that each `unordered_map` allocates. Both maps are made of nodes scattered on the heap, so both are slow in `vmult`.

</details>

### Step 4 — The compressed format: COO

The **COO** (*COOrdinate*) format stores the list of the entries `(row, col, value)` in a single `std::vector<Entry>`, sorted by row and then by column:
```
entries_ = { (0,0,-2), (0,1,1), (1,0,1), (1,1,-2), (1,2,1), (2,1,1), ..., (4,4,-2) }
```
A `CooMatrix` can only be built from a `MapMatrix`, and no entry can be added afterwards. Its constructor is a **member function template**: it has its own template parameter `Map`, so that it accepts a `MapMatrix<T, Map>` with any `Map`.

Two algorithms do most of the work (slide 80). Both need to know what *"`a` comes before `b`"* means for two entries: this is the function `by_position(a, b)`, already written in `coo_matrix.hpp`. It compares the rows; if the rows are equal, it compares the columns.

| Algorithm | What it does |
|---|---|
| `std::sort(begin, end, by_position)` | sorts the entries, from the first to the last according to `by_position` |
| `std::lower_bound(begin, end, key, by_position)` | in a **sorted** range, returns (an iterator to) the first entry that does not come before `key`, in $O(\log n)$: a binary search. If `key` is in the range, it is that entry; if it is not, it is the entry that follows it, or `end` |

We write tasks 1 and 3, the two algorithms, together. Task 2 is almost the same as in `MapMatrix`: write it on your own.

**Your tasks**, in `coo_matrix.hpp`:
1. Write the body of the constructor: copy the entries of `A` into `entries_`, row by row (`A.row(i)` returns the map of row `i`), then sort them with `std::sort` and `by_position`. Why is the sort needed, if `MapMatrix` uses a `std::map`? And if it uses a `std::unordered_map`?
2. Implement `nnz()`, `vmult(x)` (a single loop on `entries_`) and `print_entries(os)`.
3. Implement the read access with `std::lower_bound`. Build the entry you are looking for, `const Entry key{i, j, T{}};`, and look for it. Careful: `lower_bound` does not tell you if it found `(i, j)`. Check that the iterator is not `end()`, and that its `row` and `col` are really `i` and `j`, before returning its value.
4. Uncomment step 4 in `main`, and the COO lines in `benchmark`, then `make run` and `make benchmark`. Note `const CooMatrix C{B};`, with no `<double>`: how does the compiler know `T`?

✅ **Checkpoint.** The tests:
```
Step 4: COO
nrows: 5 | ncols: 5 | nnz: 13
0, 0, -2
0, 1, 1
1, 0, 1
...
vmult: PASSED
C(1, 2) = 1, C(0, 4) = 0
```
and the benchmark, `make benchmark` (ms, measured on a laptop):

| $n = 5\cdot10^5$ | fill | compress to COO | `vmult` |
|---|---|---|---|
| `MapMatrix<double, std::map>` | 50 | | 13 |
| `MapMatrix<double, std::unordered_map>` | 87 | | 9.5 |
| `CooMatrix<double>` | | 17 (from `map`), 45 (from `unordered_map`) | **5** |

Solution: [`sparse_matrix/solution`](../04-class_templates-stl/sparse_matrix/solution).

<details>
<summary>Answers</summary>

- Sort: with a `std::map` the columns of each row are already sorted, and the rows are visited in order, so `entries_` comes out sorted. With a `std::unordered_map` it does not. `std::lower_bound` needs a range sorted with the same comparison it uses: on a range that is not sorted it returns a wrong result, without any error (try: without the sort, `C(1, 2)` is 0, even if `vmult` passes). Sorting a vector that is already sorted is cheap.
- Read access: `lower_bound` returns the first entry that does not come before `(i, j)`. For `C(0, 4)`, which is not stored, it returns `(1, 0, 1)`: without the check on `row` and `col`, `C(0, 4)` would be 1.
- `CooMatrix C{B}`: *class template argument deduction* (CTAD, Lab 4). `T` appears in the parameter of the constructor, `const MapMatrix<T, Map> &`: from `B`, a `MapMatrix<double, std::unordered_map>`, the compiler deduces `T = double` and `Map = std::unordered_map`.
- Benchmark: the entries of the `CooMatrix` are in one contiguous vector, in the order in which `vmult` reads them, so its `vmult` is the fastest, for the same reason as the `vector` in the benchmark of Lab 4, section 2.

</details>

---

## 2. Parallel algorithms

The algorithms of the standard library (Lab 2, Lab 4, slides 78–91) work on a range `[begin, end)`, and most of them take a callable that says *what* to do with each element. Many of them have a second version, which takes an **execution policy** as the first argument (slides 92–93):
```cpp
#include <execution>

std::sort(v.begin(), v.end());                         // as usual
std::sort(std::execution::par, v.begin(), v.end());    // may use several threads
```

| Policy | Meaning |
|---|---|
| `std::execution::seq` | one thread, one element after the other: as with no policy |
| `std::execution::par` | the elements may be divided among several threads |
| `std::execution::par_unseq` | several threads, and inside each thread the compiler may also use vector instructions |

The policy is a *permission*, not an order: the library may still run the algorithm on one thread. With g++, the threads are created by the library **TBB** (*Threading Building Blocks*), so the program must be linked with `-ltbb`.

This is one more reason to write an algorithm instead of a `for` loop: to run a loop in parallel you have to rewrite it, to run an algorithm in parallel you add one argument.

### How much faster?

[`parallel/01-policies/assignment.cpp`](parallel/01-policies/assignment.cpp) runs four algorithms on $10^7$ random numbers, three times: with `seq`, `par` and `par_unseq`. The policy is passed to the function `run` as the parameter `policy`, but the algorithms do not use it yet.
```bash
cd ../../../05-sparse_matrix-parallel/parallel/01-policies
g++ -Wall -std=c++20 -O3 assignment.cpp -o assignment -ltbb
./assignment
```

**Your tasks**:
1. Run it: the three policies take the same time. Then add `policy` as the first argument of the four algorithms in `run`, compile and run again.
2. Which algorithms are faster with `par`? How much faster, compared to the number of threads printed at the start?
3. Look at the `sum of the squares`, with 17 digits: is it the same for the three policies? Run the program again: does it change?
4. Compile without `-ltbb`: what happens?

✅ **Checkpoint.** The times depend on your machine, but the pattern should look like this (in the container, on a laptop with 8 cores):

| ms | `seq` | `par` | `par_unseq` |
|---|---|---|---|
| `y = 2 x + 1` | 3.8 | 4.0 | 3.6 |
| `y = sin(x) exp(-x/10)` | 312 | **60** | **61** |
| sum of the squares | 9.0 | 4.1 | 2.5 |
| sort | 900 | **194** | **180** |

and the sums:
```
seq        sum = 333434181.77666193
par        sum = 333434181.77667522
par_unseq  sum = 333434181.77667737
```
Solution: [`solution.cpp`](parallel/01-policies/solution.cpp).

<details>
<summary>Answers</summary>

- 2: the work is divided among the threads only if there is enough work to divide. `sin(x) exp(-x/10)` and the sort do many operations for each number, and they are about 5 times faster with 8 threads. `2 x + 1` does one multiplication and one addition for each number: the time is spent reading `x` and writing `y` in memory, and the memory is shared by all the cores. More threads do not make the memory faster. It is the same lesson as the container benchmark of Lab 4: the time also depends on how the data reach the CPU.
- 3: with `seq` the numbers are summed from the first to the last. With `par` each thread sums its own part, and then the partial sums are added. The order of the additions is different, and with floating-point numbers $(a + b) + c$ is not always equal to $a + (b + c)$: the last digits change. How the work is divided can change from one run to the next, so the result can change too. This is why parallel algorithms use `std::reduce` and `std::transform_reduce`, which are allowed to sum in any order, and not `std::accumulate`, which by definition sums from left to right (slide 95).
- 4: the linker fails with a long list of `undefined reference to tbb::...`: the code of the parallel algorithms calls functions of TBB, which are not in the executable.

</details>

### Now break it: a data race

A parallel algorithm calls your lambda from several threads at the same time. If two calls change **the same variable**, the result is wrong: this is a **data race**. The policy does not protect you: avoiding data races is your responsibility (slide 94: *each invocation must write a distinct element without modifying shared mutable state*).

[`parallel/02-data_race/data_race.cpp`](parallel/02-data_race/data_race.cpp) counts the even numbers among $0, \dots, 10^7 - 1$ in three ways: a loop, a `std::for_each(std::execution::par, ...)` that increments a shared counter, and `std::count_if(std::execution::par, ...)`. We run it together, three times; you can run it too:
```bash
cd ../02-data_race
g++ -Wall -std=c++20 -O3 data_race.cpp -o data_race -ltbb
./data_race
```
Why is the result of `for_each` wrong, and why is it different every time? Why is `count_if` right?

✅ **Checkpoint.** (The wrong number changes at every run.)
```
Expected:            5000000
Loop:                5000000
for_each, par:       3887444
count_if, par:       5000000
```

<details>
<summary>Answers</summary>

`++count` is not one operation, but three: read `count`, add 1, write `count`. If two threads read the same value, say 41, both write 42: one increment is lost. How many are lost depends on how the threads happen to run, so it changes at every run. `count_if` does not share anything: each thread counts its own part in its own variable, and the library sums the partial counts at the end. **When you want to combine all the elements into one result** (a count, a sum, a maximum), **use the algorithm that does it** (`count_if`, `reduce`, `transform_reduce`, `max_element`), not a `for_each` that writes into a shared variable.

</details>

---

## Homework

What is left from the lab, plus some extra exercises. Each one has a difficulty label: **[Easy]**, **[Intermediate]** or **[Hard]**.
- **Section 1**, Steps 2, 3 and 4, if you did not finish them in class.
- **[Easy]** — **Now break it**, at the end of [Lab 4, Step 4](../04-class_templates-stl/Lab4_ClassTemplates_STL.md#step-4--the-compressed-format-coo-if-you-have-time): fill the matrix of the benchmark in order. What happens to the `vmult` of the `MapMatrix`, and to the one of the `CooMatrix`?
- **[Easy]** — **A parallel matrix-vector product**: the [exercise below](#exercise-a-parallel-matrix-vector-product).
- **[Easy]** — **From a loop to an algorithm**: the [exercise below](#exercise-from-a-loop-to-an-algorithm).
- **[Easy]** — **How much work is enough?** In `parallel/01-policies`, replace `2. * v + 1.` with `std::sqrt(v)`, then with `std::pow(v, 1.5)`. When does `par` start to be faster than `seq`?
- **[Easy]** — **`transform` instead of `for_each`.** Write the `vmult_parallel` of the [parallel matrix-vector product](#exercise-a-parallel-matrix-vector-product) with `std::transform(std::execution::par, ...)`, which writes `row_times_x(i, x)` into `y`. Why is it even safer than `for_each`?
- **[Intermediate]** — **A parallel `MapMatrix`.** Add a `vmult_parallel` to the `MapMatrix` of section 1 (rows divided among the threads, as in the CSR), and add it to the benchmark. Is it correct? Is it faster than the sequential one? Than the CSR?
- **[Intermediate]** — **Break the COO.** Write the wrong `vmult_parallel` of task 4 of the [parallel matrix-vector product](#exercise-a-parallel-matrix-vector-product) for the `CooMatrix`, with `for_each(std::execution::par, ...)` on `entries_`. Compare its result with `vmult` on a matrix with $n = 2\cdot10^6$ rows: how many entries of `y` are wrong? Run it several times. Does the result prove that the code is correct?
- **[Hard]** — **The CSR of Lab 4.** If you wrote your own `CsrMatrix` as homework of Lab 4, compare it with the one of the parallel matrix-vector product. Ours is built only from a `MapMatrix<T>` with `std::map`: what would you change to build it from a `MapMatrix<T, std::unordered_map>`?

### Exercise: a parallel matrix-vector product

**[Easy]**

The **CSR** (*Compressed Sparse Row*) format was the [homework of Lab 4](../04-class_templates-stl/Lab4_ClassTemplates_STL.md#exercise-the-csr-format): here it is already written, in [`parallel/03-parallel_vmult/assignment/csr_matrix.hpp`](parallel/03-parallel_vmult/assignment/csr_matrix.hpp). It stores the entries row by row, in three vectors. For the 5×5 matrix:
```
values_  = { -2, 1,   1, -2, 1,   1, -2, 1,   1, -2, 1,   1, -2 }
col_idx_ = {  0, 1,   0,  1, 2,   1,  2, 3,   2,  3, 4,   3,  4 }
row_ptr_ = {  0,      2,          5,          8,          11,     13 }
```
The entries of row `i` are in the positions from `row_ptr_[i]` to `row_ptr_[i+1] - 1`. The private method `row_times_x(i, x)` computes the product of row `i` with `x`, that is the entry `y[i]`, and `vmult` is a loop over the rows:
```cpp
for (std::size_t i = 0; i < this->nrows(); ++i)
  y[i] = row_times_x(i, x);
```
The rows are **independent**: each one writes only its own `y[i]`. We can compute them in parallel.

How do we turn a loop over the indices `i` into an algorithm? An algorithm needs a range of elements, and there is no container with the numbers $0, 1, \dots, n-1$, so we build one (slide 94). The constructor of `CsrMatrix` fills `rows_` with these numbers using `std::iota`, once. Then:
```cpp
std::for_each(std::execution::par, rows_.begin(), rows_.end(),
              [&](std::size_t i) { /* the body of the loop, for row i */ });
```

From the directory of this lab:
```bash
cd parallel/03-parallel_vmult/assignment
make run
```
The `Makefile` is the one of Lab 4, with `-ltbb`: `make run` runs the tests with AddressSanitizer, `make benchmark` also times `vmult` and `vmult_parallel` on a matrix with $n = 2\cdot10^6$ rows.

**Your tasks**:
1. Write `vmult_parallel` in `csr_matrix.hpp`, with `std::for_each` and `std::execution::par`.
2. Run `make run`, then `make benchmark`. Is `vmult_parallel` 8 times faster? Compare it with the algorithms of section 2.
3. Why does `vmult_parallel` not have the data race of section 2?
4. The COO format of section 1 also has a `vmult` with a single loop, on `entries_`. Would `std::for_each(std::execution::par, entries_.begin(), entries_.end(), ...)` work there?

✅ **Checkpoint.**
```
vmult:          PASSED
vmult_parallel: PASSED

Benchmark, n = 2000000, threads: 8
vmult:          7.4 ms
vmult_parallel: 3.6 ms
same result:    PASSED
```
Solution: [`parallel/03-parallel_vmult/solution`](parallel/03-parallel_vmult/solution).

<details>
<summary>Answers</summary>

- 1: `std::for_each(std::execution::par, rows_.begin(), rows_.end(), [&](std::size_t i) { y[i] = row_times_x(i, x); });`. The lambda captures by reference (`[&]`): it must write into `y`, not into a copy. With `[=]` it does not compile: *assignment of read-only location*.
- 2: about 2 times faster, not 8. As `y = 2 x + 1` in section 2, the product does one multiplication and one addition for each entry, and the time is spent reading `values_`, `col_idx_` and `x` from memory. Parallel algorithms pay off when there is a lot of computation for each byte read from memory.
- 3: each call of the lambda writes only `y[i]`, a different element for each `i`, and only reads `x` and the matrix. No variable is changed by two threads.
- 4: no. The entries of the same row are in different elements of `entries_`, and all of them do `y[e.row] += ...`: two threads can update the same `y[e.row]` at the same time, as `count` in section 2. In the CSR format the work is divided by rows, and each row is handled entirely by one thread.
- Why not `std::views::iota(0, n)` (C++20), instead of storing `rows_`? It needs no memory, but with g++ 13 a `for_each(std::execution::par, ...)` on it compiles and runs **on one thread**, without any warning: the parallel version requires iterators that `iota` does not provide yet. The vector of indices is the solution that works everywhere.

</details>

**What goes in the interface?** `vmult_parallel` is not in the interface `SparseMatrix<T>`: only the CSR has it. Adding it to the base class would oblige every format to have one, and for the COO a correct parallel product is not a one-line change. When you design an interface, put in it only what every derived class can really do.

### Exercise: from a loop to an algorithm

**[Easy]**

The standard library has more than a hundred algorithms (slides 78–91). Why use them instead of a `for` loop?
- the name says what the code does: `count_if` counts, `max_element` finds the largest element;
- they are already written and tested: no off-by-one errors in the indices;
- most of them have a **parallel version** (section 2): the loop has to be rewritten, the algorithm does not.

The algorithms you need:

| Algorithm | Header | What it computes |
|---|---|---|
| `std::accumulate(b, e, init)` | `<numeric>` | `init` + the sum of the elements |
| `std::inner_product(b1, e1, b2, init)` | `<numeric>` | `init` + $\sum_i a_i b_i$, the dot product of two ranges |
| `std::transform(b1, e1, b2, out, f)` | `<algorithm>` | `out[i] = f(a[i], b[i])`, for each `i` (slide 88) |
| `std::transform_reduce(b, e, init, std::plus<double>(), f)` | `<numeric>` | `init` + $\sum_i f(a_i)$: first transforms, then sums (slide 90) |
| `std::count_if(b, e, p)` | `<algorithm>` | how many elements satisfy `p(x)` |
| `std::copy_if(b, e, out, p)` | `<algorithm>` | copies to `out` the elements that satisfy `p(x)` (slides 85–86) |
| `std::max_element(b, e, less)` | `<algorithm>` | an iterator to the largest element; `less(a, b)` answers *"is `a` smaller than `b`?"* |

> **The output must have room.** An algorithm that writes, such as `transform` or `copy_if`, writes to `out`, `out + 1`, ... without ever calling `push_back`. Either the output vector already has the right size, `std::vector<double> z(x.size());`, or you write through `std::back_inserter(v)`, which calls `v.push_back` for you (Lab 4).

In [`algorithms/01-loops_to_algorithms/assignment.cpp`](algorithms/01-loops_to_algorithms/assignment.cpp) each exercise shows a loop, in a comment: write the same computation with the algorithm that is suggested. The functions in `check.hpp` tell you if the result is correct. From the directory of this lab:
```bash
cd algorithms/01-loops_to_algorithms
g++ -Wall -std=c++20 assignment.cpp -o assignment && ./assignment
```
(The warning *unused variable 'a'* disappears when you solve exercise 3.)

**Your tasks**:
1. The sum of the entries of `x`, with `std::accumulate`.
2. The dot product of `x` and `y`, with `std::inner_product`.
3. $z = a x + y$, with the `std::transform` that takes two vectors.
4. The 1-norm of `x`, with `std::transform_reduce`.
5. The entries of `x` larger than 2.5 in absolute value: count them with `std::count_if`, reserve the space, copy them with `std::copy_if`.
6. **[Intermediate]** The entry of `x` with the largest absolute value, and its position, with `std::max_element`.

✅ **Checkpoint.**
```
Exercise 1: PASSED
Exercise 2: PASSED
Exercise 3: PASSED
Exercise 4: PASSED
Exercise 5: PASSED
...
```
Solution: [`solution.cpp`](algorithms/01-loops_to_algorithms/solution.cpp).

<details>
<summary>Answers</summary>

- 1: `std::accumulate(x.begin(), x.end(), 0.)`. The type of the initial value is the type of the sum: with `0`, an `int`, every partial sum is converted to `int`, `-1.5` is truncated, and the result is 8 instead of 8.5. The same holds for `inner_product` and `transform_reduce`.
- 3: the lambda must use `a`, so it captures it: `[a](double xi, double yi) { return a * xi + yi; }`.
- 5: write the condition once, `auto is_large = [](double v) { return std::abs(v) > 2.5; };`, and pass it to both algorithms. `large.reserve(how_many)` allocates the memory, but `large` is still empty: `copy_if` must write through `std::back_inserter(large)`. With `large.begin()` it would write outside the vector.
- 6: `max_element` returns an iterator `it`: the value is `*it`, the position is `std::distance(x.begin(), it)`.

</details>

<details>
<summary>Answers (homework)</summary>

- Now break it: the `vmult` of the `MapMatrix` becomes about 4 times faster: filled in order, the nodes of the maps are allocated one after the other, and they happen to be contiguous in memory. The `CooMatrix` does not change: compressing puts the entries in order, whatever happened before (Lab 4, answers of Step 4).
- How much work: with `std::sqrt` the parallel version starts to be faster; with `std::pow` it is clearly faster, as `sin(x) exp(-x/10)`. The more operations for each number read from memory, the larger the speedup.
- `transform`: `std::transform(std::execution::par, rows_.begin(), rows_.end(), y.begin(), [&](std::size_t i) { return row_times_x(i, x); });`. The lambda does not write anything: it only returns a value, and the algorithm stores it in its place in `y`. There is no variable to share by mistake.
- `MapMatrix`: it is correct, since each row writes only its own `y[i]`, but it is slower than the CSR: each thread still follows the pointers of the nodes of the maps, scattered on the heap.
- Break the COO: most probably **no entry is wrong** (in the container, 0 wrong entries in 5 runs). The data race is there, but it rarely happens: each thread gets a block of consecutive entries, so two threads can share only the row at the border of their blocks, and they must update it at the same instant. This is the most dangerous kind of bug: the tests pass, and the wrong result appears months later, on another machine or with another matrix. A program with a data race is wrong even when its output is right.
- CSR from an `unordered_map`: the columns of each row are not sorted. Copy each row in a `std::vector` of pairs `(column, value)`, sort it by column, then append it to `col_idx_` and `values_` (this is the solution of Lab 4, [`csr_matrix.hpp`](../04-class_templates-stl/sparse_matrix/solution/csr_matrix.hpp)). The read access needs the columns sorted only if it uses a binary search, as the solution of Lab 4 does; our loop works anyway.

</details>

**Further reading**:
- in [`AMSC-CodeExamples/Examples/src/STL`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/STL): [`Reduce`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/STL/Reduce) (the order of the sums in parallel, slide 90) and the other examples on the algorithms;
- **Thrust** (slides 96–98): the same ideas, `transform`, `reduce`, `sort`, on a GPU, with `thrust::device_vector` instead of `std::vector`. It needs an NVIDIA GPU and the compiler `nvcc`: not in the container, just to read.
