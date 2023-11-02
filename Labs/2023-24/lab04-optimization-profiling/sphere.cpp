#include <array>
#include <cmath>
#include <iostream>
#include <numbers>
#include <vector>

struct Triangle {
  std::array<std::array<double, 3>, 3> pts;
};

// Function to create a vector of triangles that mesh the surface of a sphere
std::vector<Triangle> createSphereMesh(int resolution) {
  std::vector<Triangle> meshTriangles;
  constexpr double pi = std::numbers::pi;
  for (int i = 0; i < resolution; i++) {
    for (int j = 0; j < resolution; j++) {
      double theta1 = (i * 2 * pi) / resolution;
      double theta2 = ((i + 1) * 2 * pi) / resolution;
      double phi1 = (j * pi) / resolution;
      double phi2 = ((j + 1) * pi) / resolution;

      std::array<std::array<double, 3>, 3> point1 = {
          std::array<double, 3>{std::cos(theta1) * std::sin(phi1),
                                std::sin(theta1) * std::sin(phi1),
                                std::cos(phi1)},
          std::array<double, 3>{std::cos(theta2) * std::sin(phi1),
                                std::sin(theta2) * std::sin(phi1),
                                std::cos(phi1)},
          std::array<double, 3>{std::cos(theta1) * std::sin(phi2),
                                std::sin(theta1) * std::sin(phi2),
                                std::cos(phi2)}};

      std::array<std::array<double, 3>, 3> point2 = {
          std::array<double, 3>{std::cos(theta2) * std::sin(phi1),
                                std::sin(theta2) * std::sin(phi1),
                                std::cos(phi1)},
          std::array<double, 3>{std::cos(theta2) * std::sin(phi2),
                                std::sin(theta2) * std::sin(phi2),
                                std::cos(phi2)},
          std::array<double, 3>{std::cos(theta1) * std::sin(phi2),
                                std::sin(theta1) * std::sin(phi2),
                                std::cos(phi2)}};

      Triangle triangle1 = {point1};
      Triangle triangle2 = {point2};

      meshTriangles.push_back(triangle1);
      meshTriangles.push_back(triangle2);
    }
  }

  return meshTriangles;
}

int main() {
  int resolution = 10;  // Example resolution
  std::vector<Triangle> meshTriangles = createSphereMesh(resolution);

  // Printing out the meshed triangles
  for (const auto& triangle : meshTriangles) {
    for (const auto& point : triangle.pts) {
      std::cout << point[0] << "," << point[1] << "," << point[2] << ",";
    }
    std::cout << std::endl;
  }

  return 0;
}