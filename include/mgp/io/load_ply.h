/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_IO_LOAD_PLY_H
#define MGP_IO_LOAD_PLY_H

#include <mgp/exception/io_exception.h>
#include "ply/ply.h"
#include "ply/ply_vertex.h"
#include "ply/ply_face.h"

// todo: remove this and make template
#include <mgp/mesh/mesh.h>

namespace mgp::io {

template <typename MeshType>
MeshType loadPly(const std::string& filename, bool enableOptionalComponents = true);

template <typename MeshType>
MeshType loadPly(const std::string& filename, FileMeshInfo& loadedInfo, bool enableOptionalComponents = true);

template <typename MeshType>
void loadPly(MeshType& m, const std::string& filename, bool enableOptionalComponents = true);

template <typename MeshType>
void loadPly(MeshType& m, const std::string& filename, FileMeshInfo& loadedInfo, bool enableOptionalComponents = true);

}

#include "load_ply.cpp"

#endif // MGP_IO_LOAD_PLY_H
