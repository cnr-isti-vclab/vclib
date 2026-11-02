#include "../benchmark.h"

#include <vclib/space/complex/mesh_inertia.h>

#include <cmath>
#include <utility>

namespace {

using Inertia = vcl::MeshInertia<bench::Mesh>;

template<typename Func>
void runMassProperty(bench::State& state, Func&& func)
{
    const Inertia inertia(state.mesh());
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

BENCH("space/mesh_inertia/construction", [](bench::State& state) {
    const bench::Mesh& mesh = state.mesh();
    state.run([&]() { return Inertia(mesh); });
});

BENCH("space/mesh_inertia/volume", [](bench::State& state) {
    const Inertia inertia(state.mesh());
    state.run([&]() {
        bench::doNotOptimize(inertia);
        return inertia.volume();
    });
});

BENCH("space/mesh_inertia/center_of_mass", [](bench::State& state) {
    runMassProperty(
        state, [](const Inertia& inertia) { return inertia.centerOfMass(); });
});

BENCH("space/mesh_inertia/inertia_tensor", [](bench::State& state) {
    runMassProperty(state, [](const Inertia& inertia) {
        return inertia.inertiaTensor<Eigen::Matrix3d>();
    });
});

BENCH("space/mesh_inertia/inertia_tensor_eigen", [](bench::State& state) {
    runMassProperty(state, [](const Inertia& inertia) {
        Eigen::Matrix3d eigenvectors;
        vcl::Point3d    eigenvalues;
        inertia.inertiaTensorEigen(eigenvectors, eigenvalues);
        return std::pair {eigenvectors, eigenvalues};
    });
});
