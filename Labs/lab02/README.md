# Exercise 1 - Copy elision
Under some circustances, thank to [copy elision](https://en.cppreference.com/w/cpp/language/copy_elision), the compiler can optimize away a copy in 
- a return statement 
- during the initialization of an object

This is important because, among other things, it allows to write more readable code. Indeed, suppose you have a `HugeDynamicObject` that requires memory allocation on the heap. Suppose you want to write a function `parametrically_build_object` that creates `HugeDynamicObject` depending on some some parameters `p1, p2`. Without copy elision a funtion with the signature
```cpp
HugeDynamicObject parametrically_build_object(double p1, double p2) {
    //... some code
    return HugeDynamicObject{/**/};
}
HugeDynamicObject hdo = parametrically_build_object(1.0, 2.5);
```
would be very wastefull since it requires to copy the result of the function into `h`. It would be better something like:
```cpp
void parametrically_change_object(double p1, double p2, HugeDynamicObject &hdo) {
    //... modify hdo as you wish
}
HugeDynamicObject hdo;
parametrically_change_object(1.0, 2.5, hdo);
```
which is less readable, since you cannot immediatly understand which are the inputs and which are the outputs.

## Assignements
1. Starting from the `hint` folder, implement a class `Noisy` that has a contructor, a copy constructor and a destructor which just print to the standard output that they have been called.
2. Compile `main.cpp` and try to understand where copy elision is applied.

# Exercise 2 - Polymorphism


# Exercise 3 - Newton's method
Newton's method is a numerical algorithm used to find roots of a function iteratively. If the function satisfies sufficient assumptions and the initial guess is close, then

$$x_{n+1}=x_{n}-{\frac {f(x_{n})}{f'(x_{n})}}$$

## Assignements
Write a `NewtonSolver` class that takes as constructor arguments 
- the function $f(x)$
- its derivative $f'(x)$
- the maximum number of iterations `max_iter`
- the tolerance on the residual `rtol`
- the tolerance on the step size `stol`

Has a method `solve` that takes as input the starting point $x_0$, and has the following members (accessible with a getter):
- `iter` number of iterations
- `res` residual
- `xs` the history of the iterations

Apply it to the function $f(x) = x^2 - 2$. Pass it as:
- a function pointer
- a lambda

