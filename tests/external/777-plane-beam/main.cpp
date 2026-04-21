/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#include <vclib/embree/scene.h>
#include <vclib/algorithms/core/fibonacci.h>
#include <vclib/base/parallel.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

vcl::Point3d runPlaneBeam(
	vcl::PolyMesh          m,
	const std::vector<double>& gridCellSideLengths,
	vcl::uint              NUM_PLANES,
	bool                   debug)
{
	using namespace vcl;

	struct GridChoice
	{
		uint rows = 1;
		uint cols = 1;
		double sideU = 0.0;
		double sideV = 0.0;
	};

	updateBoundingBox(m);

	auto chooseGrid = [&](double lenU, double lenV) -> GridChoice {
		if (lenU <= 0.0 || lenV <= 0.0) {
			return {1, 1, lenU, lenV};
		}

		const double sideU =
			(gridCellSideLengths.size() >= 1) ? gridCellSideLengths[0] : lenU;
		const double sideV =
			(gridCellSideLengths.size() >= 2) ? gridCellSideLengths[1] : sideU;

		if (sideU <= 0.0 || sideV <= 0.0) {
			return {1, 1, lenU, lenV};
		}

		const uint cols = static_cast<uint>(std::max(1.0, std::ceil(lenU / sideU)));
		const uint rows = static_cast<uint>(std::max(1.0, std::ceil(lenV / sideV)));

		return {rows, cols, sideU, sideV};
	};

	const double EPS = 1e-6 * m.boundingBox().diagonal();

	// Ray tracing: shoot rays from grid cell centers through the mesh.
	embree::Scene scene(m);

	std::vector<std::vector<uint>> faceTriangulations;
	for (const auto& face : m.faces()) {
		const uint faceId = face.index();
		if (faceId >= faceTriangulations.size()) {
			faceTriangulations.resize(faceId + 1);
		}
		faceTriangulations[faceId] = earCut(face);
	}

	auto addSegment = [](EdgeMesh& em, const Point3d& a, const Point3d& b) {
		const uint va = em.addVertex(a);
		const uint vb = em.addVertex(b);
		em.addEdge(va, vb);
	};

	auto addQuadPrism = [](TriMesh& tm,
							const std::array<Point3d, 4>& baseCorners,
						double startOffset,
						double endOffset,
						const Point3d& dir) {
		std::array<Point3d, 4> b;
		std::array<Point3d, 4> t;
		for (uint k = 0; k < 4; ++k) {
			b[k] = baseCorners[k] + dir * startOffset;
			t[k] = baseCorners[k] + dir * endOffset;
		}

		std::array<uint, 8> ids;
		for (uint k = 0; k < 4; ++k) {
			ids[k + 0] = tm.addVertex(b[k]);
			ids[k + 4] = tm.addVertex(t[k]);
		}

		// Bottom
		tm.addFace(ids[0], ids[2], ids[1]);
		tm.addFace(ids[0], ids[3], ids[2]);
		tm.addFace(ids[4], ids[5], ids[6]);
		tm.addFace(ids[4], ids[6], ids[7]);

		// Sides
		tm.addFace(ids[0], ids[1], ids[5]);
		tm.addFace(ids[0], ids[5], ids[4]);
		tm.addFace(ids[1], ids[2], ids[6]);
		tm.addFace(ids[1], ids[6], ids[5]);
		tm.addFace(ids[2], ids[3], ids[7]);
		tm.addFace(ids[2], ids[7], ids[6]);
		tm.addFace(ids[3], ids[0], ids[4]);
		tm.addFace(ids[3], ids[4], ids[7]);
	};

	struct HitEvent
	{
		Point3d point;
		double  t = 0.0;
		Point3d normal = Point3d(0, 0, 0);
	};

	struct PlaneEvalStats
	{
		double minU  = 0.0;
		double minV  = 0.0;
		double maxU  = 0.0;
		double maxV  = 0.0;
		uint   rows  = 0;
		uint   cols  = 0;
		uint   cells = 0;
	};

	auto evaluatePlane = [&](const Point3d& rawPlaneNormal,
							bool           collectDebug,
							EdgeMesh*      outRayhitMesh,
							TriMesh*       outPrismsMesh,
							EdgeMesh*      outProjectedPointsMesh,
							EdgeMesh*      outBbox2dMesh,
							EdgeMesh*      outGrid2dMesh,
							PlaneEvalStats* outStats) {
		const bool collectDebugEnabled = debug && collectDebug;

		Point3d n = rawPlaneNormal;
		if (n.norm() <= EPS) {
			return std::numeric_limits<double>::infinity();
		}
		n.normalize();

		double minProj = std::numeric_limits<double>::infinity();
		for (const auto& vv : m.vertices()) {
			minProj = std::min(minProj, vv.position().dot(n));
		}
		const Point3d planePoint = n * minProj;
		const Planed  plane(planePoint, n);

		Point3d u, v;
		n.orthoBase(u, v);
		if (u.norm() <= EPS || v.norm() <= EPS) {
			return std::numeric_limits<double>::infinity();
		}
		u.normalize();
		v.normalize();

		double minU = std::numeric_limits<double>::infinity();
		double minV = std::numeric_limits<double>::infinity();
		double maxU = -std::numeric_limits<double>::infinity();
		double maxV = -std::numeric_limits<double>::infinity();

		std::vector<Point3d> projectedPoints;
		if (collectDebugEnabled && outProjectedPointsMesh) {
			projectedPoints.reserve(
				std::distance(m.vertices().begin(), m.vertices().end()));
		}

		for (const auto& vert : m.vertices()) {
			const Point3d projected = plane.projectPoint(vert.position());
			if (collectDebugEnabled && outProjectedPointsMesh) {
				projectedPoints.push_back(projected);
			}
			const Point3d rel = projected - planePoint;

			const double pu = rel.dot(u);
			const double pv = rel.dot(v);

			minU = std::min(minU, pu);
			minV = std::min(minV, pv);
			maxU = std::max(maxU, pu);
			maxV = std::max(maxV, pv);
		}

		const double lenU = maxU - minU;
		const double lenV = maxV - minV;
		if (lenU <= EPS || lenV <= EPS) {
			return std::numeric_limits<double>::infinity();
		}

		const GridChoice grid = chooseGrid(lenU, lenV);
		if (outStats) {
			outStats->minU  = minU;
			outStats->minV  = minV;
			outStats->maxU  = maxU;
			outStats->maxV  = maxV;
			outStats->rows  = grid.rows;
			outStats->cols  = grid.cols;
			outStats->cells = grid.rows * grid.cols;
		}

		const double gridMaxU = minU + grid.sideU * grid.cols;
		const double gridMaxV = minV + grid.sideV * grid.rows;

		const std::array<Point3d, 4> bboxCorners = {
			planePoint + u * minU + v * minV,
			planePoint + u * gridMaxU + v * minV,
			planePoint + u * gridMaxU + v * gridMaxV,
			planePoint + u * minU + v * gridMaxV};

		if (collectDebugEnabled && outProjectedPointsMesh) {
			outProjectedPointsMesh->reserveVertices(projectedPoints.size());
			for (const Point3d& p : projectedPoints) {
				outProjectedPointsMesh->addVertex(p);
			}
		}

		if (collectDebugEnabled && outBbox2dMesh) {
			std::array<uint, 4> cornerIds;
			for (uint k = 0; k < bboxCorners.size(); ++k) {
				cornerIds[k] = outBbox2dMesh->addVertex(bboxCorners[k]);
			}
			outBbox2dMesh->addEdge(cornerIds[0], cornerIds[1]);
			outBbox2dMesh->addEdge(cornerIds[1], cornerIds[2]);
			outBbox2dMesh->addEdge(cornerIds[2], cornerIds[3]);
			outBbox2dMesh->addEdge(cornerIds[3], cornerIds[0]);
		}

		if (collectDebugEnabled && outGrid2dMesh) {
			for (uint ii = 0; ii <= grid.cols; ++ii) {
				const double cu = minU + grid.sideU * ii;
				const uint a =
					outGrid2dMesh->addVertex(planePoint + u * cu + v * minV);
				const uint b =
					outGrid2dMesh->addVertex(planePoint + u * cu + v * gridMaxV);
				outGrid2dMesh->addEdge(a, b);
			}
			for (uint jj = 0; jj <= grid.rows; ++jj) {
				const double cv = minV + grid.sideV * jj;
				const uint a =
					outGrid2dMesh->addVertex(planePoint + u * minU + v * cv);
				const uint b =
					outGrid2dMesh->addVertex(planePoint + u * gridMaxU + v * cv);
				outGrid2dMesh->addEdge(a, b);
			}
		}

		auto collectHits = [&](const Point3d& rayOrigin) {
			std::vector<HitEvent> hitEvents;
			std::vector<embree::Scene::FilteredHitResult> hits =
				scene.collectFilteredHits(rayOrigin, n, static_cast<float>(EPS));
			hitEvents.reserve(hits.size());

			for (const auto& h : hits) {
				auto [hitFaceId, barCoords, hitTriId, tHit] = h;

				if (hitFaceId == UINT_NULL || hitFaceId >= faceTriangulations.size()) {
					continue;
				}

				const auto& face    = m.face(hitFaceId);
				const auto& hitTris = faceTriangulations[hitFaceId];
				const uint  base    = hitTriId * 3;
				if (base + 2 >= hitTris.size()) {
					continue;
				}

				const Point3d& q0 = face.vertex(hitTris[base + 0])->position();
				const Point3d& q1 = face.vertex(hitTris[base + 1])->position();
				const Point3d& q2 = face.vertex(hitTris[base + 2])->position();

				const Point3d hitPoint =
					q0 * barCoords.x() + q1 * barCoords.y() + q2 * barCoords.z();
				Point3d triNormal = (q1 - q0).cross(q2 - q0);
				if (triNormal.norm() >= EPS) {
					triNormal.normalize();
				}

				hitEvents.push_back({hitPoint, tHit, triNormal});
			}

			return hitEvents;
		};

		double totalVolume = 0.0;
		const double cellDu   = grid.sideU;
		const double cellDv   = grid.sideV;
		const double cellArea = cellDu * cellDv;

		auto computeCellGeometry = [&](uint i,
								 uint j,
								 Point3d& cellCenter,
								 std::array<Point3d, 4>& cellCorners) {
			const double u0 = minU + i * cellDu;
			const double u1 = u0 + cellDu;
			const double v0 = minV + j * cellDv;
			const double v1 = v0 + cellDv;

			cellCorners = {
				planePoint + u * u0 + v * v0,
				planePoint + u * u1 + v * v0,
				planePoint + u * u1 + v * v1,
				planePoint + u * u0 + v * v1};

			const double centerU = minU + (i + 0.5) * cellDu;
			const double centerV = minV + (j + 0.5) * cellDv;
			cellCenter           = planePoint + u * centerU + v * centerV;
		};

		auto accumulateSegment = [&](double& localVolume,
							 const Point3d& segStart,
								 const Point3d& segEnd,
								 const std::array<Point3d, 4>& cellCorners,
								 double startD,
						 		 double endD,
						 		 bool first = false) {
			const double segLength = endD - startD;
			const bool validLength = first || (segLength >= EPS);
			if (validLength) {
				const double segVolume = cellArea * segLength;
				localVolume += segVolume;
			}

			if (collectDebugEnabled && outRayhitMesh && validLength) {
				addSegment(*outRayhitMesh, segStart, segEnd);
			}
			if (collectDebugEnabled && outPrismsMesh && validLength) {
				addQuadPrism(*outPrismsMesh, cellCorners, startD, endD, n);
			}
		};

		auto processCell = [&](uint i, uint j, double& volumeAcc) {
			Point3d               cellCenter;
			std::array<Point3d, 4> cellCorners;
			computeCellGeometry(i, j, cellCenter, cellCorners);

			std::vector<HitEvent> hitEvents = collectHits(cellCenter);

			if (!hitEvents.empty()) {
				const Point3d segStart = cellCenter + n * -EPS;
				const Point3d segEnd   = hitEvents[0].point;
				const double  startD   = -EPS;
				const double  endD     = hitEvents[0].t;
				accumulateSegment(
					volumeAcc, segStart, segEnd, cellCorners, startD, endD, true);
			}

			int hitsMesh = 1;
			//std::cout << "Ray in cell (" << i << ", " << j << "): " << hitEvents.size() << " hits\n";
			for (int h = 0; h + 1 < (int)hitEvents.size(); h += 1) {
				const Point3d segStart = hitEvents[h].point;
				const Point3d segEnd   = hitEvents[h + 1].point;
				const double  startD   = hitEvents[h].t;
				const double  endD     = hitEvents[h + 1].t;
				if (endD <= startD) {
					continue;
				}

				const Point3d& startNormal = hitEvents[h].normal;
				const Point3d& endNormal   = hitEvents[h + 1].normal;
				const double  startDot = startNormal.dot(n);
				const double  endDot   = endNormal.dot(n);

				//std::cout << "  Hit " << h << ": normal = " << startNormal << ", StartDot = " << startDot << ", EndDot = " << endDot << ", HitMesh = " << hitsMesh << "\n";

				if (endDot < 0.0) {
					if ((startDot > 0.0) && (hitsMesh == 0)) {
						accumulateSegment(
							volumeAcc, segStart, segEnd, cellCorners, startD, endD);
					}
					hitsMesh += 1;
				}
				else {
					hitsMesh -= 1;
				}
			}
		};

		if (!collectDebugEnabled) {
			std::vector<uint> rowIndices(grid.rows);
			for (uint j = 0; j < grid.rows; ++j) {
				rowIndices[j] = j;
			}

			std::vector<double> rowVolumes(grid.rows, 0.0);
			vcl::parallelFor(rowIndices, [&](uint j) {
				double localVolume = 0.0;
				for (uint i = 0; i < grid.cols; ++i) {
					processCell(i, j, localVolume);
				}
				rowVolumes[j] = localVolume;
			});

			for (double vRow : rowVolumes) {
				totalVolume += vRow;
			}
		}
		else {
			for (uint j = 0; j < grid.rows; ++j) {
				for (uint i = 0; i < grid.cols; ++i) {
					processCell(i, j, totalVolume);
				}
			}
		}

		return totalVolume;
	};

	std::vector<Point3d> fibNormals = sphericalFibonacciPointSet<Point3d>(NUM_PLANES);
	if (fibNormals.empty()) {
		std::cerr << "No Fibonacci planes generated.\n";
		return Point3d(0, 0, 0);
	}

	uint    bestPlaneId = 0;
	double  bestVolume  = std::numeric_limits<double>::infinity();
	Point3d bestNormal  = fibNormals.front();

	for (uint i = 0; i < fibNormals.size(); ++i) {
		PlaneEvalStats stats;
		double vol = evaluatePlane(
			fibNormals[i], false, nullptr, nullptr, nullptr, nullptr, nullptr, &stats);

		if (debug) {
			std::cout << "Plane id: " << i << "/" << (fibNormals.size() - 1)
					  << ", normal: " << fibNormals[i] << ", volume: " << vol
					  << ", bbox2d(u,v): [(" << stats.minU << ", " << stats.minV
					  << ") -> (" << stats.maxU << ", " << stats.maxV << ")]"
					  << ", cells: " << stats.cells << " (" << stats.cols << "x"
					  << stats.rows << ")";
		}
		if (vol < bestVolume) {
			bestVolume  = vol;
			bestPlaneId = i;
			bestNormal  = fibNormals[i];
			if (debug) {
				std::cout << " [new best]";
			}
		}
		if (debug) {
			std::cout << "\n";
		}
	}

	EdgeMesh bestRayhitMesh;
	TriMesh  bestPrismsMesh;
	EdgeMesh bestProjectedPointsMesh;
	EdgeMesh bestBbox2dMesh;
	EdgeMesh bestGrid2dMesh;

	bestVolume = evaluatePlane(
		bestNormal,
		true,
		&bestRayhitMesh,
		&bestPrismsMesh,
		&bestProjectedPointsMesh,
		&bestBbox2dMesh,
		&bestGrid2dMesh,
		nullptr);

	if (debug) {
		saveMesh(bestRayhitMesh, VCLIB_RESULTS_PATH "/777_plane_beam_rayhits.ply");
		saveMesh(bestPrismsMesh, VCLIB_RESULTS_PATH "/777_plane_beam_prisms.ply");
		saveMesh(bestProjectedPointsMesh, VCLIB_RESULTS_PATH "/777_plane_beam_projected_points.ply");
		saveMesh(bestBbox2dMesh, VCLIB_RESULTS_PATH "/777_plane_beam_bbox2d.ply");
		saveMesh(bestGrid2dMesh, VCLIB_RESULTS_PATH "/777_plane_beam_grid2d.ply");
	}

	auto saveDebugMeshes = [&](const std::string& suffix,
								 const Point3d&     normal) {
		EdgeMesh rayhitMesh;
		TriMesh  prismsMesh;
		EdgeMesh projectedPointsMesh;
		EdgeMesh bbox2dMesh;
		EdgeMesh grid2dMesh;

		evaluatePlane(
			normal,
			true,
			&rayhitMesh,
			&prismsMesh,
			&projectedPointsMesh,
			&bbox2dMesh,
			&grid2dMesh,
			nullptr);

		const std::string base = std::string(VCLIB_RESULTS_PATH) + "/777_plane_beam_" + suffix;
		saveMesh(rayhitMesh, base + "_rayhits.ply");
		saveMesh(prismsMesh, base + "_prisms.ply");
		saveMesh(projectedPointsMesh, base + "_projected_points.ply");
		saveMesh(bbox2dMesh, base + "_bbox2d.ply");
		saveMesh(grid2dMesh, base + "_grid2d.ply");
	};

	
	/*
	const Point3d plusY(0.0, 1.0, 0.0);
	const Point3d minusY(0.0, -1.0, 0.0);

	double plusYVolume = evaluatePlane(
		plusY, false, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
	double minusYVolume = evaluatePlane(
		minusY, false, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

	saveDebugMeshes("plus_y", plusY);
	saveDebugMeshes("minus_y", minusY);

	*/

	

	if (debug) {
		std::cout << "Fibonacci planes tested: " << fibNormals.size() << "\n"
				  << "Best plane id: " << bestPlaneId << "\n"
				  << "Best plane normal: " << bestNormal << "\n"
				  << "Grid cell side lengths (u, v): "
				  << ((gridCellSideLengths.size() >= 1) ? gridCellSideLengths[0] : 0.0)
				  << ", "
				  << ((gridCellSideLengths.size() >= 2) ? gridCellSideLengths[1] :
						 ((gridCellSideLengths.size() >= 1) ? gridCellSideLengths[0] : 0.0))
				  << "\n"
				  << "Minimum volume: " << bestVolume << "\n"
				  //<< "Special plane +Y volume: " << plusYVolume << "\n"
				  //<< "Special plane -Y volume: " << minusYVolume << "\n"
				  << "Saved debug meshes:\n"
				  << " - " << VCLIB_RESULTS_PATH << "/777_plane_beam_prisms.ply\n"
				  << " - " << VCLIB_RESULTS_PATH << "/777_plane_beam_projected_points.ply\n"
				  << " - " << VCLIB_RESULTS_PATH << "/777_plane_beam_bbox2d.ply\n"
				  << " - " << VCLIB_RESULTS_PATH << "/777_plane_beam_grid2d.ply\n"
				  << " - " << VCLIB_RESULTS_PATH << "/777_plane_beam_rayhits.ply\n"
				  /*<< " - " << VCLIB_RESULTS_PATH << "/777_plane_beam_plus_y_prisms.ply\n"
				  << " - " << VCLIB_RESULTS_PATH << "/777_plane_beam_plus_y_projected_points.ply\n"
				  << " - " << VCLIB_RESULTS_PATH << "/777_plane_beam_plus_y_bbox2d.ply\n"
				  << " - " << VCLIB_RESULTS_PATH << "/777_plane_beam_plus_y_grid2d.ply\n"
				  << " - " << VCLIB_RESULTS_PATH << "/777_plane_beam_plus_y_rayhits.ply\n"
				  << " - " << VCLIB_RESULTS_PATH << "/777_plane_beam_minus_y_prisms.ply\n"
				  << " - " << VCLIB_RESULTS_PATH << "/777_plane_beam_minus_y_projected_points.ply\n"
				  << " - " << VCLIB_RESULTS_PATH << "/777_plane_beam_minus_y_bbox2d.ply\n"
				  << " - " << VCLIB_RESULTS_PATH << "/777_plane_beam_minus_y_grid2d.ply\n"
				  << " - " << VCLIB_RESULTS_PATH << "/777_plane_beam_minus_y_rayhits.ply\n"*/;
	}

	return bestNormal;
}

int main()
{
	using namespace vcl;

	std::vector<double> gridCellSideLengths = {0.184, 0.234};
	constexpr uint NUM_PLANES = 2000;
	constexpr bool debug = true;

	PolyMesh m = loadMesh<PolyMesh>(VCLIB_EXAMPLE_MESHES_PATH "/brain_enlarged.ply");
	// PolyMesh m = loadMesh<PolyMesh>("C:\\Users\\Ougi\\Desktop\\delirium2.ply");

	const auto startTime = std::chrono::steady_clock::now();
	const Point3d bestNormal =
		runPlaneBeam(std::move(m), gridCellSideLengths, NUM_PLANES, debug);
	const auto endTime = std::chrono::steady_clock::now();
	const auto elapsedMs =
		std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime)
			.count();

	if (bestNormal.norm() == 0.0) {
		return 1;
	}

	std::cout << "Best plane normal: " << bestNormal << "\n";
	std::cout << "Tempo di esecuzione: " << elapsedMs << " ms\n";

	return 0;
}