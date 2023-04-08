#ifndef SPACE_H
#define SPACE_H

#include <vclib/space.h>

void spaceStaticAsserts()
{
	using namespace vcl;

	// box2
	static_assert(Box2Concept<Box2i>, "Box2i does not satisfy the Box2Concept");
	static_assert(Box2Concept<Box2f>, "Box2f does not satisfy the Box2Concept");
	static_assert(Box2Concept<Box2d>, "Box2d does not satisfy the Box2Concept");

	// box3
	static_assert(Box3Concept<Box3i>, "Box3i does not satisfy the Box3Concept");
	static_assert(Box3Concept<Box3f>, "Box3f does not satisfy the Box3Concept");
	static_assert(Box3Concept<Box3d>, "Box3d does not satisfy the Box3Concept");

	// plane
	static_assert(PlaneConcept<Planef>, "Planef does not satisfy the PlaneConcept");
	static_assert(PlaneConcept<Planed>, "Planed does not satisfy the PlaneConcept");

	// point2
	static_assert(Point2Concept<Point2i>, "Point2i does not satisfy the Point2Concept");
	static_assert(Point2Concept<Point2f>, "Point2f does not satisfy the Point2Concept");
	static_assert(Point2Concept<Point2d>, "Point2d does not satisfy the Point2Concept");

	// point3
	static_assert(Point3Concept<Point3i>, "Point3i does not satisfy the Point3Concept");
	static_assert(Point3Concept<Point3f>, "Point3f does not satisfy the Point3Concept");
	static_assert(Point3Concept<Point3d>, "Point3d does not satisfy the Point3Concept");

	static_assert(Point4Concept<Point4i>, "Point4i does not satisfy the Point4Concept");
	static_assert(Point4Concept<Point4f>, "Point4f does not satisfy the Point4Concept");
	static_assert(Point4Concept<Point4d>, "Point4d does not satisfy the Point4Concept");

	static_assert(Polygon2Concept<Polygon2f>, "Polygon2f does not satisfy the Polygon2Concept");
	static_assert(Polygon2Concept<Polygon2d>, "Polygon2d does not satisfy the Polygon2Concept");

	static_assert(Polygon3Concept<Polygon3f>, "Polygon3f does not satisfy the Polygon3Concept");
	static_assert(Polygon3Concept<Polygon3d>, "Polygon3d does not satisfy the Polygon3Concept");

	static_assert(
		FaceSamplerConcept<MeshSampler<internal::TMPSimplePolyMesh>>,
		"MeshSampler is not a valid FaceSampler");

	static_assert(FaceSamplerConcept<PointSampler<>>, "PointSampler is not a valid FaceSampler");

	static_assert(
		SamplerConcept<VertexSampler<internal::TMPSimplePolyMesh::Vertex>>,
		"VertexSampler does not satisfy the SamplerConcept");
	static_assert(
		SamplerConcept<ConstVertexSampler<internal::TMPSimplePolyMesh::Vertex>>,
		"ConstVertexSampler does not satisfy the SamplerConcept");
//	static_assert(
//		std::ranges::range<VertexSampler<internal::TMPSimplePolyMesh::Vertex>>, "");

//	VertexSampler<internal::TMPSimplePolyMesh::Vertex> v;
//	auto it = std::ranges::begin(v);

	static_assert(SphereConcept<Spheref>, "Spheref does not satisfy the SphereConcept");
	static_assert(SphereConcept<Sphered>, "Sphered does not satisfy the SphereConcept");

	static_assert(Triangle2Concept<Triangle2f>, "Triangle2f does not satisfy the Triangle2Concept");
	static_assert(Triangle2Concept<Triangle2d>, "Triangle2d does not satisfy the Triangle2Concept");
	static_assert(
		ConstTriangle2Concept<Triangle2f>, "Triangle2f does not satisfy the ConstTriangle2Concept");
	static_assert(
		ConstTriangle2Concept<Triangle2d>, "Triangle2d does not satisfy the ConstTriangle2Concept");

	static_assert(Triangle3Concept<Triangle3f>, "Triangle3f does not satisfy the Triangle3Concept");
	static_assert(Triangle3Concept<Triangle3d>, "Triangle3d does not satisfy the Triangle3Concept");
	static_assert(
		ConstTriangle3Concept<Triangle3f>, "Triangle3f does not satisfy the ConstTriangle3Concept");
	static_assert(
		ConstTriangle3Concept<Triangle3d>, "Triangle3d does not satisfy the ConstTriangle3Concept");

	static_assert(
		ConstTriangle2Concept<TriangleWrapper2f>,
		"TriangleWrapper2f does not satisfy the ConstTriangle2Concept");
	static_assert(
		ConstTriangle2Concept<TriangleWrapper2d>,
		"TriangleWrapper2d does not satisfy the ConstTriangle2Concept");

	static_assert(
		ConstTriangle3Concept<Triangle3f>,
		"Triangle3f does not satisfy the ConstTriangle3Concept");
	static_assert(
		ConstTriangle3Concept<Triangle3d>,
		"Triangle3d does not satisfy the ConstTriangle3Concept");
}

#endif // SPACE_H
