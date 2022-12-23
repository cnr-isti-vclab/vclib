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

#include "save_obj.h"

#include <map>
#include <vclib/exception/io_exception.h>
#include <vclib/misc/file_info.h>

#include "internal/io_utils.h"
#include "internal/io_write.h"
#include "obj/material.h"

namespace vcl::io {

namespace internal {

template<typename VertexType, typename MeshType>
obj::Material materialFromVertex(const VertexType& v, const FileMeshInfo& fi)
{
	obj::Material mat;
	if constexpr (HasPerVertexColor<MeshType>) {
		if (fi.hasVertexColors()) {
			mat.hasColor = true;
			mat.Kd.x()   = v.color().redF();
			mat.Kd.y()   = v.color().greenF();
			mat.Kd.z()   = v.color().blueF();
		}
	}
	return mat;
}

template<typename FaceType, typename MeshType>
obj::Material materialFromFace(const FaceType& f, const MeshType& m, const FileMeshInfo& fi)
{
	obj::Material mat;
	if (fi.hasFaceColors()) {
		mat.hasColor = true;
		mat.Kd.x()   = f.color().redF();
		mat.Kd.y()   = f.color().greenF();
		mat.Kd.z()   = f.color().blueF();
	}
	if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
		if (fi.hasFaceWedgeTexCoords()) {
			mat.hasTexture = true;
			mat.map_Kd = m.texture(f.textureIndex());
		}
	}
	return mat;
}

template<typename ElementType, typename MeshType>
void writeElementMaterial(
	const ElementType&                    e,
	const MeshType&                       m,
	const FileMeshInfo&                   fi,
	obj::Material&                        lastMaterial,
	std::map<obj::Material, std::string>& materialMap,
	std::ofstream&                        fp,
	std::ofstream&                        mtlfp)
{
	obj::Material mat;
	if constexpr(std::is_same<ElementType, typename MeshType::VertexType>::value)
		mat = materialFromVertex<typename MeshType::VertexType, MeshType>(e, fi);
	if constexpr(HasFaces<MeshType>)
		if constexpr(std::is_same<ElementType, typename MeshType::FaceType>::value)
			mat = materialFromFace(e, m, fi);
	if (!mat.isEmpty()) {
		static const std::string MATERIAL_PREFIX = "MATERIAL_";
		std::string mname; // name of the material of the vertex
		auto        it = materialMap.find(mat);
		if (it == materialMap.end()) { // if it is a new material
			// add the new material to the map
			mname            = MATERIAL_PREFIX + std::to_string(materialMap.size());
			materialMap[mat] = mname;
			// save the material in the mtl file
			mtlfp << "newmtl " << mname << std::endl;
			mtlfp << mat << std::endl;
		}
		else { // get the name of the material
			mname = it->second;
		}
		// if the material of the vertex is different from the last used, need to add usemtl
		if (mat != lastMaterial) {
			lastMaterial = mat;
			fp << "usemtl " << mname << std::endl;
		}
	}
}

} // namespace internal

template<typename MeshType>
void saveObj(const MeshType& m, const std::string& filename)
{
	vcl::io::FileMeshInfo info(m);
	saveObj(m, filename, info);
}

