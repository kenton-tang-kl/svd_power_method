#include "power_method.h"
#include <vector>
#include <cmath>
#include <random>
#include <numeric>
#include <thread>
#include <functional>
#include <iostream>

void multiply_ata_vec_threaded(const Matrix& ata, const Vector& v, Vector& result, size_t num_cols) {
  unsigned int num_threads = std::min(static_cast<unsigned int>(num_cols), std::thread::hardware_concurrency());
  if (num_threads == 0) {
    num_threads = 1;
  }
  std::vector<std::thread> threads;
  threads.reserve(num_threads);

  size_t rows_per_thread=num_cols / num_threads;
  size_t start_row=0;

  auto multiply_chunk=[&](size_t start, size_t end) {
    for (size_t i=start; i<end; ++i) {
      double sum=0.0;
      for (size_t j=0; j<num_cols; ++j) {
        sum += ata[i * num_cols + j] * v[j];
      }
      result[i]=sum;
    }
  };

  for (unsigned int i=0; i<num_threads - 1; ++i) {
    size_t end_row=start_row + rows_per_thread;
    threads.emplace_back(multiply_chunk, start_row, end_row);
    start_row=end_row;
  }
  threads.emplace_back(multiply_chunk, start_row, num_cols);

  for (auto& t : threads) {
    t.join();
  }
}

double vector_norm_threaded(const Vector& v) {
  unsigned int num_threads=std::thread::hardware_concurrency();
  std::vector<std::thread> threads;
  threads.reserve(num_threads);
  std::vector<double> partial_sums(num_threads, 0.0);

  size_t chunk_size=v.size() / num_threads;
  size_t start_idx=0;
  
  auto sum_squares_chunk=[&](size_t start, size_t end, int thread_idx) {
    double local_sum=0.0;
    for (size_t i=start; i<end; ++i) {
      local_sum += v[i] * v[i];
    }
    partial_sums[thread_idx]=local_sum;
  };

  for (unsigned int i=0; i<num_threads - 1; ++i) {
    size_t end_idx=start_idx + chunk_size;
    threads.emplace_back(sum_squares_chunk, start_idx, end_idx, i);
    start_idx=end_idx;
  }
  threads.emplace_back(sum_squares_chunk, start_idx, v.size(), num_threads - 1);
  
  double total_sum_sq=0.0;
  for (unsigned int i=0; i<num_threads; ++i) {
    threads[i].join();
    total_sum_sq += partial_sums[i];
  }

  return std::sqrt(total_sum_sq);
}

std::pair<double, Vector> calculate_dominant_svd(const double* a_ptr, size_t rows, size_t cols, int max_iterations, double tolerance) {
  Matrix ata(cols * cols);
  for (size_t i=0; i<cols; ++i) {
    for (size_t j=0; j<cols; ++j) {
      double sum=0.0;
      for (size_t k=0; k<rows; ++k) {
        sum += a_ptr[k * cols + i] * a_ptr[k * cols + j];
      }
      ata[i * cols + j]=sum;
    }
  }
  
  Vector v(cols);
  std::mt19937 rng(std::random_device{}());
  std::uniform_real_distribution<double> dist(-1.0, 1.0);
  for (size_t i=0; i<cols; ++i) {
    v[i]=dist(rng);
  }
  double norm_v=vector_norm_threaded(v);
  for (size_t i=0; i<cols; ++i) {
    v[i] /= norm_v;
  }

  Vector w(cols);
  Vector v_old=v;

  for (int i=0; i<max_iterations; ++i) {
    multiply_ata_vec_threaded(ata, v, w, cols);
    double norm_w=vector_norm_threaded(w);
    for (size_t j=0; j<cols; ++j) {
      v[j]=w[j] / norm_w;
    }

    double dot_product=0.0;
    for(size_t k=0; k<cols; ++k) {
        dot_product += v[k] * v_old[k];
    }

    if (std::abs(std::abs(dot_product) - 1.0) < tolerance) {
      break;
    }
    v_old=v;
  }
  
  multiply_ata_vec_threaded(ata, v, w, cols);
  double eigenvalue=0.0;
  for(size_t i=0; i<cols; ++i) {
      eigenvalue += v[i] * w[i];
  }

  double singular_value=std::sqrt(eigenvalue);
  
  return {singular_value, v};
}
