# Laboratory 06 — The cluster, algorithms and templates

### 06/11/2026

In the first part of this lab we log in to the HPC cluster and use **PBS**, the program that decides when and where our jobs run. Then we put into practice two lectures: the algorithms of the Standard Library, on the nodes of a 1D mesh, and *More on templates*, with an exercise on the derivatives of a function.

Each exercise has an `assignment`, to be completed, and a `solution`. The ✅ **Checkpoint** at the end of each section shows the output you should get. Whatever you do not finish in class is listed in the [Homework](#homework).

## Outline

- [0. Setup](#0-setup)
- [1. The cluster and PBS](#1-the-cluster-and-pbs)
- [2. More algorithms](#2-more-algorithms)
- [3. Exercise: derivatives of any order](#3-exercise-derivatives-of-any-order)
- [Homework](#homework)

---

## 0. Setup

Update the repository and move to the directory of this lab, inside the container:
```bash
cd ~/shared-folder/AMSC-Labs
git pull
cd Labs/2026-27/06-cluster-templates
```

---

## 1. The cluster and PBS

A cluster is a set of computers, the **nodes**. You log in to the **login node**, where you edit files and submit jobs. Everything else, compilation included, runs on the **compute nodes**: you ask for them to **PBS**, which runs your job when the resources you asked for are free.

> **Never compile or run your computations on the login node**: it is shared by all the users of the cluster, and a process that uses too much CPU there is killed automatically.

### 1.1 Log in

You need the account you asked for at the end of [Lab 4](../04-class_templates-stl/Lab4_ClassTemplates_STL.md#before-the-next-lab-access-to-the-cluster) and the VPN. The address of the cluster is in the manual on [WeBeep](https://webeep.polimi.it/course/view.php?id=14384). Your username is `u` followed by your person code, e.g. `u12345678`.

**Your tasks**:
1. From the terminal of your laptop (not from the container), log in to the cluster with `ssh` (Lab 1, section 1.3). Run `hostname`: this is the login node.
2. Clone the repository of the labs in your home:
   ```bash
   git clone https://github.com/HPC-Courses/AMSC-Labs.git
   ```

### 1.2 The PBS commands

When you log in, the PBS commands are not in the `PATH`. Run `. /etc/profile.d/pbs.sh` **in every new terminal**, or use the full path of the commands, e.g. `/opt/pbs/bin/pbsnodes -aSj`.

| Command | What it does |
|---|---|
| `pbsnodes -aSj` | shows the nodes, and how many of their CPUs are free |
| `qstat` | shows the jobs in the queues; `qstat -u $(whoami)` only yours |
| `qstat -Qf cpu` | shows the limits of the queue `cpu` |
| `qsub` | submits a job |
| `qdel <job id>` | deletes a job you submitted |

**Your task**: run `pbsnodes -aSj`. How many nodes are there? What are their names? Then run `qstat -Qf cpu`: how many CPUs can a job ask for, at most?

<details>
<summary>Answers</summary>

7 nodes with 112 CPUs each: `cpu01`, ..., `cpu05` and `gpu01`, `gpu02`. The queue `cpu` contains the 5 `cpu` nodes. A job can ask for at most 28 CPUs (`resources_max.ncpus`) and 48 hours, and each user can run at most 4 jobs at the same time (`max_user_run`).

</details>

### 1.3 An interactive job

`qsub -I` (capital `i`) asks for an **interactive** job: PBS gives you a shell on a compute node. Exit with `ctrl+D`. You choose the queue with `-q` and the resources with `-l`, e.g. two CPUs for ten minutes:
```bash
qsub -I -q cpu -l select=1:ncpus=2 -l walltime=00:10:00
```
`select=1` asks for one *chunk* of resources, and after the `:` you say what it contains. The walltime is the maximum duration of the job.

Always write the queue and the walltime: `qsub -I -l select=1:ncpus=2` goes to the default queue, `interactive`, and is refused with `Job violates queue and/or server resource limits`.

On the nodes, the compiler and the libraries are installed with **spack** (Lab 1, section 5.2):
```bash
source /software/spack/share/spack/setup-env.sh
spack load gcc@15.2.0
which gcc
gcc --version
```

**Your tasks**:
1. Start an interactive job with 2 CPUs. Run `hostname` and `echo $NCPUS`, the number of CPUs of the job. From another terminal on the login node, run `qstat -u $(whoami)`: do you see your job?
2. Inside the job, load gcc with spack. Compare `which gcc` with the one of the login node. Exit with `ctrl+D`.

✅ **Checkpoint.** Inside the job, `hostname` prints a compute node, e.g. `cpu04.mate.polimi.it`, `echo $NCPUS` prints `2`, `which gcc` prints a path inside `/software`, and `gcc --version` prints `15.2.0`.

### 1.4 Batch jobs

For a real computation you write a **job script**: a bash script whose first lines, starting with `#PBS`, contain the options of `qsub`. PBS runs it when the resources are free, also when you are not logged in. [`cluster/1_specific_hosts.job`](cluster/1_specific_hosts.job):
```bash
#!/bin/bash
#PBS -q cpu
#PBS -l select=1:ncpus=1:host=cpu04+1:ncpus=1:host=cpu05
#PBS -l walltime=00:01:00
#PBS -N hello_pbs
echo "hello from pbs on node $(hostname)" &> $HOME/hello.from.pbs
# Keeps the job alive for a while, so that you can see it in qstat
sleep 30
```
It asks for two chunks, one CPU on `cpu04` and one on `cpu05`, for at most one minute; `-N` is the name of the job in `qstat`. [`cluster/2_scatter.job`](cluster/2_scatter.job) asks for 3 chunks, `place=scatter` puts them on different nodes, and the script prints the names of the nodes, which PBS writes in the file `$PBS_NODEFILE`.

Every option of a script can also be written on the command line of `qsub`, and vice versa: `-l select=1:ncpus=2` on the command line is the line `#PBS -l select=1:ncpus=2` of a script.

A batch job has no terminal, so what it prints has to go to a file. PBS should collect it in `<job name>.o<job id>` and `<job name>.e<job id>`, but on this cluster these files may not arrive: our scripts write their output to a file themselves, with `&>`.

**Your tasks**:
1. Submit the first script and look at the queue. The column `S` is the state: `Q` waiting, `R` running, `E` ending.
   ```bash
   cd ~/AMSC-Labs/Labs/2026-27/06-cluster-templates/cluster
   qsub 1_specific_hosts.job
   qstat -u $(whoami)
   ```
2. When it is over, read `~/hello.from.pbs`. The job has two nodes: how many lines are in the file?
3. Submit the second script, and read `~/hello.scatter.<job id>`. Who has written these lines?
4. Submit the first script again, and delete it with `qdel` before it ends.

✅ **Checkpoint.** `~/hello.from.pbs` contains `hello from pbs on node cpu04.mate.polimi.it`; `~/hello.scatter.<job id>` contains 3 lines, with 3 different nodes.

<details>
<summary>Answers</summary>

- 2: one line. The script runs **only on the first node** of the job: the CPU on `cpu05` is reserved, but nothing runs there.
- 3: the first node again: it only reads the names of the 3 nodes. To start a program on all the nodes of a job you need a launcher such as `mpirun`, which we will see with MPI.

</details>

---

## 2. More algorithms

In [Lab 5](../05-sparse_matrix-algorithms/Lab5_SparseMatrix_Algorithms.md) we used the algorithms that *read* a range and compute a result, such as `std::accumulate`, `std::count_if` and `std::max_element`. Today we use the ones that *reorganise* a range, on the nodes of a 1D mesh (slides 78–91 of *The Standard Library*).

| Algorithm | Header | What it does |
|---|---|---|
| `std::unique(b, e)` | `<algorithm>` | removes the **consecutive** duplicates and returns the new end (slide 91) |
| `std::adjacent_difference(b, e, out)` | `<numeric>` | `out[0] = a[0]`, then `out[i] = a[i] - a[i-1]` (slide 84) |
| `std::adjacent_difference(b, e, out, f)` | `<numeric>` | the same, with `f(a[i], a[i-1])` instead of the difference |
| `std::minmax_element(b, e)` | `<algorithm>` | a pair of iterators: to the smallest and to the largest element |
| `std::upper_bound(b, e, x)` | `<algorithm>` | in a **sorted** range, the first element larger than `x`, in $O(\log n)$. `std::lower_bound` (Lab 5) returns the first one that is not smaller |
| `std::merge(b1, e1, b2, e2, out)` | `<algorithm>` | writes two **sorted** ranges into `out`, sorted |

> **An algorithm cannot change the size of a container.** It only sees two iterators, not the vector: it can move the elements, but it cannot remove them. `std::unique` and `std::remove_if` move the elements to keep to the front, and return an iterator to the new end; the elements after it are still there, with an unspecified value. To really remove them you must call the `erase` of the vector, from the new end to `v.end()`.

A mesh divides an interval into **elements**: element $i$ is the interval $[x_i, x_{i+1})$ between two consecutive nodes, and its size is $h_i = x_{i+1} - x_i$. With $n$ nodes there are $n - 1$ elements:
```text
nodes      x0 ------ x1 ------ x2 ------ x3
elements   |    0    |    1    |    2    |
```

In [`algorithms/01-mesh/assignment.cpp`](algorithms/01-mesh/assignment.cpp), `nodes` contains the nodes of two meshes of $[0, 1]$, put together: they are not sorted, and some are repeated.
```bash
cd algorithms/01-mesh
g++ -Wall -std=c++20 assignment.cpp -o assignment && ./assignment
```

**Your tasks**:
1. Sort `nodes` and remove the duplicates, with `std::sort`, `std::unique` and `erase`.
2. The sizes $h_i$ of the elements, with `std::adjacent_difference`. Which is the first number that it writes?
3. The smallest and the largest size, with one call to `std::minmax_element`.
4. Write `element_of(nodes, x)`, which returns the index $i$ of the element $[x_i, x_{i+1})$ that contains `x`, with `std::upper_bound`. For example, `x = 0.3` is in $[0.25, 0.5)$, so the result is 1. What does it return for `x = 1`?
5. *(If you have time)* Refine the mesh: compute the midpoints of the elements with `std::adjacent_difference` and a lambda, then put them together with the nodes with `std::merge`.
6. **Now break it**: comment out `erase` and print `nodes`. Then put `erase` back, and comment out `std::sort`. Do you get an error? What is `hmin`?

✅ **Checkpoint.**
```
Exercise 1: PASSED
Exercise 2: PASSED
Exercise 3: PASSED
Exercise 4: PASSED
Exercise 5: PASSED
```
Solution: [`solution.cpp`](algorithms/01-mesh/solution.cpp).

<details>
<summary>Answers</summary>

- 1: `nodes.erase(std::unique(nodes.begin(), nodes.end()), nodes.end());`, after the sort.
- 2: the first number written is `nodes[0]`, which is not a difference: remove it with `h.erase(h.begin());`.
- 3: `const auto [smallest, largest] = std::minmax_element(h.begin(), h.end());`. They are iterators: the values are `*smallest` and `*largest`.
- 4: the 6 nodes define 5 elements, the intervals between two consecutive nodes: element $i$ is $[x_i, x_{i+1})$, with $i = 0, \dots, 4$. `upper_bound` returns the first node larger than `x`, which is the right end of the element, so the index is `std::distance(nodes.begin(), it) - 1`. For example, for `x = 0.3` it returns the node 0.5, which has index 2, so `x` is in element 1, $[0.25, 0.5)$. For `x = 1` no node is larger: it returns `end()`, and `element_of` returns 5, an element that does not exist. A real code must handle this case, e.g. by returning the last element.
- 5: `[](double right, double left) { return 0.5 * (left + right); }`, then remove the first number as in task 2. `std::merge` needs a `std::back_inserter(refined)`, as `copy_if` in Lab 5.
- 6: without `erase`, `nodes` still has 9 entries, `0 0.25 0.5 0.75 0.875 1 0.875 1 1`: the last three are what is left behind `unique`. Without the sort there is no error at all: no two equal nodes are consecutive, so `unique` removes nothing, and `hmin` is $-0.75$, a negative size. `unique`, `upper_bound` and `merge` do not check that the range is sorted: it is your responsibility, as in Lab 5 with `lower_bound`.

</details>

---

## 3. Exercise: derivatives of any order

This exercise puts into practice the lecture *More on templates*: unless otherwise stated, the slides are the ones of that lecture.

The first derivative of $f$ can be approximated by a forward or a backward difference, with spacing $h$:

$$
D^+ f(x) = \frac{f(x + h) - f(x)}{h}, \qquad D^- f(x) = \frac{f(x) - f(x - h)}{h}.
$$

The derivative of order $N$ is the first derivative of the derivative of order $N-1$. We write this recursion on the *types*: a `NthDerivative<2, ...>` contains a `NthDerivative<1, ...>`, which contains a `NthDerivative<0, ...>`, which contains $f$. The order `N` is known at compile time, so the compiler generates the whole chain, one class for each order.
```text
NthDerivative<2, F, double, Forward>            (f'(x + h) - f'(x)) / h
  └── NthDerivative<1, F, double, Backward>     f'(x) = (f(x) - f(x - h)) / h
        └── NthDerivative<0, F, double, Forward>   f(x)
```
Most of what we need is in the lecture on *Class templates*; the keyword `typename` is new:

| Code | Meaning |
|---|---|
| `template <unsigned N, typename F, typename T = double, typename DT = Forward>` | `N` is a non-type template parameter (*Class templates*, slide 8) |
| `class NthDerivative<0, F, T, DT>` | a partial specialization: `N` is fixed, the others are free (*Class templates*, slide 12) |
| `if constexpr (std::is_same_v<DT, Forward>)` | chooses a branch at compile time (*Class templates*, slides 20 and 22) |
| `struct Forward { using Other = Backward; };` | a **trait**: a struct that collects type aliases (slide 10) |
| `typename DT::Other` | says that `DT::Other` is a type (slide 9) |

> **`typename`.** When the compiler reads the template, it does not know `DT` yet: it cannot know if `DT::Other` is a type or a static member, and it assumes that it is not a type. If it is a type you must say so, with `typename`. This is the same problem as `this->` in Lab 4: the compiler reads a template before it knows its arguments.

The classes are in [`templates/01-derivatives/assignment/derivatives.hpp`](templates/01-derivatives/assignment/derivatives.hpp).

### Step 1 — The end of the recursion

```bash
cd ../../templates/01-derivatives/assignment
make
```
It does not compile: `fatal error: template instantiation depth exceeds maximum of 900`. Read the code of `NthDerivative`: why?

**Your task**: write the partial specialization for `N = 0`. Its call operator returns `f(x)`, and it does not need `h`. Now `make run` compiles, and all the errors are large.

### Step 2 — The call operator

**Your task**: write `operator()` with `if constexpr`: the forward difference if `DT` is `Forward`, the backward one otherwise. Use `previous_`, not `f`. Then `make run`.

The program computes the derivatives of $\sin x$ in $x = 1$, with $h = 0.01$ and $h/2$. If the error goes as $h^p$, the `ratio` of the two errors is $2^p$. What is the order of the four derivatives?

### Step 3 — Alternate the differences

For now `Previous` has the same `DT`, so all the differences are forward differences.

**Your tasks**:
1. Pass `DT::Other` to `Previous`, without `typename`, and compile. Read the error, then fix it.
2. `make run`. Which orders have changed? Write the second derivative by hand: which formula do you get?

✅ **Checkpoint.** The `ratio` printed by the program:

| `N` | Step 2, always `Forward` | Step 3, alternating |
|---|---|---|
| 1 | 2 | 2 |
| 2 | 1.99 | **4** |
| 3 | 2.01 | 2 |
| 4 | 1.98 | **3.67** |

### Step 4 — `make_derivative`

`NthDerivative<2> d{f, h};` would be convenient, but it does not compile: CTAD (Lab 4) deduces all the arguments of a class template, or none. A function template does not have this limit: you write the first arguments, and the compiler deduces the others.

**Your tasks**:
1. Write `make_derivative<N>(f, h)`, which returns a `NthDerivative<N, F, T>`. Which template parameters does it have, and in which order?
2. Uncomment Step 4 in `main`, and `make run`. Which is `T` in `d2_float`? Why is its result so wrong?

### Step 5 — How small should $h$ be?

The program also prints the errors for $N = 2$ and $N = 4$, for smaller and smaller $h$. Which $h$ gives the smallest error? Why do the errors grow again? How many times is $f$ called for $N = 4$?

✅ **Checkpoint.**
```
Step 4: make_derivative
double: -0.841, float: -0.715, exact: -0.841

Step 5: how small should h be?
h        N = 2      N = 4
0.1   0.000701   0.0014
0.01   7.01e-06   1.4e-05
0.001   7.02e-08   0.000366
0.0001   3.03e-09   0.841
1e-05   7.58e-07   1.11e+04
1e-06   3.3e-05   2.22e+08
```
Solution: [`templates/01-derivatives/solution`](templates/01-derivatives/solution).

### Step 6 — On the cluster (if you have time)

[`derivatives.job`](templates/01-derivatives/derivatives.job) compiles `solution` on a compute node, with the gcc of spack, and runs it. In the terminal connected to the cluster, update the repository with `git pull`, move to `~/AMSC-Labs/Labs/2026-27/06-cluster-templates/templates/01-derivatives` and submit it with `qsub derivatives.job`. When `qstat -u $(whoami)` no longer shows it, read `derivatives.<job id>.out`.

<details>
<summary>Answers</summary>

- Step 1: `N - 1` never stops. `N` is `unsigned`: after 0 comes `4294967295`, then the compiler gives up.
  ```cpp
  template <typename F, typename T, typename DT>
  class NthDerivative<0, F, T, DT> {
  public:
    NthDerivative(const F &f, const T &) : f_{f} {}
    T operator()(const T &x) const { return f_(x); }

  private:
    F f_;
  };
  ```
- Step 2: with `if constexpr` only one of the two branches is compiled, for each class of the chain. All the orders are first order in $h$: the ratio is 2.
- Step 3: `error: to refer to a type member of a template parameter, use 'typename DT::Other'`. The right line is `using Previous = NthDerivative<N - 1, F, T, typename DT::Other>;`. By alternating, the second derivative is the usual centred formula
  $$
  D^+ D^- f(x) = \frac{f(x + h) - 2 f(x) + f(x - h)}{h^2},
  $$
  of order 2 in $h$, so the even orders have ratio 4. For $N = 4$ the ratio is not exactly 4 because of the round-off errors: see Step 5.
- Step 4:
  ```cpp
  template <unsigned N, typename F, typename T>
  auto make_derivative(const F &f, const T &h) {
    return NthDerivative<N, F, T>{f, h};
  }
  ```
  `N` comes first because it is the one we write: `F` and `T` are deduced from the arguments. In `d2_float`, `T` is `float`, because `1e-3f` is a `float`, and `f` is a generic lambda, so it computes in `float` too. A `float` has about 7 significant digits: too few to divide by $h^2 = 10^{-6}$.
- Step 5: the truncation error decreases with $h$, but $f(x + h) - f(x)$ is a difference of two very close numbers, and each division by $h$ amplifies its round-off error: the error is about $\varepsilon / h^N$, with $\varepsilon \approx 10^{-16}$. For $N = 4$ and $h = 10^{-4}$, $\varepsilon / h^4 \approx 1$. The best $h$ is about $10^{-4}$ for $N = 2$, and $10^{-2}$ for $N = 4$. Each order calls the previous one twice, so $f$ is called $2^N = 16$ times.
- Step 6: the first line of the output is the version of the compiler, 15.2.0, and the table has the same pattern as on your laptop. The code is compiled on the compute node, never on the login node.

</details>

---

## Homework

Each exercise has a difficulty label: **[Easy]**, **[Intermediate]** or **[Hard]**.
- **Sections 1–3**, if you did not finish them in class.
- **[Easy]** — **A third difference.** Add to `derivatives.hpp` a type `Centered`, with $D^0 f(x) = \dfrac{f(x + h) - f(x - h)}{2h}$. Which is its `Other`? What does `operator()` become? With `NthDerivative<N, F, double, Centered>`, what is the `ratio` for $N = 1, 2, 3$? For $N = 2$, compare the error with the one of Step 3: which formula do you get?
- **[Easy]** — **Ranges and projections** (slide 78 of *The Standard Library*). The algorithms in `std::ranges` take the container, and a *projection*: the member to look at. In the `Particle` exercise of Lab 2, sort the particles by position with `std::ranges::sort`, and find the heaviest one with `std::ranges::max_element`, without writing any lambda.
- **[Easy]** — **Predict it: SFINAE** (slides 7–8 of *More on templates*). Before running [`templates/03-sfinae/sfinae.cpp`](templates/03-sfinae/sfinae.cpp), predict what each line prints. Then remove the first overload of `f`: why does the failure become an error?
- **[Intermediate]** — **The other `template`** (slides 11–13 of *More on templates*). Write `to_float(A)`, which takes any Eigen matrix or vector as a `const Eigen::MatrixBase<Derived> &`, and returns it converted to `float`, with the method `cast<float>()`. Compile it with `-I ${mkEigenInc}`. What does the compiler say without the keyword `template`?
- **[Intermediate]** — **The CRTP**: the [exercise below](#exercise-static-polymorphism-with-the-crtp).

<details>
<summary>Answers (homework)</summary>

- A third difference: `struct Centered { using Other = Centered; };`, and one more branch:
  ```cpp
  if constexpr (std::is_same_v<DT, Forward>)
    return (previous_(x + h_) - previous_(x)) / h_;
  else if constexpr (std::is_same_v<DT, Backward>)
    return (previous_(x) - previous_(x - h_)) / h_;
  else
    return (previous_(x + h_) - previous_(x - h_)) / (2 * h_);
  ```
  The ratio is 4 for all the orders. For $N = 2$ the error is 4 times larger than in Step 3 ($2.8\cdot10^{-5}$ against $7\cdot10^{-6}$): you get $\dfrac{f(x + 2h) - 2 f(x) + f(x - 2h)}{4h^2}$, the same formula with spacing $2h$.
- Ranges: `std::ranges::sort(particles, {}, &Particle::x);` and `std::ranges::max_element(particles, {}, &Particle::mass);`. The `{}` is the comparison: the default one, `<`. The first particle is the one with `id` 1, the heaviest the one with `id` 3.
- SFINAE: 1, 2, 1. In `f<int>(10)` the second overload needs `int::value_type`, which does not exist: it is discarded, without error. In `f<std::vector<int>>(10)` both substitutions work, but `10` cannot become a `std::vector<int>`, because that constructor is `explicit`: the second overload is called, with an `int`. In the third call a vector cannot become an `int`: the first one is called. Without the first overload, `f<int>(10)` gives `no matching function`: the substitution failure only discards a candidate, and an error remains if no candidate is left.
- The other `template`:
  ```cpp
  template <typename Derived>
  auto to_float(const Eigen::MatrixBase<Derived> &A) {
    return A.template cast<float>();
  }
  ```
  Without `template`: `error: expected primary-expression before 'float'`. `A` depends on `Derived`, so the compiler does not know that `cast` is a template, and reads the `<` as *less than*. `typename` says that a name is a type, `template` that it is a template.

</details>

### Exercise: static polymorphism with the CRTP

We have written the shapes twice: with `virtual` in [Lab 3](../03-classes-inheritance/Lab3_Classes_Inheritance.md), with `std::variant` in [Lab 4](../04-class_templates-stl/Lab4_ClassTemplates_STL.md). With the **curiously recurring template pattern** the base class takes its derived class as a template argument (slides 14–17 of *More on templates*):
```cpp
template <typename Derived>
class Shape {
public:
  double area() const { return static_cast<const Derived &>(*this).area(); }
};

class Circle : public Shape<Circle> { /* ... double area() const; ... */ };
```
`class Circle : public Shape<Circle>` is valid C++: the body of `Shape<Circle>::area` is compiled only when it is called, and by then `Circle` is complete. The base class knows the type of the object, so it can call `Circle::area` directly: `static_cast` costs nothing at run time.

| | `virtual` (Lab 3) | `std::variant` (Lab 4) | CRTP |
|---|---|---|---|
| The function is chosen | at run time | at run time | **at compile time** |
| Different shapes in one vector | yes, with pointers | yes, by value | **no** |
| To add a shape | write a class | write a struct, and add it to the variant | write a class |

The same technique is at the heart of Eigen: all its matrices derive from `MatrixBase<Derived>` (slides 11 and 15).

In [`templates/02-crtp/assignment.cpp`](templates/02-crtp/assignment.cpp), `name()` is already delegated to the derived class.
```bash
cd templates/02-crtp
g++ -Wall -std=c++20 assignment.cpp -o assignment && ./assignment
```

**Your tasks**:
1. Write `area()` in `Shape`, following the example of `name()`.
2. Write `equivalent_radius()` in `Shape`, the radius of the circle with the same area, in terms of `area()` (slide 16). You write it once, and each shape has it.
3. Write `Rectangle`, and uncomment the last line of `main`.
4. Try to write `std::vector<Shape> shapes;`, then `std::vector<Shape<Circle>> shapes;`. Could this vector contain a `Square`?
5. **Now break it**: remove `area()` from `Square`. Compile and run. What would happen in Lab 3 if a shape forgot `area()`?
6. How much does polymorphism cost? [`benchmark.cpp`](templates/02-crtp/benchmark.cpp) computes the total area of $10^7$ shapes in the three ways:
   ```bash
   g++ -Wall -std=c++20 -O3 benchmark.cpp -o benchmark && ./benchmark
   ```
   Is the difference only due to the function call?
7. In `solution.cpp`, change the base class of `Square` into `Shape<Circle>`. Does it compile? What does it print? Then make the constructor of `Shape` private and add `friend Derived;`: what changes?

✅ **Checkpoint.**
```
Circle: area 3.14159, equivalent radius 1
Square: area 4, equivalent radius 1.12838
Rectangle: area 6, equivalent radius 1.38198
```
The times depend on the machine, the pattern does not (ms, container, laptop with 8 cores):

| `-O3` | ms |
|---|---|
| `virtual` | 67 |
| `std::variant` | 45 |
| CRTP | **26** |

Solution: [`solution.cpp`](templates/02-crtp/solution.cpp).

<details>
<summary>Answers</summary>

- 1: `double area() const { return self().area(); }`.
- 2: `double equivalent_radius() const { return std::sqrt(area() / std::numbers::pi); }`.
- 4: `Shape` alone is not a type, but a template. `Shape<Circle>` and `Shape<Square>` are two different classes, with no common base: they cannot be in the same vector. This is the price of the CRTP (slide 17): the type of each object must be known at compile time.
- 5: `Square` no longer has its `area()`, so `self().area()` finds the one of the base, `Shape<Square>::area`, which calls itself: `warning: infinite recursion detected`, then `Segmentation fault`. In Lab 3 `area()` was pure virtual, and the compiler refused to create the object. The CRTP is faster, but it checks less.
- 6: no. With the CRTP there is no choice to make at run time, so `area()` is inlined; but the data are also different. `virtual` follows a pointer to the heap for each shape, the `std::variant` stores the shapes in one vector but must look at the active type of each one, the CRTP reads two contiguous vectors. As in the benchmark of Lab 4, the time also depends on where the data are.
- 7: it compiles with no warning, and the `Square` prints `Circle: area 12.5664`: the `static_cast` believes us, and reads the side as a radius. With the private constructor, only `Circle` can build a `Shape<Circle>`: `Square` no longer compiles, with `Shape<Derived>::Shape() ... is private within this context`.

</details>

**Take-away:** use the CRTP in the inner loops, when the types are known at compile time. When you need a collection of different objects chosen at run time, `virtual` is simpler and safer.

**Files on the cluster**: `/home` (10 GB) is for code and small results, `/work` (50 GB) for your projects, `/scratch_local` for jobs that read and write a lot. The scratch folders are deleted after 30 days, and no folder has a backup: keep your code on GitHub.

**Further reading**: on PBS, the slides of the 2025-26 lab, in [`2025-26/05-PBS-algorithms_and_execution_policies`](../../2025-26/05-PBS-algorithms_and_execution_policies). In [`AMSC-CodeExamples/Examples/src`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src): [`Templates/VariadicTemplates`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/Templates/VariadicTemplates), [`CompositionWithVariadicTemplates`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/CompositionWithVariadicTemplates) (slide 3), [`Derivatives`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/Derivatives), [`CRTP`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/CRTP) and [`ExpressionTemplates`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/ExpressionTemplates). A factory that forwards any number of arguments to a constructor, with a variadic template: [`make_jacobian`](../../2025-26/03-cache_align-eigen-variadic_templates-traits-factory/02-factory-variadic_templates-traits/src-solution-1/JacobianFactory.hpp), in the Lab 3 of 2025-26.
