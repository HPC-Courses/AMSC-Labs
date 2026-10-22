# Laboratory 04 — Class templates and the Standard Library

### 23/10/2026

This lab puts into practice the lectures *Class templates* and *The Standard Library*. Keep the slides at hand: as in [Lab 2](../02-smart_pointers-callables/Lab2_SmartPointers_Callables.md) and [Lab 3](../03-classes-inheritance/Lab3_Classes_Inheritance.md), the theory is only recalled in a few lines at the start of each section.

We warm up with the containers and the algorithms of the standard library, we measure how much they cost, and then we use them to write a sparse matrix: a hierarchy of classes, as the `Shape`s of Lab 3, in which every class is a **template**, as `std::vector`.

Each exercise has an `assignment`, to be completed, and a `solution`. The ✅ **Checkpoint** at the end of each section shows the output you should get. Whatever you do not finish in class is listed in the [Homework](#homework).

## Outline

- [0. Setup](#0-setup)
- [1. Warm-up: the STL in a few lines](#1-warm-up-the-stl-in-a-few-lines)
- [2. How much does a container cost?](#2-how-much-does-a-container-cost)
- [3. Exercise: a sparse matrix](#3-exercise-a-sparse-matrix)
- [Homework](#homework)

---

## 0. Setup

Update the repository and move to the directory of this lab, inside the container:
```bash
cd ~/shared-folder/AMSC-Labs
git pull
cd Labs/2026-27/04-class_templates-stl
ls stl sparse_matrix extra
```
The single-file examples have the command to compile them written at the top; the sparse matrix has a `Makefile`.

---

## 1. Warm-up: the STL in a few lines

The containers of the standard library are class templates, divided into categories (slides 7–10):

| Category | Containers | Elements are found by |
|---|---|---|
| sequence | `vector`, `array`, `deque`, `list` | their position |
| ordered associative | `set`, `multiset`, `map`, `multimap` | their key, kept **sorted** (by `<`, unless you choose another ordering) |
| unordered associative | `unordered_set`, `unordered_map` | their key, in a hash table |

All of them describe their elements with a pair of **iterators**, `[begin(), end())`: this is what the range-based `for` and the algorithms use, and it is why you can build any container from the elements of another one, e.g. `std::set<int> s(v.begin(), v.end());`. A `set` never stores the same key twice; a `map` stores pairs *(key, value)*, and `for (const auto &[key, value] : m)` visits them in the order of the keys.

The algorithms you need today, besides those of Lab 2 (`count_if`, `sort`, `transform`, ...):

| Algorithm | What it does |
|---|---|
| `std::copy_if(b, e, out, p)` | copies the elements for which `p(x)` is true to `out` |
| `std::for_each(b, e, f)` | calls `f(x)` on each element; with `[](int &x) {...}` it can change them |
| `std::back_inserter(v)` | an output iterator that calls `v.push_back`: the algorithm *adds* elements to `v` |

> **`m[key]` inserts.** If `key` is not in the map `m`, `m[key]` adds it, with a value-initialised value (0 for numbers, an empty string for strings), and returns a reference to it. Convenient to write, `++count[word]`; dangerous to read: to look for a key without inserting it, use `m.find(key)` (returns `m.end()` if the key is missing), `m.contains(key)` (C++20), or `m.at(key)` (throws `std::out_of_range`). Since it may insert, `[]` cannot be used on a `const` map (slide 51).

[`stl/01-stl_exercises/assignment.cpp`](stl/01-stl_exercises/assignment.cpp) contains six small exercises: write your solution where you find `TODO`, and the functions in `check.hpp` tell you if it is correct.
```bash
cd stl/01-stl_exercises
g++ -Wall -std=c++20 assignment.cpp -o assignment && ./assignment
```

**Your tasks**:
1. Sort a vector and remove the duplicates, using only a `std::set`.
2. Count how many times each word appears in a text, in a `std::map<std::string, std::size_t>`.
3. A `std::unordered_map<std::string, std::function<double(double)>>` turns a name, such as `"sin"`, into a function (`std::function` is in Lab 2). Use it to evaluate the functions named in a vector at $x = 0.9$.
4. Copy the even numbers into a vector with `std::copy_if` and `std::back_inserter`, then square them in place with `std::for_each`.
5. *(If you have time)* **[Intermediate]** Write `count_of(word_count, word)`, which returns a `std::optional<std::size_t>`: the count of `word`, or an **empty** optional if the word never appears (slides 68–69).
6. *(If you have time)* **[Hard]** Find the three most frequent words, with their counts; words with the same count in alphabetical order.

✅ **Checkpoint.**
```
Assignment 1: PASSED
Assignment 2: PASSED
Assignment 3: PASSED
Assignment 4: PASSED
...
```
Solution: [`solution.cpp`](stl/01-stl_exercises/solution.cpp).

<details>
<summary>Answers</summary>

- 2: `++word_count[w]`. The first time `w` is met, `[]` inserts it with count 0, then `++` makes it 1.
- 3: `{"cos", std::cos}` does not compile because `std::cos` is overloaded (`float`, `double`, ...), so the code wraps it in a lambda. `fmap` is `const`: to read it, `fmap.at(name)`, not `fmap[name]`.
- 4: `std::copy_if` writes through the output iterator, it does not create elements. With `even_squares.begin()` on an empty vector it would write outside the vector; `std::back_inserter` calls `push_back`.
- 5: `word_count` is `const`, so `word_count[word]` does not compile: use `find`, and return `std::nullopt` if the word is missing.
- 6: copy the map into a `std::vector` of pairs: it starts in alphabetical order, as the map. Sort it by decreasing count with `std::stable_sort`, which keeps the alphabetical order among equal counts, and keep the first three.

</details>

---

## 2. How much does a container cost?

The computational complexity of the main operations (slides 22–23):

| | access by position | insert / erase | find a value |
|---|---|---|---|
| `vector` | $O(1)$ | $O(1)$ at the end, $O(n)$ elsewhere | $O(n)$, or $O(\log n)$ if sorted |
| `deque` | $O(1)$, a bit slower | $O(1)$ at both ends, $O(n)$ elsewhere | $O(n)$ |
| `list` | no `[]`; $O(k)$ with `std::next` | $O(1)$, once you know the position | $O(n)$ |
| `set`, `map` | not by position: by key (see "find") | $O(\log n)$ | $O(\log n)$ |
| `unordered_set`, `unordered_map` | not by position: by key (see "find") | $O(1)$ on average | $O(1)$ on average |

But complexity counts *operations*, and the time also depends on *where* the data are in memory (slide 24). [`stl/02-benchmark/benchmark.cpp`](stl/02-benchmark/benchmark.cpp) puts the numbers $0, \dots, n-1$, $n = 10^6$, in several containers, and measures three operations:
1. the sum of all the elements, with `std::accumulate`: $O(n)$ for every container;
2. $10^6$ searches of random keys, with `std::binary_search` on the sorted `vector`, and with `contains` on the sets (the linear `std::find` is so slow that it does only $10^3$ searches);
3. $10^5$ insertions at the front.

```bash
cd ../02-benchmark
g++ -Wall -std=c++20 -O3 benchmark.cpp -o benchmark
./benchmark
```

✅ **Checkpoint.** The absolute times depend on your machine, but the pattern should look like this (measured on a laptop):

| 1. sum | ms | | 2. lookup | ns per lookup | | 3. insert at the front | ms |
|---|---|---|---|---|---|---|---|
| `vector` | **0.15** | | `vector`, `std::find` | 225000 | | `vector` | 340 |
| `list` | 63 | | `vector`, `std::binary_search` | 55 | | `deque` | **0.4** |
| `set` | 47 | | `set::contains` | 110 | | `list` | 1.6 |
| `unordered_set` | 11 | | `unordered_set::contains` | **18** | | | |

Discuss with your neighbour:
1. The sum is $O(n)$ for all the containers: why is the `vector` about 400 times faster than the `list`? (The program sorts the `list` with `lst.sort()`: why does this matter?)
2. `std::binary_search` on a sorted `vector` and `set::contains` are both $O(\log n)$: why is the `vector` faster?
3. The `unordered_set` is the fastest to search. When would you still choose a `set`?
4. Inserting at the front of a `vector` costs $O(n)$: what does it have to do? Why not always use a `deque`, then?

<details>
<summary>Answers</summary>

1. The elements of a `vector` are contiguous: the CPU reads memory in blocks of 64 bytes (*cache lines*), i.e. 16 `int`s at a time, it guesses which block will be needed next (*prefetching*), and the compiler can sum several elements with a single instruction (*vectorisation*). Each element of a `list` is a separate *node* on the heap, with a pointer to the next node: to find the next element, the CPU must first read the pointer. Right after construction the nodes are allocated one after the other, so they happen to be close in memory; `lst.sort()` relinks the nodes without moving them, so after sorting neighbours in the list are far apart in memory, and almost every step is a cache miss. The `set` is a tree of nodes, and it has the same problem.
2. Same reason: the binary search jumps around in a compact array, and its last steps all fall in the same cache line; the nodes of the tree are scattered on the heap, and every level is a pointer to follow.
3. When you need the elements **in order**: to traverse them sorted, or to find all the keys in an interval (`lower_bound`, `upper_bound`). Also when the key has an ordering but no good hash function, or when you need a guaranteed worst case (the unordered containers are $O(n)$ in the worst case).
4. It must move all the elements one position forward. A `deque` stores the elements in fixed-size blocks: inserting at both ends is cheap, but the elements are not all contiguous, and access by position is a bit slower. Use it when you really need a queue with two ends.

</details>

**Take-away:** `std::vector` is difficult to beat (slide 24). Use the other containers when you need what they provide: unique sorted keys, lookup by key, cheap insertion in the middle. And measure.

---

## 3. Exercise: a sparse matrix

A **sparse** matrix is a matrix in which most of the entries are zero, such as the matrices of the finite element method: we store only the non-zero entries. A sparse matrix usually lives in two phases:
1. **Uncompressed**: adding an entry, i.e. changing the *pattern* of the matrix, is cheap, but the matrix-vector product is slow.
2. **Compressed**: the pattern is frozen, but the entries are stored compactly, and the matrix-vector product is fast.

There are many ways to implement both phases. As the `Shape`s of Lab 3, we describe them with an abstract base class, and each format derives from it. As a `std::vector`, each class is a template on the type `T` of the entries:

```
                SparseMatrix<T>          abstract: the interface
                 /           \
  MapMatrix<T, Map>        CooMatrix<T>
   uncompressed             compressed
```

> **A class template is a family of classes.** `template <typename T> class SparseMatrix { ... };` is not a class, but a recipe: the compiler generates a class for each `T` you use, and `SparseMatrix<double>` and `SparseMatrix<float>` are two different, unrelated types (slides 2–3). The compiler generates the code where it sees both the use and the definition: this is why **all the code of a template goes in the header** (slide 23), and why the exercise has one header per class and no `.cpp` besides `main.cpp`. (To see what happens otherwise, look at [`extra/02-headers`](extra/02-headers).)

The files are in [`sparse_matrix/assignment`](sparse_matrix/assignment). The `Makefile` builds two executables: `make run` compiles with AddressSanitizer and runs the tests, `make benchmark` compiles with `-O3` and also runs the benchmark. As we saw in Lab 2, never time a program compiled with the sanitizer.
```bash
cd ../../sparse_matrix/assignment
```

### Step 1 — The interface

Write the abstract class template `SparseMatrix<T>` in [`sparse_matrix.hpp`](sparse_matrix/assignment/sparse_matrix.hpp). It must contain:

| Member | What it is |
|---|---|
| `using Vector = std::vector<T>;` | the type of the vectors |
| `std::size_t nrows() const`, `std::size_t ncols() const` | the sizes, stored in two private data members of the base class |
| `virtual std::size_t nnz() const = 0;` | the number of stored entries |
| `virtual T operator()(std::size_t i, std::size_t j) const = 0;` | the read access `A(i, j)`: returns 0 if the entry is not stored |
| `virtual Vector vmult(const Vector &x) const = 0;` | the product $y = Ax$ |
| `void print(std::ostream &os = std::cout) const` | prints `nrows: 5 \| ncols: 5 \| nnz: 13`, then calls `print_entries(os)` |
| `virtual void print_entries(std::ostream &os) const = 0;` | `protected`: each format prints its entries, one `i, j, value` per line |
| `SparseMatrix(std::size_t nrows, std::size_t ncols)` | `protected`: only the derived classes can build a `SparseMatrix` |
| a virtual destructor | Lab 3 |

How do you know that it is right? [`map_matrix.hpp`](sparse_matrix/assignment/map_matrix.hpp) and [`coo_matrix.hpp`](sparse_matrix/assignment/coo_matrix.hpp) already declare their methods with `override`: until your interface matches, they do not compile (Lab 3). When it does, `make run` compiles.

Solution: [`sparse_matrix/solution/sparse_matrix.hpp`](sparse_matrix/solution/sparse_matrix.hpp).

### Step 2 — The uncompressed format: a map per row

`MapMatrix<T, Map>` stores row `i` in `rows_[i]`, a map `{column -> value}`, so `rows_` is a `std::vector<Map<std::size_t, T>>`. The write access `A(i, j) = value` is just `rows_[i][j]`: as for any map, it inserts the entry if it is not there yet. The syntax you will meet:

| Syntax | Meaning |
|---|---|
| `template <typename T, template <typename...> class Map = std::map>` | `Map` is a **template template parameter**: not a type, but a class template, such as `std::map`, which `MapMatrix` instantiates as `Map<std::size_t, T>` (slide 15). `typename...` because `std::map` has more than two parameters, with defaults |
| `class MapMatrix : public SparseMatrix<T>` | a class template that derives from a class template |
| `using Vector = typename SparseMatrix<T>::Vector;` | `Vector` depends on `T`: `typename` tells the compiler that it is a type (since C++20 it may be omitted in a `using` like this one, but not everywhere) |
| `T{}` | a value-initialised `T`: `0` for the numbers |

> **`this->`.** Inside `MapMatrix`, `nrows()` alone does not compile: *there are no arguments to 'nrows' that depend on a template parameter*. When the compiler reads a template, it does not look into a base class that depends on `T`, because it does not know it yet. Write `this->nrows()`: `this` depends on `T`, so the name is looked up later, when `T` is known (slide 19).

The tests use the matrix of the second derivative, and `check_vmult(A)`, given, checks the product on the right. It takes a `const SparseMatrix<T> &`: it works for every format.

$$
A=
\begin{bmatrix}
-2 & 1 & 0 & \dots & 0 \\\
1 & -2 & 1 & \dots & 0 \\\
\vdots & \ddots & \ddots & \ddots & \vdots \\\
0 & \dots & 1 & -2 & 1 \\\
0 & \dots & 0 & 1 & -2 \\\
\end{bmatrix},
\qquad
A
\begin{bmatrix}
0 \\\ 1 \\\ 2 \\\ \vdots \\\ n-1
\end{bmatrix} =
\begin{bmatrix}
1 \\\ 0 \\\ 0 \\\ \vdots \\\ -n
\end{bmatrix}.
$$

**Your tasks**:
1. In `main.cpp`, complete `fill_tridiagonal(A)`, which fills this matrix. Careful with the first and the last row.
2. In `map_matrix.hpp`, implement:
   - `nnz()`;
   - the read access `T operator()(i, j) const`. It must **not** insert anything: use `find`;
   - in the write access, throw `std::out_of_range` if `(i, j)` is outside the matrix;
   - `vmult(x)`: throw `std::invalid_argument` if the size of `x` is wrong, then loop over the rows, and over the entries of each row with `for (const auto &[j, a_ij] : rows_[i])`;
   - `print_entries(os)`.
3. Run `make run`. Before you finish, AddressSanitizer stops the program with a `heap-buffer-overflow`: the last test writes `A(5, 0)`, outside a 5×5 matrix. Who should have stopped it?
4. Look at the two lines after the `vmult` check in `main`: `cA(0, 4)` does not change `nnz`, `A(4, 0)` does. Why?

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

### Step 3 — `std::map` or `std::unordered_map`?

Changing the container of the rows takes one word: the second template argument.

**Your tasks**:
1. In `main`, step 3: build `B`, a 5×5 `MapMatrix<double, std::unordered_map>`, then fill it, print it and check it as `A`. In which order are the entries of each row printed? Is `vmult` still correct?
2. At the end of `main`, run the benchmark with `std::unordered_map` too, then `make benchmark`. The benchmark fills a matrix with $n = 5\cdot10^5$ rows and times 10 products. As in a real finite element code, the rows are not filled in order: `fill_tridiagonal(A, true)` fills them in a random order (`std::shuffle`). Which `Map` is faster to fill? Which one is faster in `vmult`? Look back at section 2.

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

### Step 4 — The compressed format: COO (if you have time)

**[Intermediate]**

The **COO** (*COOrdinate*) format stores the list of the triplets `(row, col, value)`, sorted by row and then by column, in a single `std::vector<Entry>`:
```
entries_ = { (0,0,-2), (0,1,1), (1,0,1), (1,1,-2), (1,2,1), (2,1,1), ..., (4,4,-2) }
```
A `CooMatrix` can only be built from a `MapMatrix`, and no entry can be added afterwards. Its constructor is a **member function template** (slide 4): it has its own template parameter `Map`, so that it accepts a `MapMatrix<T, Map>` with any `Map`.

**Your tasks**, in `coo_matrix.hpp`:
1. Write the body of the constructor: copy the entries of `A` into `entries_`, row by row (`A.row(i)` returns the map of row `i`), then sort them with `std::sort` and the comparison `by_position`, which is given. Why is the sort needed, if `MapMatrix` uses a `std::map`? And if it uses a `std::unordered_map`?
2. Implement `nnz()`, `vmult(x)` (a single loop on `entries_`) and `print_entries(os)`.
3. Implement the read access with a **binary search**: `std::lower_bound(begin, end, key, by_position)` returns the first entry that is not before `key`, in $O(\log \text{nnz})$. Check that it really is `(i, j)` before returning its value.
4. Uncomment step 4 in `main`, and the COO lines in `benchmark`. Note `const CooMatrix C{B};`, with no `<double>`: how does the compiler deduce `T`?

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

Solution: [`sparse_matrix/solution`](sparse_matrix/solution).

**Now break it.** In `benchmark`, fill the matrix in order: `fill_tridiagonal(A, false)`. What happens to the `vmult` of the `MapMatrix`? And to the one of the `CooMatrix`?

<details>
<summary>Answers</summary>

- Step 2, `A(5, 0)`: the write access, which must check the indices and throw `std::out_of_range`. Without the check, `rows_[5]` reads outside the vector `rows_`.
- Step 2, `A(4, 0)`: `A` is not `const`, so `A(4, 0)` calls the non-const `operator()`, the write access, which inserts the entry `(4, 0)` with value 0: the trap of `m[key]` in section 1. `cA` is a `const` reference, so only the const version can be called. To prevent the mistake, the class could give a different name to the write access, e.g. `A.insert(i, j, value)`, and keep `operator()` for reading only.
- Step 3: the `std::unordered_map` prints the entries of each row in the order of its hash table; `vmult` does not care about the order. Filling is slower with the `unordered_map`: with 3 entries per row, $O(\log 3)$ and $O(1)$ are the same thing, and what counts is the constant: hashing, and the array of buckets that each `unordered_map` allocates. Both are made of nodes scattered on the heap, so both are slow in `vmult`.
- Step 4, sort: with a `std::map` the columns of each row are already sorted, and the rows are visited in order, so `entries_` comes out sorted. With a `std::unordered_map` it does not, and `std::lower_bound` requires a range sorted with the same comparison it uses: otherwise its result is wrong, without any error. Sorting a vector that is already sorted is cheap.
- Step 4, CTAD: `T` appears in the parameter of the constructor, `const MapMatrix<T, Map> &`; from `B`, a `MapMatrix<double, std::unordered_map>`, the compiler deduces `T = double` and `Map = std::unordered_map` (slide 6).
- Step 4, benchmark: the entries of the `CooMatrix` are in one contiguous vector, in the order in which `vmult` reads them: its `vmult` is faster than the one of both maps, as the `vector` in section 2.
- Break it: the `vmult` of the `MapMatrix` becomes about 4 times faster (3 ms instead of 13 on the laptop of the table), close to the COO, and on some machines even faster. Filled in order, the nodes are allocated one after the other, and they happen to be contiguous in memory, like the `list` of section 2 before sorting. Same code, same complexity, a factor 4 that depends only on the order in which the matrix was filled. The `CooMatrix` does not change: compressing puts the entries in order, whatever happened before. In a real code you do not control the order of the insertions, so you compress.

</details>

**Static and dynamic polymorphism, together.** `check_vmult` and `print` work on a `const SparseMatrix<T> &`, and call the right `vmult` at run time through the virtual functions (*dynamic*, Lab 3); inside each format, `T` and `Map` are fixed at compile time, and the compiler optimises the loop of `vmult` for them (*static*, Lab 2).

---

## Homework

What is left from the lab, plus some extra exercises. Each one has a difficulty label: **[Easy]**, **[Intermediate]** or **[Hard]**.
- **Section 1**, assignments 5 **[Intermediate]** and 6 **[Hard]**, and **Step 4** of the sparse matrix **[Intermediate]**, if you did not do them in class.
- **[Hard]** — **CSR**: the [exercise below](#exercise-the-csr-format).
- **[Easy]** — **Rows with many entries.** In the benchmark of section 3 the rows have 3 entries. Write a small program that fills a 1000×1000 `MapMatrix` with *all* the entries, with the columns of each row in a random order, then reads them all with the read access. Compare `std::map` and `std::unordered_map`: who wins now, and why?
- **[Intermediate]** — **Shapes without inheritance**: the [exercise below](#exercise-shapes-with-stdvariant).
- **[Easy]** — **Extra**: [three short examples](#extra-three-short-examples-on-class-templates) on class templates.

### Exercise: the CSR format

**[Hard]**

The COO format stores the row index of every entry. The **CSR** (*Compressed Sparse Row*) format stores the entries row by row in three vectors: `values_`, the column of each entry `col_idx_`, and `row_ptr_`, such that the entries of row `i` are in the positions `[row_ptr_[i], row_ptr_[i+1])`. For the 5×5 matrix:
```
values_  = { -2, 1,   1, -2, 1,   1, -2, 1,   1, -2, 1,   1, -2 }
col_idx_ = {  0, 1,   0,  1, 2,   1,  2, 3,   2,  3, 4,   3,  4 }
row_ptr_ = {  0,      2,          5,          8,         11,     13 }
```
CSR, and its version by columns (CSC, the default of `Eigen::SparseMatrix`, Lab 1), are the formats used by most libraries.

1. Write `csr_matrix.hpp`, with a class `CsrMatrix<T>` derived from `SparseMatrix<T>`, built from a `MapMatrix<T, Map>` like `CooMatrix`. Careful: the columns of a row must be sorted, even if the rows are `std::unordered_map`s.
2. In `vmult`, the entries of row `i` all contribute to `y[i]`: accumulate them in a local variable.
3. For the read access, use `std::lower_bound` on the columns of row `i` only.
4. Add it to the tests and to the benchmark, and compare its `vmult` with the one of the COO.

✅ **Checkpoint.** The `vmult` of the CSR is the fastest: about **1.3 ms** on the laptop of the table, 2–4 times faster than the COO, and 10 times faster than the `MapMatrix<double, std::map>`. Why? (Compare the bytes per entry of the two formats, and count how many times `y` is written.)

Solution: [`sparse_matrix/solution/csr_matrix.hpp`](sparse_matrix/solution/csr_matrix.hpp), and `make benchmark` in the solution.

### Exercise: shapes with `std::variant`

**[Intermediate]**

In Lab 3 the shapes were a hierarchy with virtual functions, and lived on the heap behind a `std::unique_ptr<Shape>`. A `std::variant<Circle, Rectangle, Square>` is an object that holds **one** value of one of the listed types, by value, and knows which one (slides 72–75). `std::visit(f, s)` calls `f` with the value that `s` holds now: with a generic lambda, `[](const auto &x) { ... }`, the compiler generates one version of `f` for each type of the variant, and `std::visit` chooses the right one at run time.

In [`stl/03-variant_shapes/assignment.cpp`](stl/03-variant_shapes/assignment.cpp), `Circle`, `Rectangle` and `Square` are plain `struct`s, with no base class, and `shapes` is a `std::vector<Shape>`, where `using Shape = std::variant<Circle, Rectangle, Square>;`.

**Your tasks**:
1. Print name, area and perimeter of each shape, with `std::visit`.
2. Compute the total area.
3. Count the circles with `std::holds_alternative<Circle>`.
4. Add a `Triangle` (Lab 3 homework) and a `Triangle{3., 4., 6.}` to `shapes`. What did you change? Now imagine adding a method, e.g. `diameter()`, to every shape: what would you change here, and what in Lab 3?

✅ **Checkpoint.**
```
  Circle: area 3.14159, perimeter 6.28319
  Rectangle: area 6, perimeter 10
  Square: area 2.25, perimeter 6
  Circle: area 0.785398, perimeter 3.14159
  Triangle: area 5.33268, perimeter 13
Total area: 17.5097
Circles: 2
```
Solution: [`solution.cpp`](stl/03-variant_shapes/solution.cpp).

### Extra: three short examples on class templates

**[Easy]**

Each file has the command to compile it at the top. Read it, predict the output, run it.
- [`extra/01-point/point.cpp`](extra/01-point/point.cpp): `Point<T, N>`, with a type and a non-type template parameter (slide 8), a default template argument, CTAD (slide 6), an alias template (slide 16). Why is `|b|` equal to 1? Why is `sizeof(r)` 12 bytes? Uncomment `names.norm()`: why does `Point2D<std::string>` compile until you call `norm()` (slide 3)?
- [`extra/02-headers`](extra/02-headers): the definition of a member of a class template in a `.cpp`, as for ordinary classes. Why does the linker fail? Fix it by moving the definition to the header; then try the explicit instantiation `template class Point<double, 2>;` at the end of `point.cpp` (slide 24): what happens with a `Point<float, 3>`?
- [`extra/03-containers/containers.cpp`](extra/03-containers/containers.cpp): sets with different orderings (slides 36–40), a phone book with `std::map`. Why does the `set<int, ModCompare{3}>` contain only three elements? Who added Maria to the phone book?

<details>
<summary>Answers (homework)</summary>

- Rows with many entries: now the `std::unordered_map` wins clearly (on the same laptop: fill 31 ms instead of 47, reading 3 ms instead of 16). With 1000 entries per row, a search in a `std::map` follows about $\log_2 1000 \approx 10$ pointers in a tree, while the `unordered_map` computes one hash and looks in one bucket. Complexity matters when $n$ is large; when $n$ is 3, the constants do.
- CSR: a COO entry takes 24 bytes (two `std::size_t` and a `double`), a CSR entry 16 (a `std::size_t` and a `double`), plus one `std::size_t` per row. And the COO writes `y[e.row]` for every entry, while the CSR sums the entries of a row in a local variable, which stays in a register, and writes `y[i]` once per row.
- Variant: adding a type means adding a `struct` and a name in the list of the variant; the loops do not change, because the generic lambdas work for every type that has `name()`, `area()` and `perimeter()`. Adding an operation is easy too: one more `std::visit`, without touching the structs. In Lab 3 it is the opposite: adding a type is easy, adding a method means changing the base class and every derived class. On the other hand, the variant is a **closed** set: the list of types is fixed where `Shape` is defined, while anybody can derive a new class from the `Shape` of Lab 3, even in another library.
- `point.cpp`: with `T = int`, `std::sqrt(2)` is a `double`, `1.414...`, converted to `int` by the `return` (`-Wconversion` warns); `auto norm() const` fixes it. The three `float`s of `r` are inside the object, on the stack: no heap, since `N` is known at compile time. The members of a class template are generated only when they are used, so the error on `norm()` appears only when it is called.
- `02-headers`: when compiling `main.cpp` the compiler sees the use of `Point<double, 2>::norm()` but not its body; when compiling `point.cpp` it sees the body, but no use. Nobody generates it. The explicit instantiation generates only `Point<double, 2>`: with a `Point<float, 3>` the linker fails again.
- `containers.cpp`: `ModCompare{3}` compares the remainders modulo 3, so numbers with the same remainder are equivalent, and a `set` keeps only the first one: `5`, `3`, `1`, sorted by remainder, `3 1 5`. Maria was added by `phone["Maria"]`, as in section 1.

</details>

**Further reading** in [`AMSC-CodeExamples/Examples/src`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src): [`Templates`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/Templates) (in particular [`Compare`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/Templates/Compare), [`ExplicitInstantiation`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/Templates/ExplicitInstantiation) and [`traits`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/Templates/traits)), [`STL`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/STL) (in particular [`Map`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/STL/Map), [`UnorderedMap`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/STL/UnorderedMap), [`Optional`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/STL/Optional) and [`Variant`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/STL/Variant)).

---

## Before the next lab: access to the cluster

1. Fill in the form to request an account on the HPC cluster: [HPC account request - students (2026)](https://forms.cloud.microsoft/e/fQtq8hk2Pk).
2. Read the manual on WeBeep, to download the VPN and to understand how the HPC cluster works: [WeBeep course page](https://webeep.polimi.it/course/view.php?id=14384).
