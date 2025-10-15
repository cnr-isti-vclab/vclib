import numpy as np
import trimesh
import trimesh.exchange.gltf

# Configuration
grid_size = 10
spacing = 0.3
base_color = [0.3401939868927002, 0.6781029105186462, 1.0, 1.0]  # RGBA
alpha_modes = ['OPAQUE', 'MASK', 'BLEND']

# Function to create a sphere mesh with a material
def create_sphere_with_material(x, y, metallic, roughness, alpha_mode):
    sphere = trimesh.creation.uv_sphere(radius=0.1)
    sphere.apply_translation([x * spacing, y * spacing, 0])
    _ = sphere.vertex_normals  # Ensure normals are computed
    base_color[3] = roughness  # Set alpha in base color growning with roughness

    material = {
        'name': f'mat_m{metallic:.2f}_r{roughness:.2f}',
        'baseColorFactor': base_color,
        'metallicFactor': metallic,
        'roughnessFactor': roughness,
        'alphaMode': alpha_mode,
        'alphaCutoff': 0.3
    }

    # Generate per-vertex color (RGBA)
    num_vertices = len(sphere.vertices)
    colors = np.zeros((num_vertices, 4), dtype=np.uint8)
    colors[:, 0] = (x % 2) * 255        # Red
    colors[:, 1] = 0                    # Green
    colors[:, 2] = (1 - (x % 2)) * 255  # Blue
    colors[:, 3] = np.random.randint(127, 255, num_vertices) # Random Alpha between 0.5 and 1.0

    # Assign vertex colors
    sphere.visual.vertex_colors = colors

    sphere.visual.material = trimesh.visual.material.PBRMaterial(**material)

    return sphere, material

for i in range(len(alpha_modes)):
    alpha_mode = alpha_modes[i]
    # Build scene
    geometry = {}
    materials = []
    nodes = []

    index = 0
    for y in range(grid_size):
        metallic = y / (grid_size - 1)
        for x in range(grid_size):
            roughness = x / (grid_size - 1)

            mesh, material = create_sphere_with_material(x, y, metallic, roughness, alpha_mode)
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
    with open(f"{alpha_mode.lower()}_grid.glb", "wb") as f:
        f.write(glb_bytes)

    print(f"Saved {alpha_mode.lower()}_grid.glb")
