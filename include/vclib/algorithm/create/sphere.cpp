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

#include "sphere.h"

#include "icosahedron.h"

#include <vclib/algorithm/clean.h>
#include <vclib/algorithm/update/transform.h>

namespace vcl {

namespace internal {

namespace cts  { // Cube to Sphere

static const vcl::Point3d origins[6] = {
	Point3d(-1.0, -1.0, -1.0),
	Point3d(1.0, -1.0, -1.0),
	Point3d(1.0, -1.0, 1.0),
	Point3d(-1.0, -1.0, 1.0),
	Point3d(-1.0, 1.0, -1.0),
	Point3d(-1.0, -1.0, 1.0)};

static const Point3d rights[6] = {
	Point3d(2.0, 0.0, 0.0),
	Point3d(0.0, 0.0, 2.0),
	Point3d(-2.0, 0.0, 0.0),
	Point3d(0.0, 0.0, -2.0),
	Point3d(2.0, 0.0, 0.0),
	Point3d(2.0, 0.0, 0.0)};

static const Point3d ups[6] = {
	Point3d(0.0, 2.0, 0.0),
	Point3d(0.0, 2.0, 0.0),
	Point3d(0.0, 2.0, 0.0),
	Point3d(0.0, 2.0, 0.0),
	Point3d(0.0, 0.0, 2.0),
	Point3d(0.0, 0.0, -2.0)};

} // namespace vcl::internal::cts

} // namespace vcl::internal

template<FaceMeshConcept MeshType, typename ScalarType>
MeshType createSphereUV(const Sphere<ScalarType>& sp, uint parallels, uint meridians)
{
	using VertexType = typename MeshType::VertexType;
	using CoordType = typename VertexType::CoordType;
	using Facetype = typename MeshType::FaceType;

	MeshType mesh;

	mesh.addVertex(CoordType(0, 1, 0));
	for (uint j = 0; j < parallels - 1; ++j) {
		double const polar = M_PI * double(j+1) / double(parallels);
		double const sp = std::sin(polar);
		double const cp = std::cos(polar);
		for (uint i = 0; i < meridians; ++i) {
			double const azimuth = 2.0 * M_PI * double(i) / double(meridians);
			double const sa = std::sin(azimuth);
			double const ca = std::cos(azimuth);
			double const x = sp * ca;
			double const y = cp;
			double const z = sp * sa;
			mesh.addVertex(CoordType(x, y, z));
		}
	}
	mesh.addVertex(CoordType(0, -1, 0));

	for (uint i = 0; i < meridians; ++i) {
		VertexType* v = &mesh.vertex(0);
		VertexType* a = &mesh.vertex(i + 1);
		VertexType* b = &mesh.vertex((i + 1) % meridians + 1);
		mesh.addFace(0, b, a);
	}

	for (uint32_t j = 0; j < parallels - 2; ++j) {
		uint32_t aStart = j * meridians + 1;
		uint32_t bStart = (j + 1) * meridians + 1;
		for (uint32_t i = 0; i < meridians; ++i) {
			VertexType* a = &mesh.vertex(aStart + i);
			VertexType* a1 = &mesh.vertex(aStart + (i + 1) % meridians);
			VertexType* b = &mesh.vertex(bStart + i);
			VertexType* b1 = &mesh.vertex(bStart + (i + 1) % meridians);
			if constexpr(HasTriangles<MeshType>) {
				mesh.addFace(a, a1, b1);
				mesh.addFace(b1, b, a);
			}
			else {
				mesh.addFace(a, a1, b1, b);
			}
		}
	}

	for (uint32_t i = 0; i < meridians; ++i) {
		VertexType* a = &mesh.vertex(i + meridians * (parallels - 2) + 1);
		VertexType* b = &mesh.vertex((i + 1) % meridians + meridians * (parallels - 2) + 1);
		VertexType* v = &mesh.vertex(mesh.vertexNumber() - 1);
		mesh.addFace(v, a, b);
	}

	vcl::scale(mesh, sp.radius());
	vcl::translate(mesh, sp.center());

	return mesh;
}

template<FaceMeshConcept MeshType, typename ScalarType>
MeshType createSphereNormalizedCube(const Sphere<ScalarType>& sp, uint divisions)
{
	using VertexType = typename MeshType::VertexType;
	using CoordType = typename VertexType::CoordType;
	using Facetype = typename MeshType::FaceType;

	MeshType mesh;

	const double step = 1.0 / double(divisions);
	const CoordType step3(step, step, step);

	for (uint face = 0; face < 6; ++face) {
		const CoordType origin = internal::cts::origins[face];
		const CoordType right = internal::cts::rights[face];
		const CoordType up = internal::cts::ups[face];
		for (uint j = 0; j < divisions + 1; ++j) {
			const CoordType j3(j, j, j);
			for (uint i = 0; i < divisions + 1; ++i) {
				const CoordType i3(i, i, i);
				const CoordType p = origin + step3.mul(i3.mul(right) + j3.mul(up));

				mesh.addVertex(p.normalized());
			}
		}
	}

	const uint k = divisions + 1;
	for (uint face = 0; face < 6; ++face) {
		for (uint j = 0; j < divisions; ++j) {
			const bool bottom = j < (divisions / 2);
			for (uint i = 0; i < divisions; ++i) {
				const bool left = i < (divisions / 2);
				VertexType* a = &mesh.vertex((face * k + j) * k + i);
				VertexType* b = &mesh.vertex((face * k + j) * k + i + 1);
				VertexType* c = &mesh.vertex((face * k + j + 1) * k + i);
				VertexType* d = &mesh.vertex((face * k + j + 1) * k + i + 1);

				if constexpr (vcl::HasTriangles<MeshType>) {
					if (bottom ^ left) {
						mesh.addFace(a, c, b);
						mesh.addFace(c, d, b);
					}
					else {
						mesh.addFace(a, c, d);
						mesh.addFace(a, d, b);
					}
				}
				else {
					mesh.addFace(a, c, d, b);
				}
			}
		}
	}

	vcl::scale(mesh, sp.radius());
	vcl::translate(mesh, sp.center());

	return mesh;
}

template<FaceMeshConcept MeshType, typename ScalarType>
MeshType createSphereSpherifiedCube(const Sphere<ScalarType>& sp, uint divisions)
{
	using VertexType = typename MeshType::VertexType;
	using CoordType = typename VertexType::CoordType;
	using Facetype = typename MeshType::FaceType;

	MeshType mesh;

	const double step = 1.0 / double(divisions);
	const CoordType step3(step, step, step);

	for (uint face = 0; face < 6; ++face) {
		const CoordType origin = internal::cts::origins[face];
		const CoordType right = internal::cts::rights[face];
		const CoordType up = internal::cts::ups[face];
		for (uint j = 0; j < divisions + 1; ++j) {
			const CoordType j3(j, j, j);
			for (uint i = 0; i < divisions + 1; ++i) {
				const CoordType i3(i, i, i);
				const CoordType p = origin + step3.mul(i3.mul(right) + j3.mul(up));
				const CoordType p2 = p.mul(p);
				const CoordType n(
					p.x() * std::sqrt(1.0 - 0.5 * (p2.y() + p2.z()) + p2.y() * p2.z() / 3.0),
					p.y() * std::sqrt(1.0 - 0.5 * (p2.z() + p2.x()) + p2.z() * p2.x() / 3.0),
					p.z() * std::sqrt(1.0 - 0.5 * (p2.x() + p2.y()) + p2.x() * p2.y() / 3.0));
				mesh.addVertex(n);
			}
		}
	}

	const uint k = divisions + 1;
	for (uint face = 0; face < 6; ++face) {
		for (uint j = 0; j < divisions; ++j) {
			const bool bottom = j < (divisions / 2);
			for (uint i = 0; i < divisions; ++i) {
				const bool left = i < (divisions / 2);
				VertexType* a = &mesh.vertex((face * k + j) * k + i);
				VertexType* b = &mesh.vertex((face * k + j) * k + i + 1);
				VertexType* c = &mesh.vertex((face * k + j + 1) * k + i);
				VertexType* d = &mesh.vertex((face * k + j + 1) * k + i + 1);
				if constexpr (vcl::HasTriangles<MeshType>) {
					if (bottom ^ left) {
						mesh.addFace(a, c, b);
						mesh.addFace(c, d, b);
					}
					else {
						mesh.addFace(a, c, d);
						mesh.addFace(a, d, b);
					}
				}
				else {
					mesh.addFace(a, c, d, b);
				}
			}
		}
	}

	vcl::scale(mesh, sp.radius());
	vcl::translate(mesh, sp.center());

	return mesh;
}

template<FaceMeshConcept MeshType, typename ScalarType>
MeshType createSphereIcosahedron(const Sphere<ScalarType>& sp, uint divisions)
{
	using VertexType = typename MeshType::VertexType;
	using CoordType = typename VertexType::CoordType;
	using FaceType = typename MeshType::FaceType;

	MeshType mesh = vcl::createIcosahedron<MeshType>(true);

	for (uint d = 0; d < divisions; d++) {

		uint nf = mesh.faceNumber();
		for (uint f = 0; f < nf; f++) {
			FaceType& f0 = mesh.face(f);
			VertexType& v0 = *f0.vertex(0);
			VertexType& v1 = *f0.vertex(1);
			VertexType& v2 = *f0.vertex(2);
			uint v1id = mesh.index(v1);
			uint v2id = mesh.index(v2);

			CoordType pa = (v0.coord() + v1.coord()); pa.normalize();
			CoordType pb = (v1.coord() + v2.coord()); pb.normalize();
			CoordType pc = (v2.coord() + v0.coord()); pc.normalize();
			uint vaid = mesh.addVertex(pa);
			uint vbid = mesh.addVertex(pb);
			uint vcid = mesh.addVertex(pc);

			f0.vertex(1) = &mesh.vertex(vaid);
			f0.vertex(2) = &mesh.vertex(vcid);
			mesh.addFace(vaid, v1id, vbid);
			mesh.addFace(vcid, vbid, v2id);
			mesh.addFace(vaid, vbid, vcid);
		}
	}

	vcl::removeDuplicatedVertices(mesh);

	vcl::scale(mesh, sp.radius());
	vcl::translate(mesh, sp.center());

	return mesh;
}

/**
 * @brief Creates a Sphere Mesh centered in `center` point and having radius `radius`, using
 * the generation method given in the argument args.mode (see https://github.com/caosdoar/spheres
 * for more details).
 * @param center
 * @param radius
 * @param args
 * @return
 */
template<FaceMeshConcept MeshType, typename ScalarType>
MeshType createSphere(const Sphere<ScalarType>& sp, const CreateSphereArgs& args)
{
	MeshType m;
	switch (args.mode) {
	case CreateSphereArgs::UV:
		m = createSphereUV<MeshType>(sp, args.parallels, args.meridians);
		break;
	case CreateSphereArgs::NORMALIZED_CUBE:
		m = createSphereNormalizedCube<MeshType>(sp, args.divisions);
		break;
	case CreateSphereArgs::SPHERIFIED_CUBE:
		m = createSphereSpherifiedCube<MeshType>(sp, args.divisions);
		break;
	case CreateSphereArgs::ICOSAHEDRON:
		m = createSphereIcosahedron<MeshType>(sp, args.divisions);
		break;
	}

	return m;
}

/**
 * @brief Creates a Sphere Mesh using the UV mode (https://github.com/caosdoar/spheres), centered in
 * (0, 0, 0), having radius 1, with 10 parallels and 20 meridias.
 * @return
 */
template<FaceMeshConcept MeshType>
MeshType createSphere()
{
	return createSphere<MeshType, double>({vcl::Point3d(), 1});
}

} // namespace vcl
