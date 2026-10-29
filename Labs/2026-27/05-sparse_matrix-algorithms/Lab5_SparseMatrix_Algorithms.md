# Laboratory 05 — Sparse matrices and algorithms

### 30/10/2026

This lab puts into practice the part of the lecture on *The Standard Library* about the algorithms (slides 78–91). Keep the slides at hand: as in [Lab 3](../03-classes-inheritance/Lab3_Classes_Inheritance.md) and [Lab 4](../04-class_templates-stl/Lab4_ClassTemplates_STL.md), the theory is only recalled in a few lines at the start of each section.

We start from where Lab 4 stopped: we finish the sparse matrix, whose compressed format already needs two algorithms, `std::sort` and `std::lower_bound`. Then we rewrite some loops as algorithms.

Each exercise has an `assignment`, to be completed, and a `solution`. The ✅ **Checkpoint** at the end of each section shows the output you should get. Whatever you do not finish in class is listed in the [Homework](#homework).

## Outline

- [0. Setup](#0-setup)
- [1. Back to the sparse matrix](#1-back-to-the-sparse-matrix)
- [2. From a loop to an algorithm](#2-from-a-loop-to-an-algorithm)
- [Homework](#homework)

---

## 0. Setup

Update the repository and move to the directory of this lab, inside the container:
```bash
cd ~/shared-folder/AMSC-Labs
git pull
cd Labs/2026-27/05-sparse_matrix-algorithms
ls algorithms
```
In section 1 we go back to the sparse matrix of Lab 4, in `../04-class_templates-stl/sparse_matrix/assignment`, where you left it.

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

## 2. From a loop to an algorithm

The standard library has more than a hundred algorithms (slides 78–91). They all work on a range `[begin, end)`, and most of them take a callable (Lab 2) that says *what* to do with each element. Why use them instead of a `for` loop?
- the name says what the code does: `count_if` counts, `max_element` finds the largest element;
- they are already written and tested: no off-by-one errors in the indices;
- most of them also have a **parallel version**, which we will see in a later lecture: to run a loop on several threads you have to rewrite it, to run an algorithm you add one argument.

The algorithms you need today:

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

In [`algorithms/01-loops_to_algorithms/assignment.cpp`](algorithms/01-loops_to_algorithms/assignment.cpp) each exercise shows a loop, in a comment: write the same computation with the algorithm that is suggested. The functions in `check.hpp` tell you if the result is correct.
```bash
cd ../../../05-sparse_matrix-algorithms/algorithms/01-loops_to_algorithms
g++ -Wall -std=c++20 assignment.cpp -o assignment && ./assignment
```
(The warning *unused variable 'a'* disappears when you solve exercise 3.)

**Your tasks**:
1. The sum of the entries of `x`, with `std::accumulate`.
2. The dot product of `x` and `y`, with `std::inner_product`.
3. $z = a x + y$, with the `std::transform` that takes two vectors.
4. The 1-norm of `x`, with `std::transform_reduce`.
5. The entries of `x` larger than 2.5 in absolute value: count them with `std::count_if`, reserve the space, copy them with `std::copy_if`.
6. *(If you have time)* **[Intermediate]** The entry of `x` with the largest absolute value, and its position, with `std::max_element`.

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

---

## Homework

What is left from the lab, plus some extra exercises. Each one has a difficulty label: **[Easy]**, **[Intermediate]** or **[Hard]**.
- **Section 1**, Steps 2, 3 and 4, and **section 2**, exercise 6 **[Intermediate]**, if you did not finish them in class.
- **[Easy]** — **Now break it**, at the end of [Lab 4, Step 4](../04-class_templates-stl/Lab4_ClassTemplates_STL.md#step-4--the-compressed-format-coo-if-you-have-time): fill the matrix of the benchmark in order. What happens to the `vmult` of the `MapMatrix`, and to the one of the `CooMatrix`?
- **[Easy]** — **Algorithms on the COO.** The entries of a `CooMatrix` are a `std::vector<Entry>`: every algorithm of section 2 works on them. Add to `CooMatrix` three methods, each one written with a single algorithm on `entries_` and a lambda that takes a `const Entry &`:
  - `std::size_t n_diagonal() const`: how many stored entries are on the diagonal, with `std::count_if`;
  - `T frobenius_norm() const`: $\sqrt{\sum_{ij} a_{ij}^2}$, with `std::transform_reduce`;
  - `T max_abs() const`: the largest $|a_{ij}|$, with `std::max_element` (careful: what if the matrix has no entries?).

  For the 5×5 matrix of the tests: 5, $\sqrt{28} \approx 5.2915$, 2.
- **[Hard]** — **CSR**: the [exercise of Lab 4](../04-class_templates-stl/Lab4_ClassTemplates_STL.md#exercise-the-csr-format), if you did not do it.

<details>
<summary>Answers (homework)</summary>

- Now break it: the `vmult` of the `MapMatrix` becomes about 4 times faster: filled in order, the nodes of the maps are allocated one after the other, and they happen to be contiguous in memory. The `CooMatrix` does not change: compressing puts the entries in order, whatever happened before (Lab 4, answers of Step 4).
- Algorithms on the COO:
  ```cpp
  std::size_t n_diagonal() const {
    return std::count_if(entries_.begin(), entries_.end(), [](const Entry &e) { return e.row == e.col; });
  }

  T frobenius_norm() const {
    return std::sqrt(std::transform_reduce(entries_.begin(), entries_.end(), T{}, std::plus<T>(),
                                           [](const Entry &e) { return e.value * e.value; }));
  }

  T max_abs() const {
    if (entries_.empty())
      return T{};
    const auto it = std::max_element(entries_.begin(), entries_.end(), [](const Entry &a, const Entry &b) {
      return std::abs(a.value) < std::abs(b.value);
    });
    return std::abs(it->value);
  }
  ```
  On an empty range `max_element` returns `end()`, and `it->value` would read outside the vector. `T{}` as the initial value of `transform_reduce` makes the sum of type `T`, as in exercise 1 of section 2. You need `<cmath>`, `<functional>` and `<numeric>`.

</details>

**Further reading**: in [`AMSC-CodeExamples/Examples/src/STL`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/STL), the examples on the algorithms.
