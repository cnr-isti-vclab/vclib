// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../benchmark.h"

#include <vclib/io.h>

#include <filesystem>

namespace {

struct TempFileRemover
{
    std::filesystem::path path;

    ~TempFileRemover()
    {
        std::error_code ec;
        std::filesystem::remove(path, ec);
    }
};

} // namespace

BENCH("io/save_mesh/save_ply_binary", []<typename Mesh>(bench::State<Mesh>& s) {
    if (s.mesh().vertexCount() == 0) {
        return s.skip("the mesh has no vertices");
    }

    const auto tmpPath =
        std::filesystem::temp_directory_path() / "vclib_bench_save_binary.ply";
    TempFileRemover   remover{tmpPath};
    const std::string pathStr = tmpPath.string();

    vcl::SaveSettings settings;
    settings.binary = true;

    s.run([&]() { vcl::saveMesh(s.mesh(), pathStr, settings); });
});

BENCH("io/save_mesh/save_ply_ascii", []<typename Mesh>(bench::State<Mesh>& s) {
    if (s.mesh().vertexCount() == 0) {
        return s.skip("the mesh has no vertices");
    }

    const auto tmpPath =
        std::filesystem::temp_directory_path() / "vclib_bench_save_ascii.ply";
    TempFileRemover   remover{tmpPath};
    const std::string pathStr = tmpPath.string();

    vcl::SaveSettings settings;
    settings.binary = false;

    s.run([&]() { vcl::saveMesh(s.mesh(), pathStr, settings); });
});

BENCH("io/save_mesh/save_obj", []<typename Mesh>(bench::State<Mesh>& s) {
    if (s.mesh().vertexCount() == 0) {
        return s.skip("the mesh has no vertices");
    }

    const auto tmpPath =
        std::filesystem::temp_directory_path() / "vclib_bench_save.obj";
    TempFileRemover   remover{tmpPath};
    const std::string pathStr = tmpPath.string();

    s.run([&]() { vcl::saveMesh(s.mesh(), pathStr); });
});

BENCH("io/save_mesh/save_stl_binary", []<typename Mesh>(bench::State<Mesh>& s) {
    if constexpr (!vcl::FaceMeshConcept<Mesh>) {
        return s.skip("the mesh has no faces");
    }
    else {
        if (s.mesh().vertexCount() == 0) {
            return s.skip("the mesh has no vertices");
        }
        if (s.mesh().faceCount() == 0) {
            return s.skip("the mesh has no faces");
        }

        const auto tmpPath =
            std::filesystem::temp_directory_path() / "vclib_bench_save_binary.stl";
        TempFileRemover   remover{tmpPath};
        const std::string pathStr = tmpPath.string();

        vcl::SaveSettings settings;
        settings.binary = true;

        s.run([&]() { vcl::saveMesh(s.mesh(), pathStr, settings); });
    }
});

BENCH("io/save_mesh/save_off", []<typename Mesh>(bench::State<Mesh>& s) {
    if (s.mesh().vertexCount() == 0) {
        return s.skip("the mesh has no vertices");
    }

    const auto tmpPath =
        std::filesystem::temp_directory_path() / "vclib_bench_save.off";
    TempFileRemover   remover{tmpPath};
    const std::string pathStr = tmpPath.string();

    s.run([&]() { vcl::saveMesh(s.mesh(), pathStr); });
});

#ifdef VCLIB_WITH_TINYGLTF
BENCH("io/save_mesh/save_glb", []<typename Mesh>(bench::State<Mesh>& s) {
    if constexpr (!vcl::FaceMeshConcept<Mesh>) {
        return s.skip("the mesh has no faces");
    }
    else {
        if (s.mesh().vertexCount() == 0) {
            return s.skip("the mesh has no vertices");
        }
        if (s.mesh().faceCount() == 0) {
            return s.skip("the mesh has no faces");
        }

        const auto tmpPath =
            std::filesystem::temp_directory_path() / "vclib_bench_save.glb";
        TempFileRemover   remover{tmpPath};
        const std::string pathStr = tmpPath.string();

        vcl::SaveSettings settings;
        settings.binary = true;

        s.run([&]() { vcl::saveMesh(s.mesh(), pathStr, settings); });
    }
});
#endif
