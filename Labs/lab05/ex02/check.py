import numpy as np 

if __name__ == "__main__":
  N = 100
  A = -np.diag(np.ones(N-1), 1) - np.diag(np.ones(N-1), -1) + 2 * np.diag(np.ones(N))
  D, V = np.linalg.eig(A)
  idx = np.argmax(D)
  v = V[:, idx]
  print(D[idx], (v.T@A@v) / (v.T@v), v)