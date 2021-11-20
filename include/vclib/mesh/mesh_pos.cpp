/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

#include "mesh_pos.h"

namespace vcl::mesh {

/**
 * @brief Helper function to check if a MeshPos is valid, that is if:
 * - v and f are valid vertex/face pointers, and e >= 0
 * - e is less than the number of vertices of f (that is the number of edges of f)
 * - v is the vertex of f in position e or (e+1)%f->numberVertices()
 *
 * @param f: a face pointer
 * @param v: a vertex pointer
 * @param e: an integer that represents the edge position in the face f
 * @return true if the triple (f,v,e) is a valid MeshPos
 */
template<typename FaceType>
bool MeshPos<FaceType>::isValid(const FaceType* f, const VertexType* v, short e)
{
	if (f == nullptr || v == nullptr || e < 0) return false;
	return e < f->vertexNumber() && (v == f->vert(e) || v == f->vertMod(e+1));
}

template<typename FaceType>
MeshPos<FaceType>::MeshPos()
{
}

template<typename FaceType>
MeshPos<FaceType>::MeshPos(const FaceType* f, const VertexType* v, short e) :
		f(f), v(v), e(e)
{
	assert(isValid(f, v, e));
}

template<typename FaceType>
bool MeshPos<FaceType>::isValid() const
{
	return isValid(f, v, e);
}

template<typename FaceType>
bool MeshPos<FaceType>::isNull() const
{
	return f == nullptr || v == nullptr || e < 0;
}

template<typename FaceType>
bool MeshPos<FaceType>::canFlipFace() const
{
	return isValid() && f->adjFace(e) != nullptr;
}

template<typename FaceType>
void MeshPos<FaceType>::flipFace()
{
	FaceType* nf = f->adjFace(e);
	e = nf->indexOfAdjFace(f);
	f = nf;
}

template<typename FaceType>
void MeshPos<FaceType>::flipVertex()
{
	if (f->vertexMod(e) == v) {
		v = f->vertexMod(e+1);
	}
	else {
		v = f->vertexMod(e);
	}
}

template<typename FaceType>
void MeshPos<FaceType>::flipEdge()
{
	if (f->vertexMod(e+1) == v) {
		e = (e + 1) % f->vertexNumber();
	}
	else {
		short n = f->vertexNumber();
		e = (e % n + n) % n; // be sure to get the right index of the previous edge
	}
}

} // namespace vcl::mesh
