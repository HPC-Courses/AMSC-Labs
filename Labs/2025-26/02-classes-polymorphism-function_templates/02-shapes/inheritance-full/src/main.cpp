/*
compile with
g++ -Wall -I ../include shapes.cpp main.cpp -o main_1.out

and with
g++ -c -Wall -I ../include shapes.cpp
g++ -c -Wall -I ../include main.cpp
g++ -Wall shapes.o main.o -o main_2.out
*/

#include <iostream>
#include <memory>
#include <vector>
#include "shapes.h"

using namespace std;
int main(int argc, char** argv)
{
    // The stack is usually restricted to a limited amount of memory per program/process, 
    // so allocating very large objects in local scope with atomatic storage is a bad idea, 
    // that could lead to stack overflow when the program runs out of stack memory.
    // This is why if the object to be allocated is large, it is better to allocate it on the heap memory, 
    // to do so, use shared_ptr or unique_ptr
    vector<shared_ptr<Shape> > shapes;
    shapes.push_back(make_shared<Circle>(2.0));
    shapes.push_back(make_shared<Rectangle>(3.1, 1.));

    // The problem when dealing with polymorphism is that the vector allocated above with shared_ptr does not store
    // the elements in a continuous memory location, this means that it is preferable to avoid to build a big vector
    // few elements are enough.
    // Polymorphism is useful in case we have, e.g., multiple quadrature methods that are all of course quadrature rules, so they share something in common.
    // In general, you will have few quadrature rule implemented in a program and can be implemented with the help of the isomorphism.
    // In general, always have a look at the performances. Data locality when accessing elements is important because CPU likes to access elements in that way.


    for (const auto& s : shapes)
    {
        cout << s->getArea() << " " << s->getName() << endl;
    }

    return 0;
}
