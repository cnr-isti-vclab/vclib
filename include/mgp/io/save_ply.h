/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_IO_SAVE_PLY_H
#define MGP_IO_SAVE_PLY_H

#include <mgp/exception/io_exception.h>

#include "ply/ply.h"
#include "ply/ply_vertex.h"
#include "ply/ply_face.h"

// todo: remove this and make template
#include <mgp/mesh/mesh.h>

namespace mgp::io {

template <typename MeshType>
void savePly(const MeshType& m, const std::string& filename);

template <typename MeshType>
void savePly(const MeshType& m, const std::string& filename, FileMeshInfo& loadedInfo);

}

#include "save_ply.cpp"

#endif // MGP_IO_SAVE_PLY_H
