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

#include "load.h"

#include "../internal/io_utils.h"
#include "../internal/io_read.h"

namespace vcl::io {

namespace internal {

bool isBinStlMalformed(const std::string& filename, bool& isBinary)
{
	std::size_t fsize = FileInfo::fileSize(filename);
	isBinary = FileInfo::isFileBinary(filename);

	if (isBinary) {
		// we can check if the size of the file is the expected one
		std::ifstream fp = internal::loadFileStream(filename);
		fp.seekg(80); // size of the header
		uint fnum = internal::readUInt<uint>(fp);
		std::size_t expectedFileSize =
			80 + 4 + // header and number of faces
			fnum * ( // for each face
				3 * sizeof(float) + // 3 floats for the face normal
				3 * 3 * sizeof(float)); // 3 floats for each vertex of the face
		if (expectedFileSize != fsize) {
			// sometimes the size is a bit wrong
			std::size_t diff = std::abs((long int)expectedFileSize - (long int)fsize);
			if (diff > fsize / 20)
				return true;
		}
	}

	return false;
}



} // namespace vcl::io::internal

template<MeshConcept MeshType>
MeshType loadStl(const std::string& filename, bool enableOptionalComponents)
{
	FileMeshInfo loadedInfo;
	return loadStl<MeshType>(filename, loadedInfo, enableOptionalComponents);
}

template<MeshConcept MeshType>
MeshType loadStl(
	const std::string& filename,
	FileMeshInfo&      loadedInfo,
	bool               enableOptionalComponents)
{
	MeshType m;
	loadStl(m, filename, loadedInfo, enableOptionalComponents);
	return m;
}

template<MeshConcept MeshType>
void loadStl(MeshType& m, const std::string& filename, bool enableOptionalComponents)
{
	FileMeshInfo loadedInfo;
	loadStl(m, filename, loadedInfo, enableOptionalComponents);
}

template<MeshConcept MeshType>
void loadStl(
	MeshType&          m,
	const std::string& filename,
	FileMeshInfo&      loadedInfo,
	bool               enableOptionalComponents)
{
	bool isBinary;
	if (internal::isBinStlMalformed(filename, isBinary))
		throw MalformedFileException(filename + " is malformed.");

	// todo
}

} // namespace vcl::io
