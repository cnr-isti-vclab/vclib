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

#include "save_obj.h"

#include <vclib/exception/io_exception.h>

#include "internal/io_utils.h"
#include "internal/io_write.h"

namespace vcl::io {

template<typename MeshType>
void saveObj(const MeshType& m, const std::string& filename)
{
	vcl::io::FileMeshInfo info(m);
	saveObj(m, filename, info);
}

template<typename MeshType>
void saveObj(const MeshType& m, const std::string& filename, const FileMeshInfo& info)
{
	std::ofstream fp = internal::saveFileStream(filename, "obj");

	std::ofstream mtlfp;
	std::string mtlFileName;
	bool useMtl =
		(info.hasVertexColors() && vcl::isPerVertexColorEnabled(m)) ||
		(info.hasTextures() &&
			((vcl::isPerVertexTexCoordEnabled(m) && info.hasVertexTexCoords()) ||
			 (vcl::isPerFaceWedgeTexCoordsEnabled(m) && info.hasFaceWedgeTexCoords()))) ||
		(info.hasFaceColors() && vcl::isPerFaceColorEnabled(m));
	if (useMtl) {
		mtlfp = internal::saveFileStream(filename, "mtl");
		mtlFileName = internal::addExtensionToFileName(filename, "mtl");
	}


	fp.close();
	if (useMtl) {
		mtlfp.close();
	}
}


} // namespace vcl::io
