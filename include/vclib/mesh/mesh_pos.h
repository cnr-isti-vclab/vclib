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

#ifndef VCL_MESH_POS_H
#define VCL_MESH_POS_H

#include "elements/face.h"

namespace vcl::mesh {

template <typename FaceType>
class MeshPos
{
	static_assert(
		face::hasAdjacentFaces<FaceType>(),
		"FaceType used for MeshPos has no Adjacent Faces.");

public:
	using VertexType = typename FaceType::VertexType;

	static bool isValid(const FaceType* f, const VertexType* v, short e);

	MeshPos();
	MeshPos(const FaceType* f, const VertexType* v, short e);

	bool isValid() const;
	bool isNull() const;

	bool canFlipFace() const;

	void flipFace();
	void flipVertex();
	void flipEdge();

private:
	const FaceType* f = nullptr;
	const VertexType* v = nullptr;
	short e = -1;
};

} // namespace vcl::mesh

#endif // VCL_MESH_POS_H
