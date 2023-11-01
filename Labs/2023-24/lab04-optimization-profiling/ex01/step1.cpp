#include <array>
#include <chrono>
#include <iostream>
#include <memory>
#include <numbers>
#include <random>
#include <vector>

class Shape {
public:
  Shape() = default;
  virtual std::array<double, 3> getNormal() const = 0;
  constexpr virtual const char *getName() = 0;
  virtual ~Shape() = default;
};

class Triangle : public Shape {
public:
  Triangle(const std::array<std::array<double, 3>, 3> &pts)
      : Shape(), m_pts(pts){};
  virtual std::array<double, 3> getNormal() const override;
  constexpr virtual const char *getName() override { return "Triangle"; };
  virtual ~Triangle() override = default;

private:
  std::array<unsigned char, 3> m_rgb;
  std::array<std::array<double, 3>, 3> m_pts;
  unsigned char m_alpha;
};

std::array<double, 3> Triangle::getNormal() const {
  // Compute vectors AB and AC
  std::array<double, 3> AB, AC, normal;
  for (int i = 0; i < 3; ++i) {
    AB[i] = m_pts[1][i] - m_pts[0][i];
    AC[i] = m_pts[2][i] - m_pts[0][i];
  }

  // Compute the cross product
  normal[0] = AB[1] * AC[2] - AB[2] * AC[1];
  normal[1] = AB[2] * AC[0] - AB[0] * AC[2];
  normal[2] = AB[0] * AC[1] - AB[1] * AC[0];

  return normal;
};

std::array<std::array<double, 3>, 3>
get_random_triangle(std::uniform_real_distribution<double> &dist,
                    std::mt19937 &gen) {
  std::array<std::array<double, 3>, 3> points;

  // Initialize points with random values
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      points[i][j] = dist(gen);
    }
  }
  return points;
}

int main(int argc, char **argv) {
  const auto N = argc > 1 ? std::atoll(argv[1]) : 1000000ull;
  std::vector<std::shared_ptr<Shape>> shapes;

  using namespace std::chrono;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dist(-10.0, 10.0);

  {
    const auto t0 = high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) {
      shapes.push_back(
          std::make_shared<Triangle>(get_random_triangle(dist, gen)));
    }
    const auto t1 = high_resolution_clock::now();
    const auto dt = duration_cast<milliseconds>(t1 - t0).count();
    std::cout << "Elapsed for initialization: " << dt << " [ms]" << std::endl;
  }

  std::vector<std::array<double, 3>> normals;
  {
    const auto t0 = high_resolution_clock::now();

    normals.reserve(shapes.size());
    for (size_t i = 0; i < N; ++i) {
      normals.push_back(shapes[i]->getNormal());
    }
    const auto t1 = high_resolution_clock::now();
    const auto dt = duration_cast<milliseconds>(t1 - t0).count();
    std::cout << "Elapsed for computation of norm: " << dt << " [ms]"
              << std::endl;
  }

  return 0;
}