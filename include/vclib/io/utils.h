/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_IO_UTILS_H
#define VCL_IO_UTILS_H

#include <algorithm>
#include <fstream>

#include <vclib/exceptions/io_exceptions.h>
#include <vclib/misc/file_info.h>
#include <vclib/misc/mesh_info.h>
#include <vclib/misc/string.h>
#include <vclib/misc/tokenizer.h>

namespace vcl::io::detail {

template<MeshConcept MeshType>
void addPerVertexCustomComponent(
    MeshType&                        m,
    const MeshInfo::CustomComponent& cc)
{
    switch (cc.type) {
    case MeshInfo::CHAR:
        m.template addPerVertexCustomComponent<char>(cc.name);
        break;
    case MeshInfo::UCHAR:
        m.template addPerVertexCustomComponent<unsigned char>(cc.name);
        break;
    case MeshInfo::SHORT:
        m.template addPerVertexCustomComponent<short>(cc.name);
        break;
    case MeshInfo::USHORT:
        m.template addPerVertexCustomComponent<unsigned short>(cc.name);
        break;
    case MeshInfo::INT:
        m.template addPerVertexCustomComponent<int>(cc.name);
        break;
    case MeshInfo::UINT:
        m.template addPerVertexCustomComponent<uint>(cc.name);
        break;
    case MeshInfo::FLOAT:
        m.template addPerVertexCustomComponent<float>(cc.name);
        break;
    case MeshInfo::DOUBLE:
        m.template addPerVertexCustomComponent<double>(cc.name);
        break;
    default: assert(0);
    }
}

template<FaceMeshConcept MeshType>
void addPerFaceCustomComponent(MeshType& m, const MeshInfo::CustomComponent& cc)
{
    switch (cc.type) {
    case MeshInfo::CHAR:
        m.template addPerFaceCustomComponent<char>(cc.name);
        break;
    case MeshInfo::UCHAR:
        m.template addPerFaceCustomComponent<unsigned char>(cc.name);
        break;
    case MeshInfo::SHORT:
        m.template addPerFaceCustomComponent<short>(cc.name);
        break;
    case MeshInfo::USHORT:
        m.template addPerFaceCustomComponent<unsigned short>(cc.name);
        break;
    case MeshInfo::INT:
        m.template addPerFaceCustomComponent<int>(cc.name);
        break;
    case MeshInfo::UINT:
        m.template addPerFaceCustomComponent<uint>(cc.name);
        break;
    case MeshInfo::FLOAT:
        m.template addPerFaceCustomComponent<float>(cc.name);
        break;
    case MeshInfo::DOUBLE:
        m.template addPerFaceCustomComponent<double>(cc.name);
        break;
    default: assert(0);
    }
}

/**
 * @brief enableOptionalComponents enables all the components that are in the
 * file mesh info and that may be enabled in the mesh. If these components are
 * not available in the mesh, the info file will be modified in order to tell
 * that a particular component cannot be saved into the mesh.
 *
 * This function is useful when you know what components are going to load from
 * a file, and you want that all the components that you are going to read in
 * the file will be saved in the mesh, if possible.
 *
 * @param info
 * @param m
 */
template<MeshConcept MeshType>
void enableOptionalComponents(MeshInfo& info, MeshType& m)
{
    if (info.hasVertices()) {
        if (info.hasVertexColors()) {
            if (!vcl::enableIfPerVertexColorOptional(m)) {
                info.setVertexColors(false);
            }
        }
        if (info.hasVertexNormals()) {
            if (!vcl::enableIfPerVertexNormalOptional(m)) {
                info.setVertexNormals(false);
            }
        }
        if (info.hasVertexQuality()) {
            if (!vcl::enableIfPerVertexQualityOptional(m)) {
                info.setVertexQuality(false);
            }
        }
        if (info.hasVertexTexCoords()) {
            if (!vcl::enableIfPerVertexTexCoordOptional(m)) {
                info.setVertexTexCoords(false);
            }
        }
        if (info.hasVertexCustomComponents()) {
            if constexpr (vcl::HasPerVertexCustomComponents<MeshType>) {
                for (const auto& cc : info.vertexCustomComponents()) {
                    addPerVertexCustomComponent(m, cc);
                }
            }
            else {
                info.clearVertexCustomComponents();
            }
        }
    }
    else {
        info.setVertices(false);
    }

    if (info.hasFaces()) {
        if (info.hasFaceColors()) {
            if (!vcl::enableIfPerFaceColorOptional(m)) {
                info.setFaceColors(false);
            }
        }
        if (info.hasFaceNormals()) {
            if (!vcl::enableIfPerFaceNormalOptional(m)) {
                info.setFaceNormals(false);
            }
        }
        if (info.hasFaceQuality()) {
            if (!vcl::enableIfPerFaceQualityOptional(m)) {
                info.setFaceQuality(false);
            }
        }
        if (info.hasFaceWedgeTexCoords()) {
            if (!vcl::enableIfPerFaceWedgeTexCoordsOptional(m)) {
                info.setFaceWedgeTexCoords(false);
            }
        }
        if (info.hasFaceCustomComponents()) {
            if constexpr (vcl::HasPerFaceCustomComponents<MeshType>) {
                for (const auto& cc : info.faceCustomComponents()) {
                    addPerFaceCustomComponent(m, cc);
                }
            }
            else {
                info.clearFaceCustomComponents();
            }
        }
    }
    else {
        info.setFaces(false);
    }
}

} // namespace vcl::io::detail

#endif // VCL_IO_UTILS_H
