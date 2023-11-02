import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from mpl_toolkits.mplot3d.art3d import Poly3DCollection


df = pd.read_csv("sphere.csv")
triangles =  []
for i, row in df.iterrows():
  triangles.append(np.array(row)[:9].reshape((3, 3)))

fig = plt.figure()
ax = fig.add_subplot(projection = '3d')

ax.add_collection(Poly3DCollection(triangles, alpha=0.5, linewidths=1, edgecolors='k'))

ax.set_xlim([-1, 1])
ax.set_ylim([-1, 1])
ax.set_zlim([-1, 1])

plt.show()
