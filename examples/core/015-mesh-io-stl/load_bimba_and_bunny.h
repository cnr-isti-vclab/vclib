#ifndef LOAD_BIMBA_AND_BUNNY_H
#define LOAD_BIMBA_AND_BUNNY_H

#include <vclib/algorithms/mesh/update.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

void loadBimbaAndBunnyMeshes(vcl::TriMesh& m1, vcl::TriMesh& m2)
{
    vcl::ConsoleLogger log;
    vcl::MeshInfo      loadedInfo;

    log.startTimer();
    m1 = vcl::load<vcl::TriMesh>(
        VCLIB_EXAMPLE_MESHES_PATH "/bimba_bin.stl", loadedInfo, log);

    log.startTimer();
    m2 = vcl::load<vcl::TriMesh>(
        VCLIB_EXAMPLE_MESHES_PATH "/bunny_simplified.stl", loadedInfo, log);
}

#endif // LOAD_BIMBA_AND_BUNNY_H
