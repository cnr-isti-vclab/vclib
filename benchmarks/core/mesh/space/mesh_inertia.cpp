#include "../benchmark.h"

#include <vclib/space/complex/mesh_inertia.h>

#include <cmath>
#include <utility>

namespace {

template<typename Mesh, typename Func>
void runMassProperty(bench::State<Mesh>& state, Func&& func)
{
    const vcl::MeshInertia<Mesh> inertia(state.mesh());
    if (!std::isfinite(inertia.volume()) || inertia.volume() == 0) {
        state.skip("the mesh has zero or non-finite signed volume");
        return;
    }
    state.run([&]() {
        bench::doNotOptimize(inertia);
        return func(inertia);
    });
}

} // namespace

BENCH(
    "space/mesh_inertia/construction",
    []<typename Mesh>(bench::State<Mesh>& state) {
        const Mesh& mesh = state.mesh();
        state.run([&]() { return vcl::MeshInertia<Mesh>(mesh); });
    });

BENCH(
    "space/mesh_inertia/volume",
    []<typename Mesh>(bench::State<Mesh>& state) {
        const vcl::MeshInertia<Mesh> inertia(state.mesh());
        state.run([&]() {
            bench::doNotOptimize(inertia);
            return inertia.volume();
        });
    });

BENCH(
    "space/mesh_inertia/center_of_mass",
    []<typename Mesh>(bench::State<Mesh>& state) {
        runMassProperty(
            state, [](const auto& inertia) { return inertia.centerOfMass(); });
    });

BENCH(
    "space/mesh_inertia/inertia_tensor",
    []<typename Mesh>(bench::State<Mesh>& state) {
        runMassProperty(state, [](const auto& inertia) {
            return inertia.template inertiaTensor<Eigen::Matrix3d>();
        });
    });

BENCH(
    "space/mesh_inertia/inertia_tensor_eigen",
    []<typename Mesh>(bench::State<Mesh>& state) {
        runMassProperty(state, [](const vcl::MeshInertia<Mesh>& inertia) {
            Eigen::Matrix3d                         eigenvectors;
            typename Mesh::VertexType::PositionType eigenvalues;
            inertia.inertiaTensorEigen(eigenvectors, eigenvalues);
            return std::pair {eigenvectors, eigenvalues};
        });
    });
