/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_POISSON_RECON_POISSON_RECONSTRUCTION_H
#define VCL_POISSON_RECON_POISSON_RECONSTRUCTION_H

#include <vclib/mesh.h>

#include <poisson_recon/PreProcessor.h>
#include <poisson_recon/Reconstructors.h>
#include <poisson_recon/Extrapolator.h>

namespace vcl::poiss {

namespace detail {

} // namespace detail

template<FaceMeshConcept OutMesh, MeshConcept MeshType>
OutMesh poissonReconstruction(
    const MeshType& mesh,
    unsigned        depth = 8,
    unsigned        width = 0,
    unsigned        scale = 1,
    bool            linearFit = false)
{
    OutMesh outMesh;

    // TODO

    return outMesh;
}

} // namespace vcl::poiss

#endif // VCL_POISSON_RECON_POISSON_RECONSTRUCTION_H
