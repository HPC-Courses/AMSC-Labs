#include <iostream>
#include <array>
#include <random>
#include <string>
#include <iomanip>
#include <numeric> 
#include <algorithm> 

constexpr size_t dim = 2;
using Point = std::array<double, dim>;
using KDTree = std::vector<std::pair<size_t, size_t>>;

size_t build_kdtree_recursive(const std::vector<Point> &points,
                            std::vector<size_t>::iterator pts_begin,
                            std::vector<size_t>::iterator pts_end,
                            KDTree &kdtree,
                            size_t depth) {
  if(pts_end - pts_begin == 0) {
    return -1;
  }
  if(pts_end - pts_begin == 1) {
    kdtree[*pts_begin] = {-1, -1};
    return *pts_begin;
  }
  const auto axis = depth % dim;
  std::sort(pts_begin, pts_end, 
  [&](size_t a, size_t b) {return points[a][axis] < points[b][axis];});
  const auto median_idx = (pts_end - pts_begin) / 2;
  const auto idx = *(pts_begin + median_idx);

  kdtree[idx].first = build_kdtree_recursive(points, pts_begin, pts_begin + median_idx, kdtree, depth + 1);
  kdtree[idx].second = build_kdtree_recursive(points, pts_begin + median_idx + 1, pts_end, kdtree, depth + 1);

  return idx;
}

KDTree build_kdtree(const std::vector<Point> &points) {
  KDTree kdtree(points.size());
  std::vector<size_t> points_idx(points.size());
  std::iota(points_idx.begin(), points_idx.end(), 0);
  build_kdtree_recursive(points, points_idx.begin(), points_idx.end(), kdtree, 0);
  return kdtree;
}

int main(int argc, char *argv[]) {
  const std::vector<Point> points = {{7, 2}, {5, 4}, {9, 6}, {4, 7}, {8, 1}, {2, 3}};
  const auto kdtree = build_kdtree(points);
  for(const auto &pp : kdtree)
    std::cout << pp.first << " " << pp.second << std::endl;

  return 0;
}