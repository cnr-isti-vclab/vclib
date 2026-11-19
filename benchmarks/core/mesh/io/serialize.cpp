// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../benchmark.h"

#include <sstream>

BENCH(
    "io/binary_serialize/serialize",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& m = s.mesh();
        if (m.vertexCount() == 0) {
            return s.skip("mesh has no vertices");
        }

        std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);

        s.run(
            [&]() {
                ss.str("");
                ss.clear();
                ss.seekp(0);
            },
            [&]() {
                m.serialize(ss);
                return ss.tellp();
            });
    });

BENCH(
    "io/binary_serialize/deserialize",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& m = s.mesh();
        if (m.vertexCount() == 0) {
            return s.skip("mesh has no vertices");
        }

        std::string buffer;
        {
            std::stringstream ss(
                std::ios::in | std::ios::out | std::ios::binary);
            m.serialize(ss);
            buffer = ss.str();
        }

        std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
        Mesh              dst;

        s.run(
            [&]() {
                ss.str(buffer);
                ss.clear();
                ss.seekg(0);
                dst.clear();
            },
            [&]() {
                dst.deserialize(ss);
                return dst.vertexCount();
            });
    });
