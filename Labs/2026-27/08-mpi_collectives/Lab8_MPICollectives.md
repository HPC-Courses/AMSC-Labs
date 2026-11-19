# Laboratory 08 — MPI: collective communications

### 20/11/2026

In Lab 7 each MPI process built its own part of the data. Today one process has all the data, and sends a part to each of the others, with the **collective** functions of MPI (*Introduction to MPI*): `MPI_Bcast`, `MPI_Scatter`, `MPI_Gather`, `MPI_Allreduce`, and their versions for blocks of different sizes, `MPI_Scatterv`, `MPI_Gatherv` and `MPI_Allgatherv`. We use them for an iterative method, where all the processes need all of a vector at each iteration. At the end, a message from one process to another, with `MPI_Send` and `MPI_Recv`, and the deadlock it can cause.

Each exercise has an `assignment`, to be completed, and a `solution`. The ✅ **Checkpoint** at the end of each section shows the output you should get. Whatever you do not finish in class is listed in the [Homework](#homework).

## Outline

- [0. Setup](#0-setup)
- [1. Scatter, gather and allreduce](#1-scatter-gather-and-allreduce)
- [2. All the processes need all the data: `MPI_Allgatherv`](#2-all-the-processes-need-all-the-data-mpi_allgatherv)
- [3. From one process to another: a ring of messages](#3-from-one-process-to-another-a-ring-of-messages)
- [Homework](#homework)

---

## 0. Setup

Update the repository and move to the directory of this lab, inside the container:
```bash
cd ~/shared-folder/AMSC-Labs
git pull
cd Labs/2026-27/08-mpi_collectives
nproc
```
We run with up to 4 processes. If `nproc` prints less than 4, give more CPUs to Docker (Docker Desktop: *Settings → Resources*).

---

## 1. Scatter, gather and allreduce

In Lab 7 each process computed its own numbers $first, \dots, last$: nothing had to be sent, except the partial sums to rank 0. Often, instead, one process has all the data, because it has read them from a file, and the others have nothing. MPI has **collective** functions for this (slides *Some collective communications: broadcast, scatter and reduce* to *Got it?*). For a vector of 8 numbers and 4 processes:
```text
MPI_Bcast: the same data to all          MPI_Scatter: a different block to each

rank 0: n = 8                            rank 0: x = 1 2 3 4 5 6 7 8
   |                                        |
   +--> rank 0, 1, 2, 3: n = 8              +--> rank 0: 1 2   rank 1: 3 4   rank 2: 5 6   rank 3: 7 8

MPI_Gather: the opposite of scatter      MPI_Allreduce: MPI_Reduce, with the result on all

rank 0: a b   rank 1: c d  ...           rank 0: 5   rank 1: 25   rank 2: 61   rank 3: 113
   |                                        |
   +--> rank 0: x = a b c d ...             +--> rank 0, 1, 2, 3: 204
```

| Code | What it does |
|---|---|
| `MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);` | rank 0 sends `n` to all the processes. On rank 0, `n` is what is sent; on the others it is where it is received |
| `MPI_Bcast(x.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);` | the same, with a vector: rank 0 sends all the `n` numbers of `x`. On the other processes `x` must already have `n` elements: MPI writes into it, it does not resize it |
| `MPI_Scatter(x.data(), k, MPI_DOUBLE, local_x.data(), k, MPI_DOUBLE, 0, MPI_COMM_WORLD);` | rank 0 divides `x` into blocks of `k` numbers: the first block goes to rank 0, the second to rank 1, ... |
| `MPI_Gather(local_x.data(), k, MPI_DOUBLE, x.data(), k, MPI_DOUBLE, 0, MPI_COMM_WORLD);` | the opposite: rank 0 receives `k` numbers from each process, and puts them one after the other in `x` |
| `MPI_Allreduce(&a, &b, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);` | as `MPI_Reduce`, but **all** the processes receive the result in `b`: there is no root |

MPI is a C library: it does not know `std::vector`. It needs the address of the first number, `x.data()`, and how many numbers there are. In `MPI_Scatter` and `MPI_Gather`, `k` is the number for **each** process, not the size of `x` (slide *`MPI_Scatter()`*). As `MPI_Reduce`, these functions are **collective**: all the processes call them, with the same root.

[`mpi/01-normalize/assignment.cpp`](mpi/01-normalize/assignment.cpp) normalises a vector, $x / |x|$, where $|x| = \sqrt{x_0^2 + x_1^2 + \dots}$. Only rank 0 has `x`, $1, 2, \dots, 8$:
```bash
cd mpi/01-normalize
mpic++ -Wall -std=c++20 assignment.cpp -o assignment
mpirun -n 4 ./assignment
```

**Your tasks**:
1. Run the assignment as it is. Why does rank 0 print `local_x = 0 0`, and the other ranks an empty `local_x`?
2. TODO 1 and TODO 2: send `n` to all the processes with `MPI_Bcast`, then a block of `x` to each one with `MPI_Scatter`. Each rank prints its two numbers.
3. TODO 3: the norm on all the processes with `MPI_Allreduce`; each process divides its `local_x` by the norm. Then try `MPI_Reduce` (root 0) instead of `MPI_Allreduce`: which norm do ranks 1, 2 and 3 print?
4. TODO 4: rank 0 collects the blocks into `x` with `MPI_Gather`, and prints it.
5. **Now break it**: run with `-n 3`. Is the norm right? Which numbers of `x` are not normalised, and why?
6. Fix it with `MPI_Scatterv` and `MPI_Gatherv`, which send a different number of elements to each process (slides *Scattering and Gathering with irregular data* to *Got it?*). Build two vectors of `size` numbers: `counts`, how many numbers each process gets, and `displacements`, where its block starts in `x`. With 3 processes, `counts = 3 3 2` and `displacements = 0 3 6`. Section 2 needs them.

| Code | What it does |
|---|---|
| `MPI_Scatterv(x.data(), counts.data(), displacements.data(), MPI_DOUBLE, local_x.data(), local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);` | rank `r` receives `counts[r]` numbers of `x`, from the position `displacements[r]` |
| `MPI_Gatherv(local_x.data(), local_n, MPI_DOUBLE, x.data(), counts.data(), displacements.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);` | the opposite |

With 3 processes and $n = 8$, on rank 0:
```text
position        0   1   2   3   4   5   6   7
x             [ 1   2   3 | 4   5   6 | 7   8 ]
                 rank 0      rank 1    rank 2

counts              3           3         2       how many numbers in each block
displacements   0           3           6         where each block starts
```
Each block starts where the previous one ends: `displacements[r] = displacements[r - 1] + counts[r - 1]`. `MPI_Scatterv` sends to rank `r` the `counts[r]` numbers of `x` from the position `displacements[r]`; `MPI_Gatherv` writes them back there. With 4 processes, `counts = 2 2 2 2` and `displacements = 0 2 4 6`.

✅ **Checkpoint.** With `-n 4`, after task 4 (the lines of the ranks arrive in another order at each run):
```
x = 1 2 3 4 5 6 7 8
rank 0: local_x = 1 2
rank 1: local_x = 3 4
rank 2: local_x = 5 6
rank 3: local_x = 7 8
rank 0: norm = 14.2829
rank 1: norm = 14.2829
rank 2: norm = 14.2829
rank 3: norm = 14.2829
x / |x| = 0.07 0.14 0.21 0.28 0.35 0.42 0.49 0.56
```
With `-n 3`, after task 6: `local_x = 1 2 3`, `4 5 6` and `7 8`, and the same `x / |x|`. Solution: [`solution.cpp`](mpi/01-normalize/solution.cpp).

<details>
<summary>Answers</summary>

- 1: nobody has sent anything yet. On ranks 1, 2 and 3, `n` is still 0, so `local_n = 0 / 4 = 0`, and `local_x` is empty. On rank 0, `n = 8` and `local_n = 2`: `local_x` has 2 numbers, but nobody has written them, and they are 0.
- 2: `MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);` before `local_n` is computed: `local_n` needs the right `n`. Then
  ```cpp
  MPI_Scatter(x.data(), local_n, MPI_DOUBLE, local_x.data(), local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  ```
  On ranks 1, 2 and 3, `x` is empty, and that is fine: what to send is read only on the root (slide *`MPI_Scatter()`*).
- 3:
  ```cpp
  double sum = 0;
  MPI_Allreduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  const double norm = std::sqrt(sum);
  for (double &xi : local_x)
    xi /= norm;
  ```
  With `MPI_Reduce`, ranks 1, 2 and 3 print `norm = 0`: only the root receives the result. They divide by 0, and rank 0 prints `x / |x| = 0.07 0.14 inf inf inf inf inf inf`. Each process needs the norm, so here `MPI_Allreduce` is needed. When only one process needs the result, use `MPI_Reduce`: it sends fewer messages (slide *Another form of MPI reduction*).
- 4: `MPI_Gather(local_x.data(), local_n, MPI_DOUBLE, x.data(), local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);`, then `if (rank == 0)` print `x`. On rank 0, `x` already has 8 elements: `MPI_Gather` writes into them, it does not resize `x`.
- 5: with 3 processes `local_n = 8 / 3 = 2`. `MPI_Scatter` sends $1, \dots, 6$, and 7 and 8 are sent to nobody. The norm is $\sqrt{1 + 4 + \dots + 36} = 9.54$, and `MPI_Gather` writes only the first 6 numbers: `x / |x| = 0.105 0.21 0.314 0.419 0.524 0.629 7 8`. It is the bug of task 6 in Lab 7, section 3. There is no error message: MPI sends what you ask for.
- 6: after `MPI_Bcast`, all the processes know `n`, so each one can compute `counts` by itself. In the slide *Scattering a vector `v` of size `n`*, rank 0 computes it and sends it with `MPI_Scatter`; both ways work.
  ```cpp
  std::vector<int> counts(size);
  std::vector<int> displacements(size, 0);
  for (int r = 0; r < size; ++r) {
    counts[r] = n / size + (r < n % size ? 1 : 0);
    if (r > 0)
      displacements[r] = displacements[r - 1] + counts[r - 1];
  }
  const int local_n = counts[rank];
  ```
  With 4 processes `counts = 2 2 2 2`: `MPI_Scatterv` does the same as `MPI_Scatter`.

</details>

---

## 2. All the processes need all the data: `MPI_Allgatherv`

In section 1 each process worked only on its own block of `x`. In a matrix-vector product $y = A x$ divided by rows, this is not enough: each process computes its rows of $y$, but each row of $A$ multiplies **all** of $x$. If $x$ is divided in blocks, as $y$, each process must receive the blocks of all the others. `MPI_Allgather` and `MPI_Allgatherv` do it: they are `MPI_Gather` and `MPI_Gatherv` with the result on all the processes, as `MPI_Allreduce` is `MPI_Reduce` with the result on all (slide *All-to-all communications*):
```text
MPI_Allgatherv: MPI_Gatherv, with the result on all

rank 0: a b c   rank 1: d e f   rank 2: g h
   |
   +--> rank 0, 1, 2: x = a b c d e f g h
```

| Code | What it does |
|---|---|
| `MPI_Allgatherv(local_x.data(), local_n, MPI_DOUBLE, x.data(), counts.data(), displacements.data(), MPI_DOUBLE, MPI_COMM_WORLD);` | each process sends its `local_n` numbers to all the others. All the processes receive the block of rank `r` in `x`, from the position `displacements[r]`. There is no root |
| `MPI_Allgather(local_x.data(), k, MPI_DOUBLE, x.data(), k, MPI_DOUBLE, MPI_COMM_WORLD);` | the same, with blocks of `k` numbers |

The **power method** computes the largest eigenvalue $\lambda_1$ of a matrix $A$, and its eigenvector $v_1$. Start from a vector $x$ with $|x| = 1$, and repeat
$$y = A x, \qquad \lambda = |y|, \qquad x = y / \lambda.$$
$\lambda$ tends to $\lambda_1$, and $x$ to $v_1$. Why? Write the first $x$ with the eigenvectors of $A$, $x = c_1 v_1 + c_2 v_2 + \dots$. Each product by $A$ multiplies each term by its eigenvalue, $A v_i = \lambda_i v_i$, so after $k$ products
$$A^k x = c_1 \lambda_1^k v_1 + c_2 \lambda_2^k v_2 + \dots = \lambda_1^k \left( c_1 v_1 + c_2 \left(\tfrac{\lambda_2}{\lambda_1}\right)^k v_2 + \dots \right).$$
If $|\lambda_2| < |\lambda_1|$, all the terms but the first go to 0 as $(\lambda_2 / \lambda_1)^k$: $x$ turns towards $v_1$. Dividing by $\lambda = |y|$ does not change the direction of $x$: it only keeps $|x| = 1$, otherwise the numbers grow as $\lambda_1^k$. When $x = v_1$, $y = \lambda_1 v_1$, and $|y| = \lambda_1$ (for $\lambda_1 > 0$). The closer $\lambda_2$ is to $\lambda_1$, the slower the method. For example, $A = \begin{pmatrix} 2 & 1 \\ 1 & 2 \end{pmatrix}$ has the eigenvalues 3 and 1, and $v_1 = (1, 1) / \sqrt 2 = (0.7071, 0.7071)$. From $x = (1, 0)$:
```text
iteration 1: lambda = 2.2361   x = 0.8944 0.4472
iteration 2: lambda = 2.8636   x = 0.7809 0.6247
iteration 3: lambda = 2.9837   x = 0.7328 0.6805
iteration 6: lambda = 3.0000   x = 0.7081 0.7061
```
Each iteration needs only a matrix-vector product and a norm, and this is what we parallelise.

In [`mpi/02-power/assignment.cpp`](mpi/02-power/assignment.cpp) rank 0 has a dense $2000 \times 2000$ matrix, stored by rows (the entry $(i, j)$ is `A[i * n + j]`), with $A_{ij} = \min(i, j) + 1$. For this matrix the eigenvalues are known: the largest is $\lambda_1 = 1 / (4 \sin^2(\theta / 2))$, with $\theta = \pi / (2n + 1)$, so the program can check the result, and the second is about 9 times smaller, so the method needs few iterations. Each process gets a block of rows of $A$, and keeps the same block of $x$:
```text
              A                  x          y = A x
rank 0 | rows    0 ...  499 |   | |       | block 0 |   each process: its rows of A
rank 1 | rows  500 ...  999 | * |x|   =   | block 1 |   times ALL of x,
rank 2 | rows 1000 ... 1499 |   | |       | block 2 |   but it keeps only its block
rank 3 | rows 1500 ... 1999 |   | |       | block 3 |   of x and of y
```
Only rank 0 knows `n`. The program already computes `rows` and `first_row`, as `counts` and `displacements` in section 1, but they need the right `n` on every process:
```bash
cd ../02-power
mpic++ -Wall -std=c++20 -O3 assignment.cpp -o assignment
mpirun -n 4 ./assignment
```

**Your tasks**:
1. TODO 1 and TODO 2: send `n` to all the processes with `MPI_Bcast`, then each process receives its rows of $A$ with `MPI_Scatterv`. You need two pairs of counts and displacements: `rows` and `first_row`, for $x$ and $y$, and a new pair for $A$. Why?
2. TODO 3 and TODO 4: at each iteration, all of $x$ on all the processes with `MPI_Allgatherv`, then $|y|$ with `MPI_Allreduce`. Run with `-n 4` and `-n 3`. How many iterations does $\lambda$ need to stop changing? Why is `MPI_Allgatherv` inside the loop, and not once before it?
3. TODO 5: rank 0 collects $x$ with `MPI_Gatherv`, to compare it with the exact eigenvector.
4. **Now break it**: replace `MPI_Allgatherv` with `MPI_Gatherv`, with root 0. Which $\lambda$ does the program find? Is there an error message? Why this number?

✅ **Checkpoint.** With `-n 4`, after task 3:
```
iteration  1: lambda = 1461506.52878005
iteration  2: lambda = 1620791.23184641
iteration  3: lambda = 1621936.04213703
iteration  4: lambda = 1621949.5248055
iteration  5: lambda = 1621949.69033342
iteration  6: lambda = 1621949.69237554
iteration  7: lambda = 1621949.69240075
iteration  8: lambda = 1621949.69240106
iteration  9: lambda = 1621949.69240106
iteration 10: lambda = 1621949.69240106
exact:        lambda = 1621949.69240106
4 processes, n = 2000: error on lambda = 2.9e-16, error on x = 3e-12
```
With `-n 3` and `-n 1` the same $\lambda$; the error on $\lambda$ can change in the last digit. Solution: [`solution.cpp`](mpi/02-power/solution.cpp).

<details>
<summary>Answers</summary>

- 1: `MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);` before `rows` is computed, as in section 1. Without it, on ranks 1, 2 and 3 `n` is 0, and the program blocks without printing anything. `MPI_Scatterv` counts numbers, not rows. A row of $A$ is `n` numbers: process `r` receives `rows[r] * n` numbers of $A$, from the position `first_row[r] * n`. The blocks of $x$ and $y$ are counted in rows.
  ```cpp
  std::vector<int> entries(size);
  std::vector<int> first_entry(size);
  for (int r = 0; r < size; ++r) {
    entries[r] = rows[r] * n;
    first_entry[r] = first_row[r] * n;
  }
  MPI_Scatterv(A.data(), entries.data(), first_entry.data(), MPI_DOUBLE, local_A.data(),
               local_rows * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  ```
  With `rows` and `first_row` in place of `entries` and `first_entry`, each process receives 500 numbers instead of $500 \cdot 2000$, the rest of `local_A` stays 0, and the program prints `lambda = 1`, without errors.
- 2:
  ```cpp
  MPI_Allgatherv(local_x.data(), local_rows, MPI_DOUBLE, x.data(), rows.data(), first_row.data(),
                 MPI_DOUBLE, MPI_COMM_WORLD);
  ...
  MPI_Allreduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  ```
  $\lambda$ stops changing after 8 iterations: the distance from the exact value is divided by about 81 at each iteration, because the second eigenvalue of this $A$ is about 9 times smaller than the first. `MPI_Allgatherv` must be in the loop because $x$ changes at each iteration, and each process changes only its block. With `MPI_Allgatherv` once before the loop, all the processes keep the first $x$, and the program prints `lambda = 1461506.52878005` ten times.
- 3: `MPI_Gatherv(local_x.data(), local_rows, MPI_DOUBLE, x.data(), rows.data(), first_row.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);`. On rank 0, `x` already has `n` elements: the same vector of the loop.
- 4: `lambda = 101524.01066418`, about 16 times too small, and no error. Ranks 1, 2 and 3 never receive $x$: their `x` stays 0, so their rows of $y$ are 0, and so is their block of the new $x$. Only rank 0 computes something: its rows of $A$ times its own block of $x$, the $500 \times 500$ block at the top left of $A$. The program finds the largest eigenvalue of that block. **A wrong communication does not always crash: here it converges, to the wrong number.**

</details>

---

## 3. From one process to another: a ring of messages

So far all the processes took part in each communication. `MPI_Send` and `MPI_Recv` send one message from one process to another (slides *Point-to-point communication* to *Deadlock*):

| Code | What it does |
|---|---|
| `MPI_Send(v.data(), m, MPI_INT, to, 0, MPI_COMM_WORLD);` | sends `m` numbers of `v` to the rank `to`, with the tag 0 |
| `MPI_Recv(w.data(), m, MPI_INT, from, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);` | receives `m` numbers from the rank `from`, with the tag 0, into `w` |
| `MPI_Sendrecv(v.data(), m, MPI_INT, to, 0, w.data(), m, MPI_INT, from, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);` | both, at the same time |

A message is received only if the tag and the communicator of `MPI_Send` and `MPI_Recv` match. In [`mpi/03-ring/assignment.cpp`](mpi/03-ring/assignment.cpp) each process sends `m` copies of its rank to the next one, and receives from the previous one; the last process sends to rank 0. `m` is the argument of the program:
```text
rank 0 --> rank 1 --> rank 2 --> rank 3
  ^                                |
  +--------------------------------+
```
```bash
cd ../03-ring
mpic++ -Wall -std=c++20 assignment.cpp -o assignment
mpirun -n 4 ./assignment 1
```

**Your tasks**:
1. TODO 1: `MPI_Send` to `next`, then `MPI_Recv` from `previous`. Run with `m = 1`: what does each rank receive?
2. **Now break it**: run with `m = 100000`. What happens? Find the smallest `m` for which it happens. (Stop the program with `ctrl+C`.)
3. TODO 2: replace the two calls with one `MPI_Sendrecv` (slide *`MPI_Sendrecv()`*). Does it work with `m = 100000`?

✅ **Checkpoint.** After task 3, with `m = 1` and `m = 100000` (in another order):
```
rank 0: received 3 from rank 3
rank 1: received 0 from rank 0
rank 2: received 1 from rank 1
rank 3: received 2 from rank 2
```
Solution: [`solution.cpp`](mpi/03-ring/solution.cpp).

<details>
<summary>Answers</summary>

- 1: `MPI_Send(message.data(), m, MPI_INT, next, 0, MPI_COMM_WORLD);` then `MPI_Recv(received.data(), m, MPI_INT, previous, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);`. Each rank receives the rank of the previous one, as in the checkpoint.
- 2: the program never ends, without an error: a **deadlock**. `MPI_Send` returns when `message` can be used again (slide *Blocking communication*). A short message is copied into a buffer of Open MPI, and `MPI_Send` returns at once. A long one is sent only when the other process calls `MPI_Recv`. All the processes wait in `MPI_Send`, and nobody reaches its `MPI_Recv`. In the container, 1000 `int` (4000 bytes) work, and 1024 `int` (4096 bytes) block. The limit depends on the MPI library and on the network: **a program that works only because messages are short is wrong**.
- 3: `MPI_Sendrecv(message.data(), m, MPI_INT, next, 0, received.data(), m, MPI_INT, previous, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);`: MPI sends and receives together, and the ring cannot block. It works also with $m = 10^7$. Another way: even ranks send first, odd ranks receive first. A third way is in the homework.

</details>

---

## Homework

Each exercise has a difficulty label: **[Easy]**, **[Intermediate]** or **[Hard]**.
- **Sections 1–3**, if you did not finish them in class.
- **[Easy]** — **`MPI_Allgatherv` or `MPI_Gatherv` + `MPI_Bcast`?** In `mpi/02-power/solution.cpp`, replace `MPI_Allgatherv` with `MPI_Gatherv` to rank 0, followed by `MPI_Bcast` of `x` from rank 0. Is $\lambda$ the same? Which version sends more data through rank 0?
- **[Intermediate]** — **Vectors of unknown length**: the [exercise below](#exercise-vectors-of-unknown-length).
- **[Intermediate]** — **A ring with non-blocking messages.** `MPI_Isend` and `MPI_Irecv` start a communication and return at once; `MPI_Waitall` waits until all of them are complete (slides *Non-blocking point-to-point communication* to *An example*). In `mpi/03-ring/solution.cpp`, replace `MPI_Sendrecv` with `MPI_Irecv` from `previous`, `MPI_Isend` to `next`, and `MPI_Waitall` on the two requests. Does it work with `m = 100000`? Why can it not block, as `MPI_Send` + `MPI_Recv` did? Which buffer must you not touch before `MPI_Waitall`?

<details>
<summary>Answers (homework)</summary>

- `MPI_Allgatherv` or `MPI_Gatherv` + `MPI_Bcast`:
  ```cpp
  MPI_Gatherv(local_x.data(), local_rows, MPI_DOUBLE, x.data(), rows.data(), first_row.data(),
              MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(x.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  ```
  `x` has `n` elements on all the processes, so `MPI_Bcast` can write into it. The same $\lambda$, and the same errors, with `-n 4` and `-n 3`. With `MPI_Gatherv` + `MPI_Bcast` all the blocks go to rank 0, and then all of `x` goes from rank 0 to each process. With `MPI_Allgatherv`, MPI can exchange the blocks directly between the processes, and rank 0 is not a bottleneck. One call, and MPI chooses how to do it.
- Non-blocking ring:
  ```cpp
  MPI_Request requests[2];
  MPI_Irecv(received.data(), m, MPI_INT, previous, 0, MPI_COMM_WORLD, &requests[0]);
  MPI_Isend(message.data(), m, MPI_INT, next, 0, MPI_COMM_WORLD, &requests[1]);
  MPI_Waitall(2, requests, MPI_STATUSES_IGNORE);
  ```
  It works with $m = 1$, $10^5$ and $10^7$. `MPI_Irecv` and `MPI_Isend` do not wait: each process reaches `MPI_Waitall` with both the receive and the send started, and MPI can complete them in any order. Before `MPI_Waitall`, do not read `received` (it may not be there yet), and do not change `message` (it may not be sent yet). The example [`MPI/Circular`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/Parallel/MPI/Circular) of the lecture does the same with `MPI_Isend`.

</details>

### Exercise: vectors of unknown length

In section 1 each process could compute `counts`, because all of them knew `n`. Often a process knows how many numbers it has only after computing them: the slide *Got it?* cites the example [`MPI/VectorSplit`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/Parallel/MPI/VectorSplit), where vectors built on each process are sent to the root and put one after the other. In [`mpi/04-primes/assignment.cpp`](mpi/04-primes/assignment.cpp) each process finds the prime numbers in its block of $0, \dots, N - 1$, with $N = 10^6$, and rank 0 must collect all of them, in order. Nobody knows in advance how many primes each process finds:
```bash
cd mpi/04-primes
mpic++ -Wall -std=c++20 -O3 assignment.cpp -o assignment
mpirun -n 4 ./assignment
```

**Your tasks**:
1. TODO 1: rank 0 receives the number of primes of each process, `local_count`, into `counts`, with `MPI_Gather`.
2. TODO 2 and TODO 3: on rank 0, the displacements and the size of `primes`; then `MPI_Gatherv`. Run with `-n 4`, `-n 3` and `-n 1`.
3. Why are the primes in order, without sorting them?

✅ **Checkpoint.** With `-n 4` (the first 4 lines in another order):
```
rank 0: 22044 primes in 0 ... 249999
rank 1: 19494 primes in 250000 ... 499999
rank 2: 18700 primes in 500000 ... 749999
rank 3: 18260 primes in 750000 ... 999999
78498 primes below 1000000: 2 3 5 7 11 ... 999961 999979 999983
```
With `-n 3` and `-n 1`, the same last line. Solution: [`solution.cpp`](mpi/04-primes/solution.cpp).

<details>
<summary>Answers</summary>

- 1:
  ```cpp
  std::vector<int> counts;
  if (rank == 0)
    counts.resize(size);
  MPI_Gather(&local_count, 1, MPI_INT, counts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
  ```
  A first collective only for the sizes, then the data: it is the usual way when the root does not know the sizes.
- 2:
  ```cpp
  if (rank == 0) {
    displacements.resize(size, 0);
    for (int r = 1; r < size; ++r)
      displacements[r] = displacements[r - 1] + counts[r - 1];
    primes.resize(displacements[size - 1] + counts[size - 1]);
  }
  MPI_Gatherv(local_primes.data(), local_count, MPI_INT, primes.data(), counts.data(),
              displacements.data(), MPI_INT, 0, MPI_COMM_WORLD);
  ```
  On the other processes `counts`, `displacements` and `primes` stay empty: MPI reads them only on the root.
- 3: the blocks are in order (rank 0 has the smallest numbers), each process finds its primes in order, and `MPI_Gatherv` puts the block of rank `r` at `displacements[r]`, after the blocks of the ranks before it.

</details>

**Further reading**: in [`AMSC-CodeExamples/Examples/src/Parallel/MPI`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/Parallel/MPI): [`VectorSplit`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/Parallel/MPI/VectorSplit) (`MPI_Gatherv`), [`MatrixVectorProduct`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/Parallel/MPI/MatrixVectorProduct), [`Circular`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/Parallel/MPI/Circular) and [`NonBlocking`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/Parallel/MPI/NonBlocking) (the ring and the deadlock), [`Probe`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/Parallel/MPI/Probe) (receiving a message of unknown length, without a first `MPI_Gather`). [RookieHPC](https://rookiehpc.github.io/) has a page for each MPI function.
