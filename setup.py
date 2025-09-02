import sys
from setuptools import setup, Extension
import pybind11

cpp_args = ['-std=c++17', '-O3']

if sys.platform == 'darwin':
  cpp_args.extend(['-stdlib=libc++', '-mmacosx-version-min=10.14'])

sfc_module = Extension(
  'cpp_power_svd',
  ['src/bindings.cc', 'src/power_method.cc'],
  include_dirs=[pybind11.get_include()],
  language='c++',
  extra_compile_args=cpp_args,
)

setup(
  name='cpp_power_svd',
  version='1.0',
  description='Python package with C++ power method for SVD',
  ext_modules=[sfc_module],
  zip_safe=False,
)
