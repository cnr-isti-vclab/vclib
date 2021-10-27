#ifndef MGP_IO_LOAD_PLY_H
#define MGP_IO_LOAD_PLY_H

#include "ply/ply.h"
#include "ply/ply_vertex.h"
#include "ply/ply_face.h"

// todo: remove this and make template
#include <mgp/mesh/mesh.h>

namespace mgp::io {

template <typename MeshType>
void loadPly(MeshType& m, const std::string& filename);

template <typename MeshType>
void loadPly(MeshType& m, const std::string& filename, FileMeshInfo& loadedInfo);

}

#include "load_ply.cpp"

#endif // MGP_IO_LOAD_PLY_H
