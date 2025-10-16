/**
 * @file shapes.cpp
 * @brief Implementation of shape class constructors
 */
#include "shapes.h"

/**
 * @brief Construct a Circle with a given radius
 * @param radius Radius of the circle
 */
Circle::Circle(const double radius) : radius(radius)
{}

/**
 * @brief Construct a Rectangle with given height and base
 * @param height Height of the rectangle
 * @param basis Basis of the rectangle
 */
Rectangle::Rectangle(const double height, const double basis) : height(height), basis(basis)
{}