template<typename MeshType>
void saveObj(const MeshType& m, const std::string& filename, const FileMeshInfo& info)
{
	FileMeshInfo meshInfo(m);

	// make sure that the given info contains only components that are actually available in the
	// mesh. meshInfo will contain the intersection between the components that the user wants to
	// save and the components that are available in the mesh.
	meshInfo = info.intersect(meshInfo);

	// if the mesh has both vertex and wedge texcords, will be save just wedges because obj does
	// not allow to save them both. In any case, also vertex texcoords will result saved as
	// wedge texcoords in the final file.
	if (meshInfo.hasVertexTexCoords() && meshInfo.hasFaceWedgeTexCoords()) {
		meshInfo.setVertexTexCoords(false);
	}

	std::ofstream fp = internal::saveFileStream(filename, "obj");

	std::ofstream                        mtlfp;
	std::map<obj::Material, std::string> materialMap;

	bool useMtl = meshInfo.hasVertexColors() || meshInfo.hasFaceColors() ||
				  (meshInfo.hasTextures() &&
				   (meshInfo.hasVertexTexCoords() || meshInfo.hasFaceWedgeTexCoords()));
	if (useMtl) {
		mtlfp                   = internal::saveFileStream(filename, "mtl");
		std::string mtlFileName = vcl::fileInfo::filenameWithExtension(filename) + ".mtl";
		fp << "mtllib ./" << mtlFileName << std::endl;
	}

	obj::Material lastMaterial;

	// vertices
	using VertexType = typename MeshType::VertexType;
	for (const VertexType& v : m.vertices()) {
		if (useMtl) { // mtl management
			internal::writeElementMaterial<VertexType, MeshType>(v, m, meshInfo, lastMaterial, materialMap, fp, mtlfp);
		}
		fp << "v ";
		internal::writeDouble(fp, v.coord().x(), false);
		internal::writeDouble(fp, v.coord().y(), false);
		internal::writeDouble(fp, v.coord().z(), false);
		fp << std::endl;

		if constexpr (HasPerVertexNormal<MeshType>) {
			if (meshInfo.hasVertexNormals()) {
				fp << "vn ";
				internal::writeDouble(fp, v.normal().x(), false);
				internal::writeDouble(fp, v.normal().y(), false);
				internal::writeDouble(fp, v.normal().z(), false);
				fp << std::endl;
			}
		}
		if constexpr (HasPerVertexTexCoord<MeshType>) {
			if (meshInfo.hasVertexTexCoords()) {
				fp << "vt ";
				internal::writeFloat(fp, v.texCoord().u(), false);
				internal::writeFloat(fp, v.texCoord().v(), false);
				fp << std::endl;
			}
		}
	}

	// faces
	if constexpr (vcl::HasFaces<MeshType>) {
		using VertexType = typename MeshType::VertexType;
		using FaceType = typename MeshType::FaceType;

		// indices of vertices that do not consider deleted vertices
		std::vector<int> vIndices = m.vertexCompactIndices();

		uint wedgeTexCoord = 1;
		for (const FaceType& f : m.faces()) {
			if (useMtl) { // mtl management
				internal::writeElementMaterial(f, m, meshInfo, lastMaterial, materialMap, fp, mtlfp);
			}
			if constexpr(HasPerFaceWedgeTexCoords<MeshType>){
				if (meshInfo.hasFaceWedgeTexCoords()) {
					using WedgeTexCoordType = typename FaceType::WedgeTexCoordType;
					for (const WedgeTexCoordType wt : f.wedgeTexCoords()){
						fp << "vt ";
						internal::writeFloat(fp, wt.u(), false);
						internal::writeFloat(fp, wt.v(), false);
						fp << std::endl;
					}
				}
			}

			fp << "f ";
			for (const VertexType* v : f.vertices()) {
				fp << vIndices[m.index(v)]+1;
				if constexpr(HasPerVertexTexCoord<MeshType>){
					// we wrote texcoords along with vertices, each texcoord has the same index
					// of its vertex
					if (meshInfo.hasVertexTexCoords()) {
						fp << "/" << vIndices[m.index(v)]+1;
					}
				}
				if constexpr(HasPerFaceWedgeTexCoords<MeshType>){
					// we wrote texcoords before the face; indices are consecutive and wedge coords
					// are the same of the number of vertices of the face
					if (meshInfo.hasFaceWedgeTexCoords()) {
						fp << "/" << wedgeTexCoord++;
					}
				}
				fp << " ";
			}
			fp << std::endl;
		}
	}

	fp.close();
	if (useMtl) {
		mtlfp.close();
	}
}

} // namespace vcl::io
