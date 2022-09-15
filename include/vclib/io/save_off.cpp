/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#include "save_off.h"

#include <vclib/exception/io_exception.h>

#include "internal/io_utils.h"
#include "internal/io_write.h"

namespace vcl::io {

template<typename MeshType>
void saveOff(const MeshType& m, const std::string& filename)
{
	vcl::io::FileMeshInfo info(m);
	saveOff(m, filename, info);
}

template<typename MeshType>
void saveOff(const MeshType& m, const std::string& filename, const FileMeshInfo& info)
{
	FileMeshInfo meshInfo(m);

	// make sure that the given info contains only components that are actually available in the
	// mesh. meshInfo will contain the intersection between the components that the user wants to
	// save and the components that are available in the mesh.
	meshInfo = info.intersect(meshInfo);
	std::ofstream fp = internal::saveFileStream(filename, "off");
	if (meshInfo.hasVertexNormals())
		fp << "N";
	if (meshInfo.hasVertexColors())
		fp << "C";
	if (meshInfo.hasVertexTexCoords())
		fp << "ST";
	fp << "OFF" << std::endl;

	uint vn = 0;
	uint fn = 0;
	uint en = 0;
	if constexpr (vcl::HasVertices<MeshType>) {
		vn = m.vertexNumber();
	}
	if constexpr (vcl::HasFaces<MeshType>) {
		fn = m.faceNumber();
	}
	if constexpr (vcl::HasEdges<MeshType>) {
		en = m.edgeNumber();
	}

	io::internal::writeInt(fp, vn, false);
	io::internal::writeInt(fp, fn, false);
	io::internal::writeInt(fp, en, false);
	fp << std::endl;

	// vertices
	if constexpr (vcl::HasVertices<MeshType>) {
		using VertexType = typename MeshType::VertexType;
		for (const VertexType& v : m.vertices()) {
			io::internal::writeDouble(fp, v.coord().x(), false);
			io::internal::writeDouble(fp, v.coord().y(), false);
			io::internal::writeDouble(fp, v.coord().z(), false);

			if constexpr(vcl::HasPerVertexColor<MeshType>) {
				if (meshInfo.hasVertexColors()) {
					io::internal::writeInt(fp, v.color().red(), false);
					io::internal::writeInt(fp, v.color().green(), false);
					io::internal::writeInt(fp, v.color().blue(), false);
					io::internal::writeInt(fp, v.color().alpha(), false);
				}
			}
			if constexpr(vcl::HasPerVertexNormal<MeshType>) {
				if (meshInfo.hasVertexNormals()) {
					io::internal::writeDouble(fp, v.normal().x(), false);
					io::internal::writeDouble(fp, v.normal().y(), false);
					io::internal::writeDouble(fp, v.normal().z(), false);
				}
			}
			if constexpr(vcl::HasPerVertexTexCoord<MeshType>) {
				if (meshInfo.hasVertexTexCoords()) {
					io::internal::writeDouble(fp, v.texCoord().u(), false);
					io::internal::writeDouble(fp, v.texCoord().v(), false);
				}
			}

			fp << std::endl;
		}
	}

	// faces
	if constexpr (vcl::HasFaces<MeshType>) {
		using VertexType = typename MeshType::VertexType;
		using FaceType = typename MeshType::FaceType;

		// indices of vertices that do not consider deleted vertices
		std::vector<int> vIndices = m.vertexCompactIndices();

		for (const FaceType& f : m.faces()) {
			io::internal::writeInt(fp, f.vertexNumber(), false);
			for (const VertexType* v : f.vertices()) {
				io::internal::writeInt(fp, vIndices[m.index(v)], false);
			}
			if constexpr(vcl::HasPerFaceColor<MeshType>) {
				if (meshInfo.hasFaceColors()) {
					io::internal::writeInt(fp, f.color().red(), false);
					io::internal::writeInt(fp, f.color().green(), false);
					io::internal::writeInt(fp, f.color().blue(), false);
					io::internal::writeInt(fp, f.color().alpha(), false);
				}
			}

			fp << std::endl;
		}
	}
	fp.close();
}

} // namespace vcl::io
