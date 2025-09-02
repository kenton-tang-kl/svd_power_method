#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include "power_method.h"

namespace py=pybind11;

py::object calculate_dominant_svd_py(py::array_t<double, py::array::c_style | py::array::forcecast> a) {
  if (a.ndim() != 2) {
    throw std::runtime_error("Input should be a 2-D NumPy array");
  }

  py::buffer_info buf=a.request();
  const double* ptr=static_cast<double*>(buf.ptr);
  size_t rows=a.shape(0);
  size_t cols=a.shape(1);

  auto result_pair=calculate_dominant_svd(ptr, rows, cols);
  
  double singular_value=result_pair.first;
  Vector singular_vector=result_pair.second;

  py::array_t<double> py_singular_vector(singular_vector.size());
  py::buffer_info vec_buf=py_singular_vector.request();
  double* vec_ptr=static_cast<double*>(vec_buf.ptr);
  std::copy(singular_vector.begin(), singular_vector.end(), vec_ptr);
  
  return py::make_tuple(singular_value, py_singular_vector);
}

PYBIND11_MODULE(cpp_power_svd, m) {
  m.doc()="High-performance SVD power method";
  m.def("calculate_dominant_svd", &calculate_dominant_svd_py, "Calculate the dominant singular value and vector of a matrix");
}
