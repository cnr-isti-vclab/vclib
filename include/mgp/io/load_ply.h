#ifndef MGP_IO_LOAD_PLY_H
#define MGP_IO_LOAD_PLY_H

#include "ply/ply.h"
#include "ply/ply_vertex.h"
#include "ply/ply_face.h"

// todo: remove this and make template
#include <mgp/trimesh.h>

namespace mgp::io {

void loadPly(TriMesh& m, const std::string& filenamem, FileMeshInfo& loadedInfo);

}

#include "load_ply.cpp"

#endif // MGP_IO_LOAD_PLY_H
