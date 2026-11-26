# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

# This example assumes that vclib is installed and available in the PYTHONPATH.

import vclib as vcl
import os
import sys

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from paths import VCLIB_EXAMPLE_MESHES_PATH, VCLIB_PYTHON_RESULTS_PATH

# This example demonstrates various point sampling algorithms available in
# VCLib:
# 1. All vertices sampling
# 2. Uniform face sampling
# 3. Montecarlo sampling
# 4. Poisson sampling


def mesh_point_sampling():
    print("=== VCLib Example 019: Mesh Point Sampling ===\n")

    # /****** Load a mesh ******/

    print("=== Loading Mesh ===")

    original_mesh = vcl.load_tri_mesh(f"{VCLIB_EXAMPLE_MESHES_PATH}/bunny.obj")
    original_mesh.set_name("Original Bunny Mesh")

    vcl.update_vertex_and_face_normals(original_mesh)

    print(
        f"Loaded mesh with {original_mesh.vertex_count()} vertices and "
        f"{original_mesh.face_count()} faces"
    )

    # /****** All Vertices Sampling ******/

    print("\n=== All Vertices Sampling ===")

    # Sample all vertices of the mesh
    all_vertices_sampler = vcl.all_vertices_point_sampling(original_mesh)
    all_vertices = all_vertices_sampler.to_point_cloud()
    all_vertices.set_name("All Vertices Sampling")

    print(f"Sampled {all_vertices.vertex_count()} vertices (all vertices)")

    # /****** Uniform Face Sampling ******/

    print("\n=== Uniform Face Sampling ===")

    # Sample 500 points uniformly from face centers
    num_samples = 500
    uniform_sampler = vcl.face_uniform_point_sampling(
        original_mesh, num_samples
    )
    uniform = uniform_sampler.to_point_cloud()
    uniform.set_name("Uniform Face Sampling")

    print(
        f"Sampled {uniform.vertex_count()} points using uniform face sampling"
    )

    # /****** Montecarlo Sampling ******/

    print("\n=== Montecarlo Sampling ===")

    # Sample 750 points using montecarlo method
    num_samples = 750
    montecarlo_sampler = vcl.montecarlo_point_sampling(
        original_mesh, num_samples
    )
    montecarlo = montecarlo_sampler.to_point_cloud()
    montecarlo.set_name("Montecarlo Sampling")

    print(
        f"Sampled {montecarlo.vertex_count()} points using montecarlo sampling"
    )

    # /****** Poisson Sampling ******/

    print("\n=== Poisson Sampling ===")

    # Sample using poisson distribution (approximate number of samples)
    num_samples = 1000
    poisson_sampler = vcl.montecarlo_poisson_point_sampling(
        original_mesh, num_samples
    )
    poisson = poisson_sampler.to_point_cloud()
    poisson.set_name("Poisson Sampling")

    print(
        f"Sampled {poisson.vertex_count()} points using poisson sampling "
        "(approximate)"
    )

    # /****** Sampling comparison ******/

    print("\n=== Sampling Summary ===")
    print(
        f"Original mesh: {original_mesh.vertex_count()} vertices, "
        f"{original_mesh.face_count()} faces"
    )
    print(f"All vertices:  {all_vertices.vertex_count()} points")
    print(f"Uniform:       {uniform.vertex_count()} points")
    print(f"Montecarlo:    {montecarlo.vertex_count()} points")
    print(f"Poisson:       {poisson.vertex_count()} points")

    return original_mesh, all_vertices, uniform, montecarlo, poisson


def main():
    try:
        original_mesh, all_vertices, uniform, montecarlo, poisson = (
            mesh_point_sampling()
        )

        # /****** Save the sampled meshes ******/

        print("\n=== Saving Sampled Meshes ===")

        os.makedirs(VCLIB_PYTHON_RESULTS_PATH, exist_ok=True)

        vcl.save_mesh(
            original_mesh, f"{VCLIB_PYTHON_RESULTS_PATH}/019_original_mesh.ply"
        )
        vcl.save_mesh(
            all_vertices,
            f"{VCLIB_PYTHON_RESULTS_PATH}/019_vertices_sampling.ply",
        )
        vcl.save_mesh(
            uniform, f"{VCLIB_PYTHON_RESULTS_PATH}/019_uniform_sampling.ply"
        )
        vcl.save_mesh(
            montecarlo,
            f"{VCLIB_PYTHON_RESULTS_PATH}/019_montecarlo_sampling.ply",
        )
        vcl.save_mesh(
            poisson, f"{VCLIB_PYTHON_RESULTS_PATH}/019_poisson_sampling.ply"
        )

        print("Saved all sampled meshes to results directory")

        print("Example completed successfully!")

    except Exception as e:
        print(f"Error in saving: {e}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    exit(main())
