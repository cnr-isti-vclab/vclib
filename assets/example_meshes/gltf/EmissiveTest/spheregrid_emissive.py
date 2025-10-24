import numpy as np
import trimesh
import trimesh.exchange.gltf

# Configuration
grid_size = 10
spacing = 0.3
# Function to create a sphere mesh emissive
def create_sphere_with_emissive(x, emissive):
    sphere = trimesh.creation.uv_sphere(radius=0.1)
    sphere.apply_translation([x * spacing, 0, 0])
    _ = sphere.vertex_normals  # Ensure normals are computed

    material = {
        'name': f'mat_e{emissive:.2f}',
        'baseColorFactor': [0.0, 0.0, 0.5, 1.0],
        'emissiveFactor': [emissive, emissive, emissive]
    }

    sphere.visual.material = trimesh.visual.material.PBRMaterial(**material)

    return sphere, material



# Build scene
geometry = {}
materials = []
nodes = []
index = 0
for x in range(grid_size):
    emissive = x / (grid_size - 1)
    mesh, material = create_sphere_with_emissive(x, emissive)
    name = f"sphere_{x}"
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
with open(f"emissive_grid.glb", "wb") as f:
    f.write(glb_bytes)
print(f"Saved emissive_grid.glb")
