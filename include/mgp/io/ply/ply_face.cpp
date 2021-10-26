/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "ply_face.h"

namespace mgp {
namespace ply {

namespace internal {

template<typename A, typename D>
void saveFaceIndices(
	std::ofstream&          file,
	Property                p,
	uint                    f,
	uint&                   startingIndex,
	const A                 faces[],
	const io::FileMeshInfo& meshType,
	const D                 polygonSizes[],
	bool                    bin)
{
	assert(p.list);

	uint fsize;
	if (meshType.isTriangleMesh()) {
		fsize         = 3;
		startingIndex = f * 3;
	}
	else if (meshType.isQuadMesh()) {
		fsize         = 4;
		startingIndex = f * 4;
	}
	else {
		fsize = polygonSizes[f];
	}

	internal::writeProperty(file, fsize, p.listSizeType, bin);

	for (uint k = 0; k < fsize; ++k)
		internal::writeProperty(file, faces[startingIndex + k], p.type, bin);

	if (meshType.isPolygonMesh())
		startingIndex += polygonSizes[f];
}

// load

template<template<typename... Args> class Container, typename A, typename D>
bool loadFaceIndicesTxt(
	const mgp::Tokenizer&     spaceTokenizer,
	mgp::Tokenizer::iterator& token,
	Property                  p,
	uint                      f,
	Container<A>&             faces,
	D                         polygonSizes[])
{
	if (!p.list)
		return false;
	if (token == spaceTokenizer.end())
		return false;

	uint fSize      = internal::readProperty<uint>(token, p.listSizeType);
	polygonSizes[f] = fSize;
	for (uint i = 0; i < fSize; ++i) {
		if (token == spaceTokenizer.end())
			return false;
		faces.push_back(internal::readProperty<A>(token, p.type));
	}
	return true;
}

template<typename A, typename B, typename C, typename D>
bool loadFacesTxt(
	std::ifstream&              file,
	const PlyHeader&            header,
	A&                          faces,
	io::FileMeshInfo::MeshType& meshType,
	B                           faceNormals[],
	io::FileMeshInfo::ColorMode colorMod,
	C                           faceColors[],
	D                           polygonSizes[])
{
	bool error     = false;
	uint colorStep = 3;
	if (colorMod == io::FileMeshInfo::RGBA)
		colorStep = 4;
	mgp::Tokenizer spaceTokenizer;
	error                          = !internal::nextLine(file, spaceTokenizer);
	mgp::Tokenizer::iterator token = spaceTokenizer.begin();
	for (uint f = 0; f < header.numberFaces(); ++f) {
		for (ply::Property p : header.faceProperties()) {
			if (token == spaceTokenizer.end()) {
				error = !nextLine(file, spaceTokenizer);
				token = spaceTokenizer.begin();
			}
			if (error)
				return false;
			switch (p.name) {
			case ply::nx: faceNormals[f * 3] = internal::readProperty<B>(token, p.type); break;
			case ply::ny: faceNormals[f * 3 + 1] = internal::readProperty<B>(token, p.type); break;
			case ply::nz: faceNormals[f * 3 + 2] = internal::readProperty<B>(token, p.type); break;
			case ply::red:
				faceColors[f * colorStep] = internal::readProperty<C>(token, p.type, true);
				break;
			case ply::green:
				faceColors[f * colorStep + 1] = internal::readProperty<C>(token, p.type, true);
				break;
			case ply::blue:
				faceColors[f * colorStep + 2] = internal::readProperty<C>(token, p.type, true);
				break;
			case ply::alpha:
				if (colorStep == 4) { // alpha in file that will be saved
					faceColors[f * colorStep + 3] = internal::readProperty<C>(token, p.type, true);
				}
				else { // alpha in file that will not be saved
					++token;
				}
				break;
			case ply::vertex_indices:
				error = !loadFaceIndicesTxt(spaceTokenizer, token, p, f, faces, polygonSizes);
				break;
			default: // reading everything else with non-recognised name
				if (p.list) {
					uint s = internal::readProperty<int>(token, p.listSizeType);
					for (uint i = 0; i < s; ++i)
						internal::readProperty<int>(token, p.type);
				}
				else {
					internal::readProperty<int>(token, p.type);
				}
			}
			if (error)
				return false;
		}

		if (f == 0) { // first face
			if (polygonSizes[f] == 3)
				meshType = io::FileMeshInfo::TRIANGLE_MESH;
			else if (polygonSizes[f] == 3)
				meshType = io::FileMeshInfo::QUAD_MESH;
			else
				meshType = io::FileMeshInfo::POLYGON_MESH;
		}
		else {
			if (meshType == io::FileMeshInfo::TRIANGLE_MESH && polygonSizes[f] != 3)
				meshType = io::FileMeshInfo::POLYGON_MESH;
			else if (meshType == io::FileMeshInfo::QUAD_MESH && polygonSizes[f] != 4)
				meshType = io::FileMeshInfo::POLYGON_MESH;
		}
	}
	return true;
}

template<template<typename... Args> class Container, typename A, typename D>
bool loadFaceIndicesBin(
	std::ifstream& file,
	Property       p,
	uint           f,
	Container<A>&  faces,
	D              polygonSizes[])
{
	if (!p.list)
		return false;

	uint fsize;
	fsize           = internal::readProperty<uint>(file, p.listSizeType);
	polygonSizes[f] = fsize;

	for (uint k = 0; k < fsize; ++k)
		faces.push_back(internal::readProperty<A>(file, p.type));

	return true;
}

template<typename A, typename B, typename C, typename D>
bool loadFacesBin(
	std::ifstream&              file,
	const PlyHeader&            header,
	A&                          faces,
	io::FileMeshInfo::MeshType& meshType,
	B                           faceNormals[],
	io::FileMeshInfo::ColorMode colorMod,
	C                           faceColors[],
	D                           polygonSizes[])
{
	uint colorStep = 3;
	if (colorMod == io::FileMeshInfo::RGBA)
		colorStep = 4;
	for (uint f = 0; f < header.numberFaces(); ++f) {
		for (ply::Property p : header.faceProperties()) {
			switch (p.name) {
			case ply::nx: faceNormals[f * 3] = internal::readProperty<B>(file, p.type); break;
			case ply::ny: faceNormals[f * 3 + 1] = internal::readProperty<B>(file, p.type); break;
			case ply::nz: faceNormals[f * 3 + 2] = internal::readProperty<B>(file, p.type); break;
			case ply::red:
				faceColors[f * colorStep] = internal::readProperty<C>(file, p.type);
				break;
			case ply::green:
				faceColors[f * colorStep + 1] = internal::readProperty<C>(file, p.type);
				break;
			case ply::blue:
				faceColors[f * colorStep + 2] = internal::readProperty<C>(file, p.type);
				break;
			case ply::alpha:
				if (colorStep == 4)
					faceColors[f * colorStep + 3] = internal::readProperty<C>(file, p.type);
				else
					internal::readProperty<C>(file, p.type);
				;
				break;
			case ply::vertex_indices: loadFaceIndicesBin(file, p, f, faces, polygonSizes); break;
			default:
				if (p.list) {
					uint s = internal::readProperty<int>(file, p.listSizeType);
					for (uint i = 0; i < s; ++i)
						internal::readProperty<int>(file, p.type);
				}
				else {
					internal::readProperty<int>(file, p.type);
				}
			}
		}
		if (f == 0) { // modify meshType
			if (polygonSizes[f] == 3)
				meshType = io::FileMeshInfo::TRIANGLE_MESH;
			else if (polygonSizes[f] == 3)
				meshType = io::FileMeshInfo::QUAD_MESH;
			else
				meshType = io::FileMeshInfo::POLYGON_MESH;
		}
		else {
			if (meshType == io::FileMeshInfo::TRIANGLE_MESH && polygonSizes[f] != 3)
				meshType = io::FileMeshInfo::POLYGON_MESH;
			else if (meshType == io::FileMeshInfo::QUAD_MESH && polygonSizes[f] != 4)
				meshType = io::FileMeshInfo::POLYGON_MESH;
		}
	}
	return true;
}

} // namespace internal

template<typename A, typename B, typename C, typename D>
void saveFaces(
	std::ofstream&              file,
	const PlyHeader&            header,
	const A                     faces[],
	io::FileMeshInfo            meshMode,
	const B                     faceNormals[],
	io::FileMeshInfo::ColorMode colorMod,
	const C                     faceColors[],
	const D                     polygonSizes[])
{
	bool bin           = header.format() == ply::BINARY;
	uint colorStep     = 3;
	uint startingIndex = 0;
	if (colorMod == io::FileMeshInfo::RGBA)
		colorStep = 4;
	for (uint f = 0; f < header.numberFaces(); ++f) {
		for (ply::Property p : header.faceProperties()) {
			switch (p.name) {
			case ply::nx: internal::writeProperty(file, faceNormals[f * 3], p.type, bin); break;
			case ply::ny: internal::writeProperty(file, faceNormals[f * 3 + 1], p.type, bin); break;
			case ply::nz: internal::writeProperty(file, faceNormals[f * 3 + 2], p.type, bin); break;
			case ply::red:
				internal::writeProperty(file, faceColors[f * colorStep], p.type, bin, true);
				break;
			case ply::green:
				internal::writeProperty(file, faceColors[f * colorStep + 1], p.type, bin, true);
				break;
			case ply::blue:
				internal::writeProperty(file, faceColors[f * colorStep + 2], p.type, bin, true);
				break;
			case ply::alpha:
				if (colorStep == 4)
					internal::writeProperty(file, faceColors[f * colorStep + 3], p.type, bin, true);
				else
					internal::writeProperty(file, (p.type < 6 ? 255 : 1), p.type, bin, true);
				;
				break;
			case ply::vertex_indices:
				internal::saveFaceIndices(
					file, p, f, startingIndex, faces, meshMode, polygonSizes, bin);
				break;
			default: internal::writeProperty(file, 0, p.type, bin); break;
			}
		}
		if (!bin)
			file << std::endl;
	}
}

template<typename A, typename B, typename C, typename D>
bool loadFaces(
	std::ifstream&              file,
	const PlyHeader&            header,
	A&                          faces,
	io::FileMeshInfo::MeshType& meshType,
	B                           faceNormals[],
	io::FileMeshInfo::ColorMode colorMod,
	C                           faceColors[],
	D                           polygonSizes[])
{
	if (header.format() == ply::ASCII) {
		return internal::loadFacesTxt(
			file, header, faces, meshType, faceNormals, colorMod, faceColors, polygonSizes);
	}
	else {
		return internal::loadFacesBin(
			file, header, faces, meshType, faceNormals, colorMod, faceColors, polygonSizes);
	}
}

} // namespace ply
} // namespace mgp
