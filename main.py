import numpy as np
import cpp_power_svd
import time

def main():
  rows=2000
  cols=1000
  
  print(f"Generating a {rows}x{cols} matrix")
  A=np.random.rand(rows, cols).astype(np.float64)

  print("Running C++ Power Method SVD")
  start_time=time.time()
  s_val, s_vec=cpp_power_svd.calculate_dominant_svd(A)
  end_time=time.time()
  cpp_duration=end_time - start_time
  
  print(f"\n--- C++ Implementation ---")
  print(f"Execution Time: {cpp_duration:.6f} seconds")
  print(f"Dominant Singular Value: {s_val}")
  print(f"Dominant Singular Vector (first 5 elements): {s_vec[:5]}")

  print("\nRunning NumPy SVD for validation")
  start_time=time.time()
  U, s, Vt=np.linalg.svd(A, full_matrices=False)
  end_time=time.time()
  numpy_duration=end_time - start_time
  
  numpy_s_vec=Vt[0, :]
  
  if np.sign(s_vec[0]) != np.sign(numpy_s_vec[0]):
    numpy_s_vec *= -1
  
  print(f"\n--- NumPy linalg.svd ---")
  print(f"Execution Time: {numpy_duration:.6f} seconds")
  print(f"Dominant Singular Value: {s[0]}")
  print(f"Dominant Singular Vector (first 5 elements): {numpy_s_vec[:5]}")
  
  print("\n--- Comparison ---")
  print(f"C++ implementation was {numpy_duration/cpp_duration:.2f}x faster.")
  print(f"Singular Value Difference: {abs(s_val - s[0]):.6e}")
  print(f"Singular Vector Cosine Similarity: {np.dot(s_vec, numpy_s_vec) / (np.linalg.norm(s_vec) * np.linalg.norm(numpy_s_vec)):.6f}")

if __name__=="__main__":
  main()
