import numpy as np
import trimesh
import trimesh.exchange.gltf

# Configuration
grid_size = 10
spacing = 0.3

# Function to create a sphere mesh with a material
def create_sphere_with_material(x, y, base_color, vertex_color):
    sphere = trimesh.creation.uv_sphere(radius=0.1)
    sphere.apply_translation([x * spacing, y * spacing, 0])
    _ = sphere.vertex_normals  # Ensure normals are computed

    material = {
        'name': f'mat_x{x:.2f}_y{y:.2f}',
        'baseColorFactor': base_color
    }

    # Generate per-vertex color (RGBA)
    num_vertices = len(sphere.vertices)
    colors = np.zeros((num_vertices, 4), dtype=np.uint8)
    colors[:, 0] = vertex_color[0] * 255  # Red
    colors[:, 1] = vertex_color[1] * 255  # Green
    colors[:, 2] = vertex_color[2] * 255  # Blue
    colors[:, 3] = vertex_color[3] * 255  # Alpha

    # Assign vertex colors
    sphere.visual.vertex_colors = colors

    sphere.visual.material = trimesh.visual.material.PBRMaterial(**material)

    return sphere, material

# Build scene
geometry = {}
materials = []
nodes = []
index = 0
for y in range(grid_size):
    weight_y = y / (grid_size - 1)
    vertex_color = [0.0, weight_y, 1.0, 1.0]
    for x in range(grid_size):
        weight_x = x / (grid_size - 1)
        base_color = [0.0, 1.0, weight_x, 1.0]
        mesh, material = create_sphere_with_material(x, y, base_color, vertex_color)
        name = f"sphere_{x}_{y}"
        geometry[name] = mesh
        materials.append(material)
        nodes.append({
            "name": name,
            "mesh": name
        })
# Assemble into a scene
scene = trimesh.Scene(geometry)
# Export to GLB
glb_bytes = trimesh.exchange.gltf.export_glb(scene, include_normals=True)
# Save to file
with open(f"vertex_color_grid.glb", "wb") as f:
    f.write(glb_bytes)
print(f"Saved vertex_color_grid.glb")
