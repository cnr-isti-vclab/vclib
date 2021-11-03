/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "ply_vertex.h"
#include <mgp/misc/tokenizer.h>
#include <mgp/mesh/requirements.h>

namespace mgp {
namespace ply {

namespace internal {

template <typename A, typename B, typename C>
bool loadVerticesTxt(
		std::ifstream& file,
		const PlyHeader& header,
		A vertices[],
		B vertexNormals[],
		io::FileMeshInfo::ColorMode colorMod ,
		C vertexColors[])
{
	bool error = false;
	uint colorStep = 3;
	if (colorMod == io::FileMeshInfo::RGBA)
		colorStep = 4;
	mgp::Tokenizer spaceTokenizer;
	error = !internal::nextLine(file, spaceTokenizer);
	mgp::Tokenizer::iterator token = spaceTokenizer.begin();
	for(uint v = 0; v < header.numberVertices(); ++v) {
		for (ply::Property p : header.vertexProperties()) {
			if (token == spaceTokenizer.end()){
				error = !nextLine(file, spaceTokenizer);
				token = spaceTokenizer.begin();
			}
			if (error) return false;
			switch (p.name) {
				case ply::x :
					vertices[v*3] = internal::readProperty<A>(token, p.type); break;
				case ply::y :
					vertices[v*3+1] = internal::readProperty<A>(token, p.type); break;
				case ply::z :
					vertices[v*3+2] = internal::readProperty<A>(token, p.type); break;
				case ply::nx :
					vertexNormals[v*3] = internal::readProperty<B>(token, p.type); break;
				case ply::ny :
					vertexNormals[v*3+1] = internal::readProperty<B>(token, p.type); break;
				case ply::nz :
					vertexNormals[v*3+2] = internal::readProperty<B>(token, p.type); break;
				case ply::red :
					vertexColors[v*colorStep] = internal::readProperty<C>(token, p.type, true); break;
				case ply::green :
					vertexColors[v*colorStep+1] = internal::readProperty<C>(token, p.type, true); break;
				case ply::blue :
					vertexColors[v*colorStep+2] = internal::readProperty<C>(token, p.type, true); break;
				case ply::alpha :
					if(colorStep == 4){ // alpha in file that will be saved
						vertexColors[v*colorStep+3] = internal::readProperty<C>(token, p.type, true);;
					}
					else { // alpha in file that will not be saved
						token++;
					}
					break;
				default:
					if (p.list){
						uint s = internal::readProperty<int>(token, p.listSizeType);
						for (uint i = 0; i < s; ++i)
							internal::readProperty<int>(token, p.type);
					}
					else {
						internal::readProperty<int>(token, p.type);
					}
			}
		}
	}
	return !error;
}

template <typename A, typename B, typename C>
bool loadVerticesBin(
		std::ifstream& file,
		const PlyHeader& header,
		A vertices[],
		B vertexNormals[],
		io::FileMeshInfo::ColorMode colorMod ,
		C vertexColors[])
{
	uint colorStep = 3;
	if (colorMod == io::FileMeshInfo::RGBA)
		colorStep = 4;
	for(uint v = 0; v < header.numberVertices(); ++v) {
		for (ply::Property p : header.vertexProperties()) {
			switch (p.name) {
				case ply::x :
					vertices[v*3] = internal::readProperty<A>(file, p.type); break;
				case ply::y :
					vertices[v*3+1] = internal::readProperty<A>(file, p.type); break;
				case ply::z :
					vertices[v*3+2] = internal::readProperty<A>(file, p.type); break;
				case ply::nx :
					vertexNormals[v*3] = internal::readProperty<B>(file, p.type); break;
				case ply::ny :
					vertexNormals[v*3+1] = internal::readProperty<B>(file, p.type); break;
				case ply::nz :
					vertexNormals[v*3+2] = internal::readProperty<B>(file, p.type); break;
				case ply::red :
					vertexColors[v*colorStep] = internal::readProperty<C>(file, p.type, true); break;
				case ply::green :
					vertexColors[v*colorStep+1] = internal::readProperty<C>(file, p.type, true); break;
				case ply::blue :
					vertexColors[v*colorStep+2] = internal::readProperty<C>(file, p.type, true); break;
				case ply::alpha :
					if (colorStep == 4)
						vertexColors[v*colorStep+3] = internal::readProperty<C>(file, p.type, true);
					else
						internal::readProperty<C>(file, p.type, true); //read without save anywhere
					break;
				default:
					if (p.list){
						uint s = internal::readProperty<int>(file, p.listSizeType);
						for (uint i = 0; i < s; ++i)
							internal::readProperty<int>(file, p.type);
					}
					else {
						internal::readProperty<int>(file, p.type);
					}
			}
		}
	}
	return true;
}

} //namespace mgp::ply::internal

template <typename MeshType>
void saveVertices(
	std::ofstream& file,
	const PlyHeader& header,
	const MeshType& mesh)
{
	using VertexType = typename MeshType::Vertex;

	bool bin = header.format() == ply::BINARY;
	for(const VertexType& v : mesh.vertexIterator()) {
		for (ply::Property p : header.vertexProperties()) {
			bool hasBeenWritten = false;
			if (p.name >= ply::x && p.name <= ply::z) {
				if constexpr (mgp::hasPerVertexCoordinate(mesh)) {
					int a = p.name - ply::x;
					internal::writeProperty(file, v.coordinate()[a], p.type, bin);
					hasBeenWritten = true;
				}
			}
			if (p.name >= ply::nx && p.name <= ply::nz) {
				if constexpr (mgp::hasPerVertexNormal(mesh)) {
					if (mgp::isPerVertexNormalEnabled(mesh)) {
						internal::writeProperty(file, v.normal()[p.name - ply::nx], p.type, bin);
						hasBeenWritten = true;
					}
				}
			}
			if (p.name >= ply::red && p.name <= ply::alpha) {
				if constexpr (mgp::hasPerVertexColor(mesh)) {
					if (mgp::isPerVertexColorEnabled(mesh)) {
						internal::writeProperty(file, v.color()[p.name - ply::red], p.type, bin);
						hasBeenWritten = true;
					}
				}
			}
			if (!hasBeenWritten){
				// be sure to write something if the header declares some property that is not
				// in the mesh
				internal::writeProperty(file, 0, p.type, bin);
			}
		}
		if (!bin)
			file << std::endl;
	}
}

template <typename A, typename B, typename C>
bool loadVertices(
		std::ifstream& file,
		const PlyHeader& header,
		A vertices[],
		B vertexNormals[],
		io::FileMeshInfo::ColorMode colorMod ,
		C vertexColors[])
{
	if(header.format() == ply::ASCII) {
		return internal::loadVerticesTxt(file, header, vertices, vertexNormals, colorMod, vertexColors);
	}
	else if(header.format() == ply::BINARY) {
		return internal::loadVerticesBin(file, header, vertices, vertexNormals, colorMod, vertexColors);
	}
	else return false;
}

} //namespace mgp::ply
} //namespace mgp
