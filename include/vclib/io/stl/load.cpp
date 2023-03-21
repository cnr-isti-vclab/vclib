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
				3 * 3 * sizeof(float) + // 3 floats for each vertex of the face
				sizeof(unsigned short)); // a short containing attributes
		if (expectedFileSize != fsize) {
			// sometimes the size is a bit wrong
			std::size_t diff = std::abs((long int)expectedFileSize - (long int)fsize);
			if (diff > fsize / 20)
				return true;
		}
	}

	return false;
}

bool isStlColored(std::ifstream& fp, bool& magicsMode)
{
	bool colored = false;

	char buf[80];
	fp.read(buf, 80);
	std::string s(buf);
	size_t cInd = s.rfind("COLOR=");
	size_t mInd = s.rfind("MATERIAL=");
	if(cInd!=std::string::npos && mInd!=std::string::npos)
		magicsMode = true;
	else
		magicsMode = false;
	uint fnum = internal::readUInt<uint>(fp);
	static const uint fmax = 1000;
	static const uint fdataSize = 12 * sizeof(float); // 3 floats for normal and 9 for vcoords

	for (uint i = 0; i < std::min(fnum, fmax); ++i) {
		fp.seekg(fdataSize, std::ios::cur);
		unsigned short attr = internal::readShort<unsigned short>(fp);
		Color c; c.setFromUnsignedR5G5B5(attr);
		if (c != Color::White)
			colored = true;
	}
	return colored;
}

template<MeshConcept MeshType>
void loadStlBin(
	MeshType&      m,
	std::ifstream& fp,
	FileMeshInfo&  loadedInfo,
	bool           enableOptionalComponents)
{
	bool magicsMode, colored;
	colored = isStlColored(fp, magicsMode);

	if (enableOptionalComponents) {
		if (colored)
			loadedInfo.setFaceColors();
		internal::enableOptionalComponents(loadedInfo, m);
	}
	else if (colored) {
		if constexpr (HasPerFaceColor<MeshType>) {
			if (isPerFaceColorEnabled(m))
				loadedInfo.setFaceColors();
		}
	}

	fp.seekg(80); // size of the header
	uint fnum = internal::readUInt<uint>(fp);

	m.addVertices(fnum * 3);
	if constexpr(HasFaces<MeshType>) {
		m.reserveFaces(fnum);
	}

	uint vi = 0;
	for (uint i = 0; i < fnum; ++i) {
		Point3f norm;
		norm.x() = internal::readFloat<float>(fp);
		norm.y() = internal::readFloat<float>(fp);
		norm.z() = internal::readFloat<float>(fp);

		for (uint j = 0; j < 3; ++j) {
			m.vertex(vi + j).coord().x() = internal::readFloat<float>(fp);
			m.vertex(vi + j).coord().y() = internal::readFloat<float>(fp);
			m.vertex(vi + j).coord().z() = internal::readFloat<float>(fp);
		}

		unsigned short attr = internal::readShort<unsigned short>(fp);

		if constexpr (HasFaces<MeshType>) {
			using FaceType = typename MeshType::FaceType;

			uint fi = m.addFace();
			FaceType& f = m.face(fi);
			for (uint j = 0; j < 3; ++j)
				f.vertex(j) = &m.vertex(vi + j);
			if (HasPerFaceNormal<MeshType>) {
				using ST = typename FaceType::NormalType::ScalarType;
				if (isPerFaceNormalEnabled(m)) {
					f.normal() = norm.cast<ST>();
				}
			}
			if (HasPerFaceColor<MeshType>) {
				if (isPerFaceColorEnabled(m) && colored) {
					Color c;
					if (magicsMode)
						c.setFromUnsignedR5G5B5(attr);
					else
						c.setFromUnsignedB5G5R5(attr);
					f.color() = c;
				}
			}
		}


		vi += 3;
	}
}

template<MeshConcept MeshType>
void loadStlAscii(
	MeshType&      m,
	std::ifstream& fp)
{

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

	std::ifstream fp = internal::loadFileStream(filename);

	loadedInfo = FileMeshInfo();
	loadedInfo.setVertices();
	loadedInfo.setVertexCoords();
	if constexpr(HasFaces<MeshType>) {
		loadedInfo.setFaces();
		loadedInfo.setFaceVRefs();
		loadedInfo.setFaceNormals();
	}

	if constexpr (HasName<MeshType>) {
		m.name() = FileInfo::filenameWithoutExtension(filename);
	}

	if (isBinary)
		internal::loadStlBin(m, fp, loadedInfo, enableOptionalComponents);
	else
		internal::loadStlAscii(m, fp);
}

} // namespace vcl::io
