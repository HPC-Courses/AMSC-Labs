# Laboratory 07 — The cluster, parallel algorithms and a first MPI program

### 13/11/2026

Today we log in to the HPC cluster, and use **PBS**, the program that decides when and where our jobs run. Then we put into practice the last part of the lecture on *The Standard Library*, the parallel algorithms, and we write a first program with **MPI** (*Introduction to MPI*).

Each exercise has an `assignment`, to be completed, and a `solution`. The ✅ **Checkpoint** at the end of each section shows the output you should get. Whatever you do not finish in class is listed in the [Homework](#homework).

## Outline

- [0. Setup](#0-setup)
- [1. The cluster and PBS](#1-the-cluster-and-pbs)
- [2. Parallel algorithms](#2-parallel-algorithms)
- [3. A first MPI program](#3-a-first-mpi-program)
- [Homework](#homework)

---

## 0. Setup

Connect to the VPN now: you need it for section 1. Then update the repository and move to the directory of this lab, inside the container:
```bash
cd ~/shared-folder/AMSC-Labs
git pull
cd Labs/2026-27/07-cluster-parallel_algorithms-mpi
nproc
```
The parallel algorithms work **inside the container** (g++ and the library TBB), not with the Apple clang of macOS. If `nproc` prints 1 or 2, give more CPUs to Docker (Docker Desktop: *Settings → Resources*), otherwise you will see no speedup in section 2.

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

For a real computation you write a **job script**: a bash script whose first lines, starting with `#PBS`, contain the options of `qsub`. PBS runs it when the resources are free, also when you are not logged in. The two scripts are in [`cluster`](cluster). [`1_specific_hosts.job`](cluster/1_specific_hosts.job):
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
It asks for two chunks, one CPU on `cpu04` and one on `cpu05`, for at most one minute; `-N` is the name of the job in `qstat`. [`2_scatter.job`](cluster/2_scatter.job) asks for 3 chunks, `place=scatter` puts them on different nodes, and the script prints the names of the nodes, which PBS writes in the file `$PBS_NODEFILE`.

Every option of a script can also be written on the command line of `qsub`, and vice versa: `-l select=1:ncpus=2` on the command line is the line `#PBS -l select=1:ncpus=2` of a script.

A batch job has no terminal, so what it prints has to go to a file. PBS should collect it in `<job name>.o<job id>` and `<job name>.e<job id>`, but on this cluster these files may not arrive: our scripts write their output to a file themselves, with `&>`.

**Your tasks**:
1. Submit the first script and look at the queue. The column `S` is the state: `Q` waiting, `R` running, `E` ending.
   ```bash
   cd ~/AMSC-Labs/Labs/2026-27/07-cluster-parallel_algorithms-mpi/cluster
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
- 3: the first node again: it only reads the names of the 3 nodes. To start a program on all the CPUs of a job you need a launcher such as `mpirun`: we use it in section 3.

</details>

Keep this terminal open: in sections 2 and 3 you will submit two more jobs.

---

## 2. Parallel algorithms

So far our programs have run on one **thread**: one sequence of instructions, executed one after the other by one core. But a laptop has several cores, which can work at the same time. To work **in parallel** means to divide the work among several threads, one for each core. For example, the sum of 8 numbers:
```text
One thread:                   time -->
  core 0 | x0 x1 x2 x3 x4 x5 x6 x7 |

Four threads:                 time -->
  core 0 | x0 x1 |--+
  core 1 | x2 x3 |--+--> sum the 4 partial sums
  core 2 | x4 x5 |--+
  core 3 | x6 x7 |--+
```
Each core does a quarter of the work, so the sum can take about a quarter of the time. There is a price: the partial results must be combined, and the threads must not get in each other's way.

The Standard Library has parallel algorithms of two kinds (*The Standard Library*, slides *Parallel algorithms* to *A final comment on parallel algorithms*):

| | Result | Examples | What you must check |
|---|---|---|---|
| **Algorithms that combine** | one value | `std::reduce`, `std::transform_reduce`, `std::count_if` | the result must not depend on the order of the operations |
| **All the others** | a range | `std::transform`, `std::for_each`, `std::sort` | your lambda must not write into a shared variable |

The first kind has a parallel twin of two functions of Lab 5: `std::reduce` for `std::accumulate`, and `std::transform_reduce` for `std::inner_product` (slide *`std::transform_reduce`*). `reduce` combines the elements as they are; `transform_reduce` first *transforms* them, then combines the results: `std::transform_reduce(b, e, 0., std::plus<double>(), square)` is $x_0^2 + x_1^2 + \dots$, and we use it in section 2.2.

`accumulate` is defined to work **from left to right**, $((init + x_0) + x_1) + \dots$. `reduce` may combine the elements **in any order**, even on one thread, and this is correct only if the operation is **associative** and **commutative**, as `+`, `*`, `max`. With `-` the two give different numbers: $(10 - 5) - 2 = 3$, but $10 - (5 - 2) = 7$. The threads arrive in section 2.1.

### 2.1 Execution policies and TBB

Many algorithms have a version that takes an **execution policy** as the first argument (slides *Parallel algorithms* and *Execution policies*):
```cpp
#include <execution>

std::sort(v.begin(), v.end());                         // as usual
std::sort(std::execution::par, v.begin(), v.end());    // may use several threads
```

| Policy | Meaning |
|---|---|
| `std::execution::seq` | one thread: as with no policy |
| `std::execution::par` | the elements may be divided among several threads |
| `std::execution::unseq` (C++20) | one thread, which may use vector instructions |
| `std::execution::par_unseq` | several threads, and each one may also use vector instructions |

A **vector instruction** does the same operation on several numbers at the same time, on one core. How many numbers depends on the CPU; here 4:
```text
par: each core handles one element at a time           time -->
  core 0 | x0 | x1 | x2 | x3 |       4 steps
  core 1 | x4 | x5 | x6 | x7 |       4 steps

par_unseq: each core may also handle 4 elements with one instruction
  core 0 | x0 x1 x2 x3 |             1 step
  core 1 | x4 x5 x6 x7 |             1 step
```
Vector instructions have nothing to do with `std::vector`, nor with `std::reduce`. An algorithm says *what* to compute; the policy says *how* it may be run, so the two can be combined: `std::reduce(std::execution::par_unseq, b, e, 0.)`.

The policy is a *permission*, not an order: the library may still run on one thread. To run a `for` loop in parallel you have to rewrite it; to run an algorithm in parallel you add one argument.

**What is TBB?** Somebody has to create the threads and divide the work among them. g++ does not do it itself: it asks **TBB** (Threading Building Blocks), a C++ library that creates a group of threads, splits the work into small tasks and gives them to the threads. TBB is not part of the compiler: as any external library, it has to be linked, with `-ltbb`.

**When do you need it?** It depends on the policy, not on the algorithm: only `par` and `par_unseq` use several threads.

| Call | Threads | `-ltbb` |
|---|---|---|
| `std::reduce(b, e, 0.)`, `std::sort(b, e)` | one | not needed |
| `std::reduce(std::execution::seq, b, e, 0.)` | one | not needed |
| `std::reduce(std::execution::par, b, e, 0.)`, `std::sort(std::execution::par, b, e)` | several | **needed** |

With `std::execution::par`, `reduce` needs TBB as `sort` does.

### 2.2 How much faster?

[`parallel/02-policies/assignment.cpp`](parallel/02-policies/assignment.cpp) runs four algorithms on $10^7$ random numbers, with `seq`, `par` and `par_unseq`. The policy is the parameter `policy` of `run`, but the algorithms do not use it yet.
```bash
cd ../02-policies
g++ -Wall -std=c++20 -O3 assignment.cpp -o assignment -ltbb
./assignment
```

**Your tasks**:
1. Add `policy` as the first argument of the four algorithms in `run`, compile and run.
2. **On the cluster**: [`policies.job`](parallel/02-policies/policies.job) asks for 8 CPUs, loads gcc and TBB, compiles `solution.cpp` and runs it. In the terminal connected to the cluster, submit it now, and go on with the next tasks:
   ```bash
   cd ~/AMSC-Labs/Labs/2026-27/07-cluster-parallel_algorithms-mpi/parallel/02-policies
   qsub policies.job
   ```
3. Which algorithms are faster with `par`? How much, compared to the number of threads?
4. Is the `sum of the squares` the same for the three policies? And if you run again?
5. Compile without `-ltbb`: what happens?
6. When the job is over, compare `policies.<job id>.out` with your laptop. Look at the first lines of `main`: why do we need `tbb::global_control`?

✅ **Checkpoint.** The times depend on the machine, the pattern does not (container, laptop with 8 cores):

| ms | `seq` | `par` | `par_unseq` |
|---|---|---|---|
| `y = 2 x + 1` | 3.6 | 4.0 | 4.1 |
| `y = sin(x) exp(-x/10)` | 317 | **63** | **66** |
| sum of the squares | 9.1 | 2.2 | 3.0 |
| sort | 935 | **228** | **211** |

The sum is `333434181.77666193` with `seq`; with `par` and `par_unseq` the last digits differ. Solution: [`solution.cpp`](parallel/02-policies/solution.cpp).

<details>
<summary>Answers</summary>

- 3: `sin(x) exp(-x/10)` and the sort do many operations for each number, and are about 4–5 times faster with 8 threads. `2 x + 1` does two operations for each number: the time is spent reading and writing memory, which is shared by all the cores. As in the benchmark of Lab 4, the time also depends on how the data reach the CPU.
- 4: with `par` each thread sums its own part, then the partial sums are added: the order of the additions changes, and with floating-point numbers $(a + b) + c$ is not always $a + (b + c)$. How the work is divided can change at each run, and so can the last digits.
- 5: the linker fails with `undefined reference to tbb::...`.
- 6: the job prints `CPUs reserved: 8` and `Threads: 8`. PBS reserves 8 CPUs, but it does not stop a program from using all the 112 threads of the node: `tbb::global_control` limits TBB to `$NCPUS`, the CPUs we asked for. The pattern of the times is the same as on the laptop, but the speedup is smaller and the times change from run to run: the node is shared with other jobs.

</details>

### 2.3 Now break it: a data race

A parallel algorithm calls your lambda from several threads at the same time. If two calls change **the same variable**, the result is wrong: a **data race**. Avoiding it is your responsibility: *each invocation must write a distinct element without modifying shared mutable state* (slide *From an indexed loop to `std::for_each`*).

[`parallel/03-data_race/data_race.cpp`](parallel/03-data_race/data_race.cpp) counts the even numbers among $0, \dots, 10^7 - 1$ with a loop, with a `std::for_each(std::execution::par, ...)` that increments a shared counter, and with `std::count_if(std::execution::par, ...)`:
```bash
cd ../03-data_race
g++ -Wall -std=c++20 -O3 data_race.cpp -o data_race -ltbb
./data_race
```
Run it a few times. Why is `for_each` wrong, and different every time? Why is `count_if` right?

✅ **Checkpoint.** (The wrong number changes at every run.)
```
Expected:            5000000
Loop:                5000000
for_each, par:       3817194
count_if, par:       5000000
```

<details>
<summary>Answers</summary>

`++count` is three operations: read `count`, add 1, write `count`. If two threads read 41, both write 42, and one increment is lost; how many are lost depends on how the threads happen to run. `count_if` shares nothing: each thread counts its own part, and the library sums the partial counts. **To combine all the elements into one result, use the algorithm that does it** (`count_if`, `reduce`, `transform_reduce`), not a `for_each` that writes into a shared variable.

</details>

---

## 3. A first MPI program

The threads of section 2 belong to one program, and share its memory: all of them can read `x`, and all of them can write into the same `count`. **MPI** (Message Passing Interface) works in another way: `mpirun` starts several **processes**, several copies of the same program, and each one has **its own memory**. A process cannot read the variables of another one: to exchange data, they send each other **messages** (*Introduction to MPI*, slides *What is MPI?* and *Single Program Multiple Data*). This is what a cluster needs: each node has its own memory.
```text
Threads (section 2)                     Processes (MPI)
  one program, one memory                 four copies of the program, four memories
  +-----------------------+               +--------+ +--------+ +--------+ +--------+
  | thread 0   thread 1   |               | rank 0 | | rank 1 | | rank 2 | | rank 3 |
  | thread 2   thread 3   |               |  sum   | |  sum   | |  sum   | |  sum   |
  |         count         |               +--------+ +--------+ +--------+ +--------+
  +-----------------------+                    <------- messages ------->
```
All the processes run the same `main`, from the first line to the last. To make them do different things, each process asks for its **rank**, its number from 0 to `size - 1`, and uses it in an `if`, or to choose its own part of the work.

| Code | What it does |
|---|---|
| `#include <mpi.h>` | all the MPI functions, whose names start with `MPI_` |
| `MPI_Init(&argc, &argv);` | the first MPI call of the program |
| `MPI_Comm_size(MPI_COMM_WORLD, &size);` | writes into `size` the number of processes |
| `MPI_Comm_rank(MPI_COMM_WORLD, &rank);` | writes into `rank` the rank of this process |
| `MPI_Reduce(&sum, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);` | each process sends its `sum`, and rank 0 receives their total in `total` |
| `MPI_Finalize();` | the last MPI call of the program |
| `mpic++ -Wall -std=c++20 main.cpp -o main` | compiles: `g++`, with the flags of the MPI library |
| `mpirun -n 4 ./main` | runs `main` with 4 processes |

`MPI_COMM_WORLD` is a **communicator**: the group of all the processes started by `mpirun`. MPI is a C library: its functions do not return the value, they write it into the `int` whose address you pass, `&size`.

`MPI_Reduce` is the MPI twin of `std::reduce` (section 2), and the first function we see that sends **messages**: each process sends its partial result, and one process, the **root**, receives them combined with an operation (slide *`MPI_Reduce()`*). Its arguments, in order: what each process sends, where the root writes the result, how many numbers, their type (`MPI_INT`, `MPI_DOUBLE`, ...), the operation (`MPI_SUM`, `MPI_MAX`, ...), the rank of the root, and the communicator. It is a **collective** call: **all** the processes of the communicator must call it, not only the root.
```text
rank 0: sum = 325  --+
rank 1: sum = 950  --+--> MPI_SUM --> rank 0: total = 5050
rank 2: sum = 1575 --+
rank 3: sum = 2200 --+
```

[`mpi/01-hello/assignment.cpp`](mpi/01-hello/assignment.cpp) compiles and runs, but each process does not know yet who it is:
```bash
cd ../../mpi/01-hello
mpic++ -Wall -std=c++20 assignment.cpp -o assignment
mpirun -n 4 ./assignment
```
If `mpirun` says `There are not enough slots available in the system`, Docker has fewer than 4 CPUs: run with `-n 2`, or add `--oversubscribe`.

**Your tasks**:
1. Run the assignment as it is, with `mpirun -n 4`. Why are there 4 lines? Why do all of them say `rank 0 of 1`?
2. TODO 1: ask MPI for `size` and `rank`. Run the program a few times: in which order are the lines printed?
3. TODO 2: only rank 0 prints `Running with 4 processes`.
4. TODO 3: each process sums its own part of $1, \dots, 100$; `first` and `last` are already computed. Is the total of the 4 sums 5050? Which process could compute it?
5. TODO 4: the total on rank 0, with `MPI_Reduce`; rank 0 prints it. Then:
   - print `total` on all the ranks, without the `if`: what do ranks 1, 2 and 3 print?
   - put the `if (rank == 0)` back, also before `MPI_Reduce`: only rank 0 calls it. What happens? (Stop the program with `ctrl+C`.)
6. **Now break it**: run with `-n 3`. Is the total still 5050? Fix `last`. Then try `mpirun -n 16`, and `./assignment` without `mpirun`.
7. *(If you have time)* **On the cluster**: [`hello_mpi.job`](mpi/01-hello/hello_mpi.job) asks for 4 CPUs and 4 MPI processes, loads gcc and Open MPI, compiles `solution.cpp` with `mpic++`, and runs it with `mpirun -n 4`. Submit it from `~/AMSC-Labs/Labs/2026-27/07-cluster-parallel_algorithms-mpi/mpi/01-hello`, then read `hello_mpi.<job id>.out`. Where do the processes run?

✅ **Checkpoint.** With `mpirun -n 4`, after task 5 (the lines arrive in another order at each run, `total` is printed after `rank 0: sum ...`, and the name is the one of your container):
```
Hello from rank 0 of 4, on e220287f5e17
Running with 4 processes
rank 0: sum of 1, ..., 25 = 325
Hello from rank 1 of 4, on e220287f5e17
rank 1: sum of 26, ..., 50 = 950
Hello from rank 2 of 4, on e220287f5e17
rank 2: sum of 51, ..., 75 = 1575
Hello from rank 3 of 4, on e220287f5e17
rank 3: sum of 76, ..., 100 = 2200
total = 5050
```
With `-n 3`, after the fix of task 6: `561`, `1650`, `2839` and `total = 5050`. Solution: [`solution.cpp`](mpi/01-hello/solution.cpp).

<details>
<summary>Answers</summary>

- 1: `mpirun -n 4` starts 4 copies of the program, and each one runs all of `main`: 4 lines. Nobody has asked MPI for the size and the rank yet, so each copy prints the values it was initialised with, 1 and 0. The 4 copies do not know about each other.
- 2: `MPI_Comm_size(MPI_COMM_WORLD, &size);` and `MPI_Comm_rank(MPI_COMM_WORLD, &rank);`. The order changes from run to run: the processes are independent, and each one prints when it gets there (slide *An important note*).
- 3: `if (rank == 0)` before the `std::cout`. All the processes run the `if`, only one enters it.
- 4: $325 + 950 + 1575 + 2200 = 5050$. No process can compute it by itself: each `sum` is in the memory of its own process. Rank 0 has to receive the other three sums, with a message: task 5.
- 5: all the processes call
  ```cpp
  int total = 0;
  MPI_Reduce(&sum, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  if (rank == 0)
    std::cout << "total = " << total << std::endl;
  ```
  Without the `if`, ranks 1, 2 and 3 print `total = 0`: only the root receives the result, and the `total` of the others is never written. If every process needs the result, use `MPI_Allreduce` (slide *Another form of MPI reduction*). If only rank 0 calls `MPI_Reduce`, the program never ends: rank 0 waits for three messages that nobody sends. There is no error message: the processes just wait for each other, a **deadlock** (slide *Deadlock*).
- 6: with 3 processes `block` is 33, and the three parts are $1, \dots, 33$, $34, \dots, 66$ and $67, \dots, 99$: `total = 4950`, nobody sums 100. One fix: the last process also takes what is left,
  ```cpp
  const int last = (rank == size - 1) ? n : first + block - 1;
  ```
  A better one, which gives the remainder to the first processes, one number each, is in the slide *Types of block partitioning* of *Introduction to parallel computing*. With `-n 16` on a laptop with 8 cores: `There are not enough slots available in the system`. Open MPI starts at most one process for each core; `--oversubscribe` removes the limit, but then the processes share the cores. Without `mpirun` there is one process: `rank 0 of 1`, which sums all the numbers, 5050.
- 7: the 4 processes print the same compute node, e.g. `cpu04.mate.polimi.it`: the job asks for one chunk, `select=1:ncpus=4:mpiprocs=4`, so all of them are on one node. Unlike `1_specific_hosts.job` of section 1.4, the program does not run once: `mpirun` starts it on all the CPUs of the job.

</details>

---

## Homework

Each exercise has a difficulty label: **[Easy]**, **[Intermediate]** or **[Hard]**.
- **Sections 1–3**, if you did not finish them in class.
- **[Easy]** — **`reduce` and `transform_reduce`.** In [`parallel/01-reduce/assignment.cpp`](parallel/01-reduce/assignment.cpp) each exercise shows the Lab 5 solution, or a loop, in a comment. Compile it with `g++ -Wall -std=c++20 assignment.cpp -o assignment`: (1) the sum of the entries of `x`, with `std::reduce`; (2) the dot product of `x` and `y`, with `std::transform_reduce`; (3) the largest entry of `x`, with `std::reduce` and a lambda; (4) uncomment the two lines with `std::minus` at the end of `main`: do `accumulate` and `reduce` agree? Solution: [`solution.cpp`](parallel/01-reduce/solution.cpp). The output should be `Exercise 1: PASSED`, `Exercise 2: PASSED`, `Exercise 3: PASSED`, `accumulate: -8.5, reduce: -16.5`.
- **[Easy]** — **Derivatives on the cluster.** The exercise of [Lab 6](../06-algorithms-templates/Lab6_Algorithms_Templates.md#2-exercise-derivatives-of-any-order) has a job script, [`derivatives.job`](../06-algorithms-templates/templates/01-derivatives/derivatives.job), that compiles `solution` on a compute node, with the gcc of spack, and runs it. On the cluster, update the repository with `git pull`, move to `~/AMSC-Labs/Labs/2026-27/06-algorithms-templates/templates/01-derivatives` and submit it with `qsub derivatives.job`. When `qstat -u $(whoami)` no longer shows it, read `derivatives.<job id>.out`.
- **[Easy]** — **How many CPUs?** `qsub -l select=1:ncpus=2 policies.job` overrides the `#PBS -l` of the script. Submit `policies.job` with 1, 2, 4 and 8 CPUs: how does the time of `sin(x) exp(-x/10)` with `par` change?
- **[Easy]** — **What is `mpic++`?** Run `mpic++ --showme` in the container. Which flags does it add to `g++`? Compile `mpi/01-hello/solution.cpp` with `g++`, first without these flags, then with them, and run it with `mpirun`.
- **[Intermediate]** — **A parallel matrix-vector product**: the [exercise below](#exercise-a-parallel-matrix-vector-product).
- **[Intermediate]** — **`row_ptr` with `std::partial_sum`.** The [CSR format](../04-class_templates-stl/Lab4_ClassTemplates_STL.md#exercise-the-csr-format) stores in `row_ptr` where each row starts. Given the vector `rows`, with the row of each entry, build `row_ptr` by counting the entries of each row, then with one `std::partial_sum` (in `<numeric>`, it writes `out[i] = a[0] + ... + a[i]`). How do you go back to the number of entries of each row? For the 5×5 tridiagonal matrix, `row_ptr` is `0 2 5 8 11 13`.

<details>
<summary>Answers (homework)</summary>

- `reduce` and `transform_reduce`: for (3), `std::reduce(x.begin(), x.end(), x[0], [](double a, double b) { return std::max(a, b); })`. For (4), no: `-` is neither associative nor commutative, and `reduce` may group the elements in any order, even on one thread. `accumulate` goes from left to right and subtracts all the entries from 0: $-8.5$. With g++, `reduce` takes the elements 4 at a time:
  ```text
  3, -1, 4, -1.5:  (3 - (-1)) - (4 - (-1.5)) = 4 - 5.5   = -1.5    total: 0 - (-1.5) = 1.5
  5, -9, 2, 6:     (5 - (-9)) - (2 - 6)      = 14 - (-4) = 18      total: 1.5 - 18   = -16.5
  ```
  The numbers are the same, the parentheses are not, and another compiler may give another number. **Pass to `reduce` only operations for which the order does not matter.**
- Derivatives on the cluster: the first line of the output is the version of the compiler, 15.2.0, and the table has the same pattern as on your laptop. The code is compiled on the compute node, never on the login node.
- How many CPUs: the time goes down when you add CPUs, because `main` limits TBB to `$NCPUS`, but it is not halved each time you double them, and it changes from run to run: the node is shared with other jobs. Check the `Threads:` printed by the job.
- `mpic++ --showme` prints `g++ -I/usr/lib/x86_64-linux-gnu/openmpi/include -I/usr/lib/x86_64-linux-gnu/openmpi/include/openmpi -L/usr/lib/x86_64-linux-gnu/openmpi/lib -lmpi_cxx -lmpi`: the folder of `mpi.h`, the folder of the library, and the library. With `g++` alone you get `fatal error: mpi.h: No such file or directory`. `mpic++` is only a shortcut: MPI is a library, as TBB.
- `row_ptr`:
  ```cpp
  std::vector<std::size_t> row_ptr(nrows + 1, 0);
  for (std::size_t i : rows)
    ++row_ptr[i + 1];
  std::partial_sum(row_ptr.begin(), row_ptr.end(), row_ptr.begin());
  ```
  `std::adjacent_difference` is the inverse of `std::partial_sum`: on `row_ptr` it gives `0 2 3 3 3 2`.

</details>

### Exercise: a parallel matrix-vector product

The **CSR** format, the [homework of Lab 4](../04-class_templates-stl/Lab4_ClassTemplates_STL.md#exercise-the-csr-format), is already written in [`parallel/04-parallel_vmult/assignment/csr_matrix.hpp`](parallel/04-parallel_vmult/assignment/csr_matrix.hpp). The entries of row `i` are in the positions from `row_ptr_[i]` to `row_ptr_[i+1] - 1` of `values_` and `col_idx_`, and `vmult` is a loop over the rows:
```cpp
for (std::size_t i = 0; i < this->nrows(); ++i)
  y[i] = row_times_x(i, x);   // the product of row i with x
```
Each row writes only its own `y[i]`: the rows can be computed in parallel. An algorithm needs a range, so the constructor fills `rows_` with $0, 1, \dots, n-1$ using `std::iota`, and we loop over it, as in the slide *From an indexed loop to `std::for_each`*:
```cpp
std::for_each(std::execution::par, rows_.begin(), rows_.end(),
              [&](std::size_t i) { /* the body of the loop, for row i */ });
```

```bash
cd parallel/04-parallel_vmult/assignment
make run
```
`make run` runs the tests with AddressSanitizer, `make benchmark` times `vmult` and `vmult_parallel` with $n = 2\cdot10^6$ rows.

**Your tasks**:
1. Write `vmult_parallel` in `csr_matrix.hpp`, with `std::for_each` and `std::execution::par`.
2. Run `make run`, then `make benchmark`. Is `vmult_parallel` 8 times faster?
3. Why does `vmult_parallel` not have the data race of section 2.3?
4. Would `std::for_each(std::execution::par, ...)` on `entries_` work for the `vmult` of the COO (Lab 5)?
5. Write `vmult_parallel` again, with `std::transform(std::execution::par, ...)`, writing `row_times_x(i, x)` into `y`. Why is it even safer?

✅ **Checkpoint.**
```
vmult:          PASSED
vmult_parallel: PASSED

Benchmark, n = 2000000, threads: 8
vmult:          7.2 ms
vmult_parallel: 3.3 ms
same result:    PASSED
```
Solution: [`parallel/04-parallel_vmult/solution`](parallel/04-parallel_vmult/solution).

<details>
<summary>Answers</summary>

- 1: `std::for_each(std::execution::par, rows_.begin(), rows_.end(), [&](std::size_t i) { y[i] = row_times_x(i, x); });`. The lambda captures by reference: it must write into `y`, not into a copy.
- 2: about 2 times faster, not 8: as `2 x + 1` in section 2.2, there are two operations for each entry read from memory.
- 3: each call writes only `y[i]`, a different element for each `i`.
- 4: no. The entries of the same row are different elements of `entries_`, and all of them do `y[e.row] += ...`: two threads can update the same `y[e.row]` at the same time. In the CSR each row is handled by one thread.
- 5: `std::transform(std::execution::par, rows_.begin(), rows_.end(), y.begin(), [&](std::size_t i) { return row_times_x(i, x); });`. The lambda writes nothing: it returns a value, and the algorithm stores it in `y`.

</details>

**Files on the cluster**: `/home` (10 GB) is for code and small results, `/work` (50 GB) for your projects, `/scratch_local` for jobs that read and write a lot. The scratch folders are deleted after 30 days, and no folder has a backup: keep your code on GitHub.

**Further reading**: the slides of the 2025-26 lab, in [`2025-26/05-PBS-algorithms_and_execution_policies`](../../2025-26/05-PBS-algorithms_and_execution_policies); in [`AMSC-CodeExamples/Examples/src`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src): [`STL/Reduce`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/STL/Reduce) and [`Parallel/MPI/HelloWorld`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/Parallel/MPI/HelloWorld); **Thrust** (slides *Thrust: a CUDA data-parallel library* and following), the same algorithms on an NVIDIA GPU. On MPI, the reference suggested in the lecture: [RookieHPC](https://rookiehpc.github.io/mpi/docs/index.html).
