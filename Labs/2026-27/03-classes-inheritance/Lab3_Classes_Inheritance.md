# Laboratory 03 — Classes and inheritance

### 16/10/2026

This lab puts into practice the lecture on *Classes*: classes that protect their data, the special member functions, inheritance and dynamic polymorphism. Keep the slides at hand: as in [Lab 2](../02-smart_pointers-callables/Lab2_SmartPointers_Callables.md), the theory is only recalled in a few lines at the start of each section.

We start from where Lab 2 stopped: the `Dataset` comes back, and this time it becomes a proper class.

Each exercise has an `assignment`, to be completed, and a `solution`. The ✅ **Checkpoint** at the end of each section shows the output you should get. Whatever you do not finish in class is listed in the [Homework](#homework).

## Outline

- [0. Setup](#0-setup)
- **Part 1 — Classes**
  - [1. Exercise: a class with an invariant](#1-exercise-a-class-with-an-invariant)
  - [2. Predict it: the special member functions](#2-predict-it-the-special-member-functions)
- **Part 2 — Inheritance and polymorphism**
  - [3. Exercise: a hierarchy of shapes](#3-exercise-a-hierarchy-of-shapes)
- [Homework](#homework)

---

## 0. Setup

Update the repository and move to the directory of this lab, inside the container:
```bash
cd ~/shared-folder/AMSC-Labs
git pull
cd Labs/2026-27/03-classes-inheritance
ls classes inheritance
```
As recommended in Lab 2, the `Makefile`s of the exercises compile with AddressSanitizer (`-g -fsanitize=address`).

---

# Part 1 — Classes

## 1. Exercise: a class with an invariant

The `Dataset` of Lab 2 is an **aggregate**: its data are public and it has no constructor, so any code can put it in any state, even a meaningless one, such as a dataset with no values. Now we want a type that guarantees an **invariant**: a property that is true for every object, from its construction to its destruction. For `Dataset` it is *"there is at least one value"*, so that `mean()` never divides by zero. To guarantee it:
- the data are `private`: nobody outside the class can touch them;
- the **constructor** establishes the invariant. If it cannot, it throws an exception, and the object is never created;
- the `public` methods are the only way to read or change the object, and each of them preserves the invariant. The methods that do not change the object are `const`, and only they can be called on a `const` object.

```cpp
class Dataset {
public:
  Dataset(const std::string &name, const std::vector<double> &values)
      : name_{name}, values_{values} { // initialisation list
    if (values_.empty())
      throw std::invalid_argument("...");                     // invariant not satisfied
  }
  double mean() const;    // const: does not change the object
  void scale(double f);   // non-const: changes the object
private:
  std::string name_;
  std::vector<double> values_;
};
```

**`struct` or `class`?** The two keywords differ only in the default access: `public` for `struct`, `private` for `class` (slide 2). A `struct` can have private members, constructors and methods too: what makes the invariant possible is private data plus a constructor that checks them, not the keyword. By convention, `struct` is used for plain data without an invariant, whose members can take any value independently (like `NewtonOptions` in Lab 2), and `class` when there is an invariant to protect.

In [`classes/01-dataset/assignment`](classes/01-dataset/assignment) you find the `Dataset` of Lab 2 as we left it. Run it first:
```bash
cd classes/01-dataset/assignment
make run
```
The last line prints `mean nan`: `main` emptied the values of a dataset, and nothing prevented it.

**Your tasks**:
1. Turn `Dataset` into a class with the invariant *"`values_` is never empty"*:
   - private data members `name_` and `values_`;
   - the constructor above, which throws `std::invalid_argument` (from `<stdexcept>`) if `values` is empty;
   - `const` methods `name()`, `size()` and `mean()`;
   - a non-const method `scale(factor)`, which multiplies all the values by `factor`. Why can it not break the invariant?
2. In Lab 2 `read_dataset` returned a `unique_ptr`, which was the owner of the `Dataset`, and `nullptr` for an invalid line. Neither is needed: a `Dataset` is a small object, and an invalid line can now throw. Make `read_dataset` return a `Dataset` **by value**, and throw `std::invalid_argument` instead of returning `nullptr`.
3. In `main`:
   - store the datasets in a `std::vector<Dataset>`, and skip the invalid lines with `try`/`catch`:
     ```cpp
     try {
       all.push_back(read_dataset(iss));
     } catch (const std::invalid_argument &e) {
       std::cerr << "Skipping invalid line: " << e.what() << std::endl;
     }
     ```
   - write `print(const Dataset &)`;
   - `strain` is in mm/m: convert it to m/m with `scale(1e-3)`, and print it again.
4. Try to break it: add one of these to `main` at a time, and check what happens, at compile time or at run time.
   - the line that emptied the values, written for the new class:
     ```cpp
     all[0].values_.clear(); // equivalent to all.front().values_.clear();
     ```
   - a `const` dataset, then a call to `scale`:
     ```cpp
     Dataset const c{"c", {1., 2.}};
     c.scale(2.);
     ```
   - a dataset with no values:
     ```cpp
     Dataset e{"e", {}};
     ```

**`throw`, `try` and `catch`.** `throw` means *"I cannot go on"*: the function stops immediately, and a constructor that throws does not create the object. What you throw describes the problem: `std::invalid_argument("...")` means *"you gave me a value that is not valid"*, followed by a message. The problem travels back to the caller, then to its caller, and so on, until a `try { ... } catch (const std::invalid_argument &e) { ... }` stops it and decides what to do (`e.what()` returns the message). If nobody catches it, the program terminates. Why not a `return`? A `return` in the constructor would still create the object, empty, and a special return value like `nullptr` is easy to forget to check, while an exception cannot be ignored by mistake.

✅ **Checkpoint.**
```
Skipping invalid line: dataset 'empty' has no values
Skipping invalid line: dataset 'velocity' contains a value that is not a number
datasets:
  pressure, 4 values, mean 1.275
  temperature, 3 values, mean 20.4333
  density, 4 values, mean 999.75
  strain, 2 values, mean 1.5
strain in m/m:
  strain, 2 values, mean 0.0015
```
Solution: [`classes/01-dataset/solution`](classes/01-dataset/solution).

<details>
<summary>Answers</summary>

1. `scale` changes the values, but not their number: a non-empty dataset stays non-empty. A public `values` could be emptied by anybody, a method like `clear()` would break the invariant, so it does not exist.
2. The `struct` of Lab 2 already owned its values: the `vector` does, like the `v` of Lab 2, section 1. The `unique_ptr` owned only the small object `Dataset` (a string and a vector), and its `nullptr` meant "no dataset". Now an invalid line throws, so there is no need for an empty value, and the function returns the `Dataset` itself. Returning it by value is cheap: it is *moved* into the vector, i.e. only the pointer to the block of values is copied.
4. The first two do not compile: `values_` is private, and `scale()` is not `const`, so it cannot be called on a `const` object. The third compiles, but the constructor throws, and since nobody catches the exception the program terminates. The object `e` never exists, so nobody can ever use an empty dataset.

</details>

Note that we wrote no destructor, no copy constructor, no assignment operator: the ones generated by the compiler copy, move and destroy `name_` and `values_`, and this is exactly what we want. Section 2 explains why.

---

## 2. Predict it: the special member functions

For every class `T` the compiler can generate six special member functions (slides 14–17):

| Function | Called when |
|---|---|
| `T()` | default construction: `T a;` |
| `T(const T &)` | copy construction: `T b = a;`, pass by value, ... |
| `T(T &&) noexcept` | move construction: `T c = std::move(a);`, ... |
| `T &operator=(const T &)` | copy assignment: `b = a;` |
| `T &operator=(T &&) noexcept` | move assignment: `b = std::move(a);` |
| `~T()` | destruction: end of scope, `delete`, ... |

[`classes/02-special_members/special_members.cpp`](classes/02-special_members/special_members.cpp) defines a class `Noisy` in which all six print their name.

1. **Before running it**, read `main`. Cases 1–4 follow directly from the table; for cases 5, 6a and 7, write down which functions you expect to be called.
2. Compile, run and compare:
   ```bash
   cd ../../02-special_members
   g++ -Wall -std=c++20 special_members.cpp -o special_members
   ./special_members
   ```
3. Answer:
   - case 5: `make_noisy` builds a `Noisy` and returns it by value. Why are neither the copy nor the move constructor called?
   - case 6a: why a copy, and when is it destroyed?
   - case 7: at the second `push_back` the vector is full and reallocates its buffer (as `v` in Lab 2, section 1). What happens to `v0`?
4. Remove `noexcept` from the move constructor, and run again. What changes in case 7?

✅ **Checkpoint.** With `noexcept`, the second `push_back` prints
```
    constructor(v1)
    move constructor(v1)
    move constructor(v0)
    destructor()
    destructor()
```
and without `noexcept`
```
    constructor(v1)
    move constructor(v1)
    copy constructor(v0)
    destructor(v0)
    destructor()
```

<details>
<summary>Answers</summary>

- Case 5: *copy elision*. Since C++17, an object returned by value from a temporary is built directly in its final place, `d`: no copy and no move.
- Case 6a: the parameter `n` is a new object, copy-constructed from `c`, and destroyed when `by_value` returns. It prints `a` because `c` was moved from `a` in case 4. Case 6b prints nothing: a reference is not a new object.
- Case 7: the vector allocates a larger buffer, builds `v1` there, moves `v0` into it, and destroys the old `v0`. The two `destructor()` are the old `v0` and the temporary `Noisy{"v1"}`: both were *moved from*, so their name is now empty.
- Without `noexcept` the vector **copies** `v0`. If moving could throw, an exception in the middle of the reallocation would leave some elements moved and some not. With a copy the old buffer is untouched, so the vector stays as it was if something goes wrong. This is why move operations should be `noexcept` (slide 26).

</details>

### When the generated functions are wrong: the shallow copy

When do you have to write the special member functions yourself? Almost only when the class manages a resource directly, e.g. memory through an owning raw pointer: then the generated copy copies the **address**, not the data.

[`classes/03-rule_of_zero/buggy_vector.cpp`](classes/03-rule_of_zero/buggy_vector.cpp) contains a small `Vector` that allocates its data with `new[]` and frees them in the destructor. Compile it with AddressSanitizer and run it:
```bash
cd ../03-rule_of_zero
g++ -Wall -std=c++20 -g -fsanitize=address buggy_vector.cpp -o buggy_vector
./buggy_vector
```
Changing `b[0]` also changes `a[0]`, and AddressSanitizer reports a `heap-use-after-free`. The generated copy constructor copied `data_`, i.e. the address: `a`, `b` and the parameter `v` of `sum` share the same block. When `sum` returns, `v` is destroyed and frees it (look at the line *freed by* of the report).

The fix is not to write a better copy constructor, but to need none: [`vector.cpp`](classes/03-rule_of_zero/vector.cpp) stores the data in a `std::vector<double>`, and has no destructor at all.

> **Rule of zero**: if the members of your class (containers, smart pointers, strings, ...) already manage their resources, do not write any of the special member functions. This is what we did with `Dataset`.
>
> **Rule of five**: if you have to write one of destructor, copy constructor, copy assignment, move constructor, move assignment, you probably have to write all five.

Writing the five functions for `buggy_vector.cpp` is in the [Homework](#homework).

---

# Part 2 — Inheritance and polymorphism

## 3. Exercise: a hierarchy of shapes

We want to handle circles, rectangles and squares with the same code: one `print` and one loop that work for every shape, including those we will add later. Each shape computes its area in its own way, but all of them answer the same questions: area, perimeter, name.
- **The base class is a contract.** `Shape` declares *what* every shape can do, not *how*. `virtual double area() const = 0;` has no implementation (*pure virtual*), so `Shape` is *abstract* and cannot be instantiated: a shape that does not say which shape it is has no area.
- **A derived class keeps the contract.** `class Circle : public Shape` means *a `Circle` is a `Shape`*: public inheritance expresses an **is-a** relationship, so a `Circle` can be used wherever a `Shape` is expected. With `override`, `Circle` writes its own version of `area()`.
- **`virtual`: the version is chosen at run time.** `print(const Shape &s)` only knows that `s` is a shape. When it calls `s.area()`, the version of the actual object runs: `Circle::area()` for a circle, `Rectangle::area()` for a rectangle. This is **dynamic polymorphism**. Without `virtual`, `Shape::area()` would always be called, since `s` is declared as a `Shape`.
- **Only through a reference or a pointer.** A copy into a `Shape` object would keep only its `Shape` part (*slicing*, see "Now break it" below), so `s` must refer to the original object.

The syntax you need:

| Syntax | Meaning |
|---|---|
| `class Circle : public Shape` | `Circle` derives from `Shape` |
| `virtual double area() const = 0;` | *pure virtual*: the derived classes must override it. `Shape` is *abstract*: it cannot be instantiated |
| `virtual std::string name() const;` | virtual with a default implementation: the derived classes *may* override it |
| `double area() const override;` | overrides a virtual function of the base; the compiler checks that it really does |
| `virtual ~Shape() = default;` | needed to delete a derived object through a pointer to the base |
| `class Circle final` | nobody can derive from `Circle` |
| `explicit Circle(double r)` | a one-argument constructor that is not an implicit conversion `double` → `Circle` |
| `Square(double l) : Rectangle{l, l} {}` | the constructor of the derived class calls the one of the base |

In [`inheritance/01-shapes/assignment`](inheritance/01-shapes/assignment):
- `shapes.hpp` contains the abstract class `Shape` and the class `Circle`, complete;
- `main.cpp` reads [`shapes.txt`](inheritance/01-shapes/assignment/shapes.txt) with the factory `make_shape`, which returns a `std::unique_ptr<Shape>`, and stores the shapes in a `std::vector<std::unique_ptr<Shape>>`. Then it prints name, area and perimeter of each shape with `print(const Shape &)`.

**Why a pointer again?** In section 1 `read_dataset` stopped returning a pointer: all the datasets have the same type, so a `std::vector<Dataset>` is enough. Here the shapes have *different* types, and we want them in the same vector. A `std::vector<Shape>` is impossible: `Shape` is abstract, and even if it were not, each element would be sliced to a `Shape`. The vector holds pointers to `Shape` instead: all the elements have the same type, and each one points to a different shape. Here the pointer is needed for polymorphism. (`make_shape` also returns `nullptr` for an unknown type, as `read_dataset` did in Lab 2.)

```bash
cd ../../inheritance/01-shapes/assignment
make run
```
For now only the circles are recognised.

**Your tasks**:
1. Following the example of `Circle`, write `Rectangle(base, height)`. Like the `Dataset` of section 1, a shape has an invariant: its sizes are positive. `Circle` checks it in its constructor: do the same for `Rectangle`, and throw `std::invalid_argument` if `base` or `height` is not positive.
2. Write `Square(side)`, derived from `Rectangle`. Which methods does it need to override? Does it need to check that `side` is positive? Can `Rectangle` be `final`?
3. Extend `make_shape` to `rectangle b h` and `square l`.
4. Uncomment the last lines of `main`.

Note that `print` and the loops in `main` do not change: they work for every shape, present and future. This is the point of dynamic polymorphism.

✅ **Checkpoint.** (The triangles are still unknown: they are in the [Homework](#homework).)
```
Skipping invalid shape: rectangle   2.0 -1.0 (the sides must be positive)
Skipping invalid shape: square      -1.0 (the sides must be positive)
Skipping unknown shape: triangle    3.0 4.0 6.0
Skipping unknown shape: triangle    1.0 1.0 5.0
Skipping unknown shape: hexagon     1.0
  Circle: area 3.14159, perimeter 6.28319
  Rectangle: area 6, perimeter 10
  Square: area 2.25, perimeter 6
  Circle: area 0.785398, perimeter 3.14159
Total area: 12.177
Largest: Rectangle
copy: Rectangle, reference: Square
```
Solution: [`inheritance/01-shapes/solution`](inheritance/01-shapes/solution).

**Now break it**, one change at a time:
1. Give `Circle` a destructor that prints its name, `~Circle() { std::cout << "~Circle" << std::endl; }` (include `<iostream>` in `shapes.hpp`), and run: `~Circle` is printed once per circle. Now remove `virtual` from the destructor of `Shape`, and run again: `~Circle` is no longer printed. Which destructor does the `unique_ptr<Shape>` call? What would happen if `Circle` owned a resource, e.g. a `std::vector`? Read also the compiler warning.
2. Look at the last lines of `main`: a `Square` copied into a `Rectangle` object prints `Rectangle`, because only its `Rectangle` part was copied (**slicing**). Through a reference, instead, it is still a `Square`. This is why polymorphic objects are passed by reference or pointer, never by value.

<details>
<summary>Answers</summary>

- Task 2: `Square` only needs its constructor and `name()`: area and perimeter of a `Rectangle` are already right. It does not need to check `side` either: its constructor calls the one of `Rectangle`, which throws if `side` is not positive, so `square -1.0` is rejected too. `Rectangle` cannot be `final`, since `Square` derives from it.
- Break it 1: the `unique_ptr<Shape>` deletes a `Shape *`, so without `virtual` only the destructor of `Shape` is called, and the one of `Circle` never runs. Here nothing visible is lost, since a `Circle` only holds a `double`; but if a derived class owned a resource (a `std::vector`, an open file, memory allocated with `new`), it would never be released: a **leak**. Formally, deleting a derived object through a pointer to the base without a virtual destructor is undefined behaviour, and with some compilers AddressSanitizer also stops the program. The rule: a class with virtual functions must have a virtual destructor.

</details>

### Static and dynamic polymorphism

"One function, many types" is not new. In Lab 2, `midpoint_template` accepted any callable as a template parameter, and the version with `std::function` accepted any callable too. Both are polymorphism: with the template the compiler chooses the function to call (*static polymorphism*), with `std::function` the choice is made at run time, as with the virtual functions of this section (*dynamic polymorphism*).

| | Static: templates | Dynamic: virtual functions |
|---|---|---|
| The function to call is chosen | at compile time, from the type | at run time, through the virtual table |
| Cost of a call | none: it can be inlined | an indirect call, which cannot be inlined ([Lab 2, section 6](../02-smart_pointers-callables/Lab2_SmartPointers_Callables.md#6-how-much-does-a-callable-cost)) |
| Different types in the same container | no: `RootFinder<F1>` and `RootFinder<F2>` are two different types | yes: `std::vector<std::unique_ptr<Shape>>` |
| Memory | the objects can be stored next to each other, e.g. in a `std::vector<Circle>` | each shape is allocated on its own on the heap: neighbours in the vector can be far apart in memory, and the CPU reads them more slowly |
| Code | a new version of the function is generated for each type | `print` is compiled once and works for every shape, even future ones |

**Take-away:** templates when the type is known at compile time and speed matters, as in the inner loops of Lab 2; virtual functions when the type is chosen at run time, e.g. shapes read from a file, or when objects of different types must live in the same container. The [root finders](#exercise-a-family-of-root-finders) of the Homework use both.

---

## Homework

What is left from the lab, plus some extra exercises:
- **Triangles.** Write `Triangle(a, b, c)`, given the three sides: the constructor throws `std::invalid_argument` if they violate the triangle inequality (the invariant of this class), and the area is given by Heron's formula, $\sqrt{s(s-a)(s-b)(s-c)}$ with $s$ the half perimeter. Extend `make_shape` to `triangle a b c`, and check that you get the output of the solution:
  ```
  Skipping invalid shape: triangle    1.0 1.0 5.0 (the sides do not form a triangle)
  ...
    Triangle: area 5.33268, perimeter 13
  ...
  Total area: 17.5097
  ```
- **Two more ways to break the shapes**, one change at a time:
  - in `Circle`, replace `std::string name() const override` with `std::string name()`, without `const` and without `override`. It compiles (read the warning), but circles are printed as `Shape`: why? Now add `override` back, and read the error;
  - remove `area()` from `Circle`: why does `std::make_unique<Circle>` no longer compile?
- **Rule of five.** Instead of using a `std::vector`, fix `buggy_vector.cpp` by writing the five special member functions (slides 22–25):
  - copy constructor and copy assignment allocate new memory and copy the data (*deep copy*);
  - move constructor and move assignment steal the pointer and leave the source empty;
  - the destructor frees the memory.

  Check it with AddressSanitizer, then compare the length of the two solutions.
- **A family of root finders**: the [exercise below](#exercise-a-family-of-root-finders).
- **Polymorphic copy.** A `std::vector<std::unique_ptr<Shape>>` cannot be copied. Add to `Shape` a pure virtual method `std::unique_ptr<Shape> clone() const`, implemented in each derived class as `return std::make_unique<Circle>(*this);` (slides 74–76), and use it to write a function that copies the vector.

### Exercise: a family of root finders

In Lab 2 you wrote Newton's method as a function template, and at home the secant and bisection methods. Now we want to use them through the same interface, e.g. to compare them in a loop:
```cpp
std::vector<std::unique_ptr<RootFinder>> solvers;
...
for (const auto &s : solvers)
  auto r = s->solve();
```
In [`inheritance/02-root_finders/assignment`](inheritance/02-root_finders/assignment), `root_finders.hpp` contains
- `RootFinderOptions` and `RootFinderResult`, the structs of Lab 2;
- the abstract base class `RootFinder`, complete: it stores the function `f_` and the options in `protected` members (accessible to the derived classes, not to the users), and declares the pure virtual `solve()` and `name()`;
- the class `Newton`, declared.

**Your tasks**:
1. Implement `Newton::solve()`, reusing the body of `newton()` of Lab 2. What changes, now that `f`, `df`, `x0` and the options are data members?
2. Write the class `Secant(f, x0, x1, options)`.
3. Write the class `Bisection(f, a, b, options)`: its constructor throws `std::invalid_argument` if $f(a)f(b) > 0$, the invariant of this class.
4. Uncomment the lines in `main` and compare the number of iterations.
5. In Lab 2, `newton` took the function as a template parameter; here `RootFinder` stores it as a `std::function`. Why? (Hint: if the base were a template, `RootFinder<F>`, could two solvers of two different lambdas be stored in the same vector?)

✅ **Checkpoint.**
```
Bisection on [2, 3] not created: f(a) and f(b) must have opposite signs
Newton     root = 1.4142135623731, iterations = 5
Secant     root = 1.4142135623731, iterations = 6
Bisection  root = 1.41421356237333, iterations = 39
```
Solution: [`inheritance/02-root_finders/solution`](inheritance/02-root_finders/solution).

**Further reading** in [`AMSC-CodeExamples/Examples/src`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src): [`Polygon`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/Polygon), [`MyMat0`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/MyMat0), [`MoveSemantic_simple`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/MoveSemantic_simple), [`NewtonSolver`](https://github.com/HPC-Courses/AMSC-CodeExamples/tree/AMSC/Examples/src/NewtonSolver).
