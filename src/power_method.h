#pragma once

#include <vector>
#include <utility>

using Matrix=std::vector<double>;
using Vector=std::vector<double>;

std::pair<double, Vector> calculate_dominant_svd(
  const double* a_ptr,
  size_t rows,
  size_t cols,
  int max_iterations=100,
  double tolerance=1e-9
);
