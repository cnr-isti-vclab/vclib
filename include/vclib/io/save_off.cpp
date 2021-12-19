/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include "internal/io_utils.h"

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
	std::ofstream fp = internal::saveFileStream(filename, "off");
	if (info.hasVertexNormals())
		fp << "N";
	if (info.hasVertexColors())
		fp << "C";
	if (info.hasVertexTexCoords())
		fp << "ST";
	fp << "OFF\n";

	uint vn = 0;
	uint fn = 0;
	uint en = 0;
	if constexpr (vcl::hasVertices<MeshType>()) {
		vn = m.vertexNumber();
	}
	if constexpr (vcl::hasFaces<MeshType>()) {
		fn = m.faceNumber();
	}
	if constexpr (vcl::hasEdges<MeshType>()) {
		en = m.edgeNumber();
	}
	fp << vn << " " << fn << " " << en << "\n";

	// vertices
	if constexpr (vcl::hasVertices<MeshType>()) {
		using VertexType = typename MeshType::VertexType;
		for (const VertexType& v : m.vertices()) {
			fp << v.coord().x() << " " << v.coord().y() << " " << v.coord().z() << " ";

			if constexpr(vcl::hasPerVertexColor<MeshType>()) {
				if (info.hasVertexColors() && vcl::isPerVertexColorEnabled(m)) {
					fp << v.color().red() << " " << v.color().green() << " " << v.color().blue()
					   << " " << v.color().alpha() << " ";
				}
			}
			if constexpr(vcl::hasPerVertexNormal<MeshType>()) {
				if (info.hasVertexNormals() && vcl::isPerVertexNormalEnabled(m)) {
					fp << v.normal().x() << " " << v.normal().y() << " " << v.normal().z() << " ";
				}
			}
			if constexpr(vcl::hasPerVertexTexCoord<MeshType>()) {
				if (info.hasVertexTexCoords() && vcl::isPerVertexTexCoordEnabled(m)) {
					fp << v.texCoord().u() << " " << v.texCoord().v() << " ";
				}
			}

			fp << "\n";
		}
	}

	// faces
	if constexpr (vcl::hasFaces<MeshType>()) {
		using VertexType = typename MeshType::VertexType;
		using FaceType = typename MeshType::FaceType;

		// indices of vertices that do not consider deleted vertices
		std::vector<int> vIndices = m.vertexCompactIndices();

		for (const FaceType& f : m.faces()) {
			fp << f.vertexNumber() << " ";
			for (const VertexType* v : f.vertices()) {
				fp << vIndices[m.index(v)] << " ";
			}
			if constexpr(vcl::hasPerFaceColor<MeshType>()) {
				if (info.hasFaceColors() && vcl::isPerFaceColorEnabled(m)) {
					fp << f.color().red() << " " << f.color().green() << " " << f.color().blue()
					   << " ";
				}
			}

			fp << "\n";
		}
	}
	fp.close();
}

} // namespace vcl::io
