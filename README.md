# C++ Power Method SVD for Python

This project implements a high-performance singular value decomposition (SVD) calculation using the power method. The core algorithm is written in modern C++ and parallelized to leverage multiple CPU cores. It is exposed to Python using pybind11 for seamless integration with NumPy.

This approach combines the raw computational speed of C++ with the flexibility and ease-of-use of Python.

## Features
- **High-Performance C++ Core**: The power method is implemented in C++17 for maximum speed.
- **Multithreaded**: Matrix and vector operations are parallelized using std::thread to utilize available hardware concurrency.
- **Zero-Copy**: Data is passed from NumPy to C++ without any copying, eliminating a major performance bottleneck.
- **Simple Python API**: The C++ function is callable from Python as if it were a native Python function.

### Install dependencies:

```bash
pip install numpy pybind11
```

### Build the C++ module:

```bash
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++
python setup.py build_ext --inplace
```

### Usage
Run the main Python script to see a demonstration and performance comparison against NumPy's built-in SVD.

```bash
python main.py
```

The script will generate a large random matrix, compute its dominant singular value and vector using both the custom C++ module and NumPy, and print the results and execution times.