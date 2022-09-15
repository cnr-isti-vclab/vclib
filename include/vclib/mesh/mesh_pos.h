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

#ifndef VCL_MESH_POS_H
#define VCL_MESH_POS_H

#include "elements/face.h"

namespace vcl::mesh {

/**
 * @brief The MeshPos class describes a "Position in a Mesh" that can be identified with a triplet
 * of Face-Vertex-Edge, where:
 * - the Vertex is part of the Face;
 * - the edge is an index less than the number of vertices of the Face
 * - The Vertex is part of the Edge in the Face.
 *
 * Its utility is to navigate the topology of the Mesh trough a set of well-defined operations that
 * allow to "move" the MeshPos object.
 *
 * To be used, the MeshPos requires that the Mesh on which the given Face-Vertex-Edge triplet is
 * defined, has per Face Adjacent Faces topology informtion.
 */
template <face::HasAdjacentFaces FaceType>
class MeshPos
{
public:
	using VertexType = typename FaceType::VertexType;

	static bool isValid(const FaceType* f, const VertexType* v, short e);

	MeshPos();
	MeshPos(const FaceType* f, short e);
	MeshPos(const FaceType* f, const VertexType* v, short e);

	const FaceType* face() const;

	bool isValid() const;
	bool isNull() const;
	bool isEdgeOnBorder() const;

	bool flipFace();
	void flipVertex();
	void flipEdge();

	void nextEdgeAdjacentToV();
	void nextEdgeOnBorderAdjacentToV();

	uint numberOfAdjacentFacesToV() const;

	bool operator==(const MeshPos& op) const;
	bool operator!=(const MeshPos& op) const;
	bool operator<(const MeshPos& op) const;

private:
	const FaceType* f = nullptr;
	const VertexType* v = nullptr;
	short e = -1;

	uint countAdjacentFacesToV(bool& onBorder) const;
};

} // namespace vcl::mesh

#include "mesh_pos.cpp"

#endif // VCL_MESH_POS_H
