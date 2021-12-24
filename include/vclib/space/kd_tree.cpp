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

#include "kd_tree.h"

#include <numeric>
#include <queue>
#include <vclib/mesh/requirements.h>

#include "box.h"

namespace vcl {

template<typename PointType>
KDTree<PointType>::KDTree()
{
}

template<typename PointType>
KDTree<PointType>::KDTree(
	const std::vector<PointType>& points,
	uint                          pointsPerCell,
	uint                          maxDepth,
	bool                          balanced) :
		points(points), indices(points.size()), pointsPerCell(pointsPerCell), maxDepth(maxDepth)
{
	std::iota(std::begin(indices), std::end(indices), 0);
	nodes.resize(1);
	nodes.back().leaf = 0;

	depth = createTree(0, 0, points.size(), 1, balanced);
}

/**
 * @brief Builds the KDTree starting from the given mesh.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *
 * @param m
 * @param pointsPerCell
 * @param maxDepth
 * @param balanced
 */
template<typename PointType>
template<typename MeshType>
KDTree<PointType>::KDTree(const MeshType& m, uint pointsPerCell, uint maxDepth, bool balanced) :
		points(m.vertexNumber()),
		indices(m.vertexNumber()),
		pointsPerCell(pointsPerCell),
		maxDepth(maxDepth)
{
	vcl::requireVertices<MeshType>();

	using VertexType = typename MeshType::VertexType;

	uint           i = 0;
	for (const VertexType& v : m.vertices()) {
		points[i] = v.coord();
		indices[i] = m.index(v);
		i++;
	}
	nodes.resize(1);
	nodes.back().leaf = 0;

	depth = createTree(0, 0, points.size(), 1, balanced);
}

/**
 * Searchs the closest point.
 *
 * The result of the query, the closest point to the query point, is the index of the point and
 * and the distance from the query point.
 */
template<typename PointType>
uint KDTree<PointType>::nearestNeighborIndex(const PointType& queryPoint, Scalar& dist) const
{
	std::vector<QueryNode> mNodeStack(depth + 1);
	mNodeStack[0].nodeId = 0;
	mNodeStack[0].sq     = 0.;
	unsigned int count   = 1;

	int    minIndex = indices.size() / 2;
	Scalar minDist  = queryPoint.squaredDist(points[minIndex]);
	minIndex        = indices[minIndex];

	while (count) {
		QueryNode&  qnode = mNodeStack[count - 1];
		const Node& node  = nodes[qnode.nodeId];

		if (qnode.sq < minDist) {
			if (node.leaf) {
				--count; // pop
				uint end = node.start + node.size;
				for (uint i = node.start; i < end; ++i) {
					Scalar pointSquareDist = queryPoint.squaredDist(points[i]);
					if (pointSquareDist < minDist) {
						minDist  = pointSquareDist;
						minIndex = indices[i];
					}
				}
			}
			else {
				// replace the stack top by the farthest and push the closest
				Scalar new_off = queryPoint[node.dim] - node.splitValue;
				if (new_off < 0.) {
					mNodeStack[count].nodeId = node.firstChildId;
					qnode.nodeId             = node.firstChildId + 1;
				}
				else {
					mNodeStack[count].nodeId = node.firstChildId + 1;
					qnode.nodeId             = node.firstChildId;
				}
				mNodeStack[count].sq = qnode.sq;
				qnode.sq             = new_off * new_off;
				++count;
			}
		}
		else {
			// pop
			--count;
		}
	}
	dist = std::sqrt(minDist);
	return minIndex;
}

template<typename PointType>
PointType KDTree<PointType>::nearestNeighbor(const PointType& queryPoint, Scalar& dist) const
{
	return points[nearestNeighborIndex(queryPoint, dist)];
}

/**
 * Performs the k nearest neighbour query.
 *
 * This algorithm uses the simple distance to the split plane to prune nodes.
 * A more elaborated approach consists to track the closest corner of the cell
 * relatively to the current query point. This strategy allows to save about 5%
 * of the leaves. However, in practice the slight overhead due to this tracking
 * reduces the overall performance.
 *
 * This algorithm also use a simple stack while a priority queue using the squared
 * distances to the cells as a priority values allows to save about 10% of the leaves.
 * But, again, priority queue insertions and deletions are quite involved, and therefore
 * a simple stack is by far much faster.
 *
 * The result of the query, the k-nearest neighbors, are stored into a vector, and they
 * are sorted on order of neighbohood.
 */
template<typename PointType>
std::vector<uint> KDTree<PointType>::kNearestNeighborsIndices(
	const PointType&     queryPoint,
	uint                 k,
	std::vector<Scalar>& distances) const
{
	struct P
	{
		P(const PointType& p, int i) : p(p), i(i) {}
		PointType p;
		uint      i;
	};

	struct Comparator
	{
		const PointType qp;
		Comparator(const PointType& qp) : qp(qp) {}
		bool operator()(const P& p1, const P& p2)
		{
			if (qp.squaredDist(p1.p) > qp.squaredDist(p2.p))
				return true;
			return false;
		}
	};

	Comparator                                         cmp(queryPoint);
	std::priority_queue<P, std::vector<P>, Comparator> neighborQueue(cmp);

	std::vector<QueryNode> mNodeStack(depth + 1);
	mNodeStack[0].nodeId = 0;
	mNodeStack[0].sq     = 0.;
	unsigned int count   = 1;

	while (count) {
		// we select the last node (AABB) inserted in the stack
		QueryNode& qnode = mNodeStack[count - 1];

		// while going down the tree qnode.nodeId is the nearest sub-tree, otherwise,
		// in backtracking, qnode.nodeId is the other sub-tree that will be visited iff
		// the actual nearest node is further than the split distance.
		const Node& node = nodes[qnode.nodeId];

		// if the distance is less than the top of the max-heap, it could be one of the k-nearest
		// neighbours
		if (neighborQueue.size() < k ||
			qnode.sq < queryPoint.squaredDist(neighborQueue.top().p)) {
			// when we arrive to a leaf
			if (node.leaf) {
				--count; // pop of the leaf

				// end is the index of the last element of the leaf in mPoints
				unsigned int end = node.start + node.size;
				// adding the element of the leaf to the heap
				for (unsigned int i = node.start; i < end; ++i)
					neighborQueue.push(P(points[i], indices[i]));
			}
			// otherwise, if we're not on a leaf
			else {
				// the new offset is the distance between the searched point and the actual split
				// coordinate
				Scalar new_off = queryPoint[node.dim] - node.splitValue;

				// left sub-tree
				if (new_off < 0.) {
					mNodeStack[count].nodeId = node.firstChildId;
					// in the father's nodeId we save the index of the other sub-tree (for
					// backtracking)
					qnode.nodeId = node.firstChildId + 1;
				}
				// right sub-tree (same as above)
				else {
					mNodeStack[count].nodeId = node.firstChildId + 1;
					qnode.nodeId             = node.firstChildId;
				}
				// distance is inherited from the father (while descending the tree it's equal to 0)
				mNodeStack[count].sq = qnode.sq;
				// distance of the father is the squared distance from the split plane
				qnode.sq = new_off * new_off;
				++count;
			}
		}
		else {
			// pop
			--count;
		}
	}
	distances.clear();
	std::vector<uint> res;
	uint              i = 0;
	while (!neighborQueue.empty() && i < k) {
		res.push_back(neighborQueue.top().i);
		distances.push_back(queryPoint.dist(neighborQueue.top().p));
		neighborQueue.pop();
		i++;
	}
	return res;
}

template<typename PointType>
std::vector<PointType> KDTree<PointType>::kNearestNeighbors(
	const PointType&     queryPoint,
	uint                 k,
	std::vector<Scalar>& distances) const
{
	std::vector<uint>      dists = kNearestNeighborsIndices(queryPoint, k, distances);
	std::vector<PointType> res;
	res.reserve(dists.size());
	for (uint k : dists) {
		res.push_back(points[k]);
	}
	return res;
}

/**
 * Performs the distance query.
 *
 * The result of the query, all the points within the distance dist form the query point, is the
 * vector of the indeces and the vector of the distances from the query point.
 */
template<typename PointType>
std::vector<uint> KDTree<PointType>::neighborsIndicesInDistance(
	const PointType&     queryPoint,
	Scalar               dist,
	std::vector<Scalar>& distances) const
{
	std::vector<uint> queryPoints;
	distances.clear();
	std::vector<QueryNode> mNodeStack(depth + 1);
	mNodeStack[0].nodeId = 0;
	mNodeStack[0].sq     = 0.;
	unsigned int count   = 1;

	Scalar squareDist = dist * dist;
	while (count) {
		QueryNode&  qnode = mNodeStack[count - 1];
		const Node& node  = nodes[qnode.nodeId];

		if (qnode.sq < squareDist) {
			if (node.leaf) {
				--count; // pop
				unsigned int end = node.start + node.size;
				for (unsigned int i = node.start; i < end; ++i) {
					Scalar pointSquareDist = queryPoint.squareDist(points[i]);
					if (pointSquareDist < squareDist) {
						queryPoints.push_back(indices[i]);
						distances.push_back(queryPoint.dist(points[i]));
					}
				}
			}
			else {
				// replace the stack top by the farthest and push the closest
				Scalar new_off = queryPoint[node.dim] - node.splitValue;
				if (new_off < 0.) {
					mNodeStack[count].nodeId = node.firstChildId;
					qnode.nodeId             = node.firstChildId + 1;
				}
				else {
					mNodeStack[count].nodeId = node.firstChildId + 1;
					qnode.nodeId             = node.firstChildId;
				}
				mNodeStack[count].sq = qnode.sq;
				qnode.sq             = new_off * new_off;
				++count;
			}
		}
		else {
			// pop
			--count;
		}
	}
	return queryPoints;
}

template<typename PointType>
std::vector<PointType> KDTree<PointType>::neighborsInDistance(
	const PointType&     queryPoint,
	Scalar               dist,
	std::vector<Scalar>& distances) const
{
	std::vector<uint>      dists = neighborsIndicesInDistance(queryPoint, dist, distances);
	std::vector<PointType> res;
	res.reserve(dists.size());
	for (uint k : dists) {
		res.push_back(points[k]);
	}
	return res;
}

/**
 * Rrecursively builds the kdtree
 *
 * The heuristic is the following:
 *  - if the number of points in the node is lower than targetCellsize then make a leaf
 *  - else compute the AABB of the points of the node and split it at the middle of
 *    the largest AABB dimension.
 *
 * This strategy might look not optimal because it does not explicitly prune empty space,
 * unlike more advanced SAH-like techniques used for RT. On the other hand it leads to a shorter
 * tree, faster to traverse and our experience shown that in the special case of kNN queries, this
 * strategy is indeed more efficient (and much faster to build). Moreover, for volume data (e.g.,
 * fluid simulation) pruning the empty space is useless.
 *
 * Actually, storing at each node the exact AABB (we therefore have a binary BVH) allows
 * to prune only about 10% of the leaves, but the overhead of this pruning (ball/ABBB intersection)
 * is more expensive than the gain it provides and the memory consumption is x4 higher !
 */
template<typename PointType>
uint KDTree<PointType>::createTree(uint nodeId, uint start, uint end, uint level, bool balanced)
{
	// select the first node
	Node&          node = nodes[nodeId];
	Box<PointType> aabb;

	// putting all the points in the bounding box
	aabb.add(points[start]);
	for (uint i = start + 1; i < end; ++i)
		aabb.add(points[i]);

	// bounding box diagonal
	PointType diag = aabb.max - aabb.min;

	// the split "dim" is the dimension of the box with the biggest value
	uint   dim = 0;
	Scalar max = std::numeric_limits<double>::lowest();
	for (uint i = 0; i < PointType::DIM; ++i) {
		if (diag[i] > max) {
			max = diag[i];
			dim = i;
		}
	}

	node.dim = dim;
	if (balanced) { // we divide the points using the median value along the "dim" dimension
		std::vector<Scalar> tempVector;
		for (uint i = start + 1; i < end; ++i)
			tempVector.push_back(points[i][dim]);
		std::sort(tempVector.begin(), tempVector.end());
		node.splitValue =
			(tempVector[tempVector.size() / 2.0] + tempVector[tempVector.size() / 2.0 + 1]) / 2.0;
	}
	// we divide the bounding box in 2 partitions, considering the average of the "dim" dimension
	else {
		node.splitValue = Scalar(0.5 * (aabb.max[dim] + aabb.min[dim]));
	}

	// midId is the index of the first element in the second partition
	unsigned int midId = split(start, end, dim, node.splitValue);

	node.firstChildId = nodes.size();
	nodes.resize(nodes.size() + 2);
	bool flag = (midId == start) || (midId == end);
	int  leftLevel, rightLevel;

	// left child
	unsigned int childId = nodes[nodeId].firstChildId;
	Node&        childL  = nodes[childId];
	if (flag || (midId - start) <= pointsPerCell || level >= maxDepth) {
		childL.leaf  = 1;
		childL.start = start;
		childL.size  = midId - start;
		leftLevel    = level;
	}
	else {
		childL.leaf = 0;
		leftLevel   = createTree(childId, start, midId, level + 1, balanced);
	}

	// right child
	childId      = nodes[nodeId].firstChildId + 1;
	Node& childR = nodes[childId];
	if (flag || (end - midId) <= pointsPerCell || level >= maxDepth) {
		childR.leaf  = 1;
		childR.start = midId;
		childR.size  = end - midId;
		rightLevel   = level;
	}
	else {
		childR.leaf = 0;
		rightLevel  = createTree(childId, midId, end, level + 1, balanced);
	}

	if (leftLevel > rightLevel)
		return leftLevel;
	return rightLevel;
}

/**
 * Split the subarray between start and end in two part, one with the elements less than splitValue,
 * the other with the elements greater or equal than splitValue. The elements are compared
 * using the "dim" coordinate [0 = x, 1 = y, 2 = z].
 */
template<typename PointType>
uint KDTree<PointType>::split(uint start, uint end, uint dim, Scalar splitValue)
{
	uint l, r;
	for (l = start, r = end - 1; l < r; ++l, --r) {
		while (l < end && points[l][dim] < splitValue)
			l++;
		while (r >= start && points[r][dim] >= splitValue)
			r--;
		if (l > r)
			break;
		std::swap(points[l], points[r]);
		std::swap(indices[l], indices[r]);
	}

	// returns the index of the first element on the second part
	return (points[l][dim] < splitValue ? l + 1 : l);
}

} // namespace vcl
