import csv
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Read points from the CSV file
points = []
with open("landsample.txt", "r") as file:
    reader = csv.reader(file)
    for row in reader:
        x, y, z = map(float, row)
        points.append((x, y, z))

# Separate x, y, and z coordinates into separate lists
x_coords, y_coords, z_coords = zip(*points)

# Create a 3D plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot the surface
ax.plot_trisurf(x_coords, y_coords, z_coords, cmap='viridis')

# Set labels and title
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.set_title('3D Surface')

# Show the plot
plt.show()
