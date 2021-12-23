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
	Box<PointType> b;
	uint i = 0;
	for (const PointType& p : points) {
		b.add(p);
		indices[i] = i;
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
 * and the squared distance from the query point.
 */
template<typename PointType>
uint KDTree<PointType>::closestPointIndex(const PointType& queryPoint, Scalar& dist) const
{
	std::vector<QueryNode> mNodeStack(depth + 1);
	mNodeStack[0].nodeId = 0;
	mNodeStack[0].sq = 0.;
	unsigned int count = 1;

	int minIndex = indices.size() / 2;
	Scalar minDist = (queryPoint - points[minIndex]).squaredNorm();
	minIndex = indices[minIndex];

	while (count) {
		QueryNode& qnode = mNodeStack[count - 1];
		const Node& node = nodes[qnode.nodeId];

		if (qnode.sq < minDist) {
			if (node.leaf) {
				--count; // pop
				uint end = node.start + node.size;
				for (uint i = node.start; i < end; ++i) {
					Scalar pointSquareDist = (queryPoint - points[i]).squaredNorm();
					if (pointSquareDist < minDist) {
						minDist = pointSquareDist;
						minIndex = indices[i];
					}
				}
			}
			else {
				// replace the stack top by the farthest and push the closest
				Scalar new_off = queryPoint[node.dim] - node.splitValue;
				if (new_off < 0.) {
					mNodeStack[count].nodeId = node.firstChildId;
					qnode.nodeId = node.firstChildId + 1;
				}
				else {
					mNodeStack[count].nodeId = node.firstChildId + 1;
					qnode.nodeId = node.firstChildId;
				}
				mNodeStack[count].sq = qnode.sq;
				qnode.sq = new_off*new_off;
				++count;
			}
		}
		else {
			// pop
			--count;
		}
	}
	dist = minDist;
	return minIndex;
}

template<typename PointType>
PointType KDTree<PointType>::closestPoint(const PointType& queryPoint, Scalar& dist) const
{
	return points[closestPointIndex(queryPoint, dist)];
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
 * unlike more advanced SAH-like techniques used for RT. On the other hand it leads to a shorter tree,
 * faster to traverse and our experience shown that in the special case of kNN queries,
 * this strategy is indeed more efficient (and much faster to build). Moreover, for volume data
 * (e.g., fluid simulation) pruning the empty space is useless.
 *
 * Actually, storing at each node the exact AABB (we therefore have a binary BVH) allows
 * to prune only about 10% of the leaves, but the overhead of this pruning (ball/ABBB intersection)
 * is more expensive than the gain it provides and the memory consumption is x4 higher !
 */
template<typename PointType>
uint KDTree<PointType>::createTree(uint nodeId, uint start, uint end, uint level, bool balanced)
{
	//select the first node
	Node& node = nodes[nodeId];
	Box<PointType> aabb;

	//putting all the points in the bounding box
	aabb.add(points[start]);
	for (uint i = start + 1; i < end; ++i)
		aabb.add(points[i]);

	//bounding box diagonal
	PointType diag = aabb.max - aabb.min;

	//the split "dim" is the dimension of the box with the biggest value
	uint dim = 0;
	Scalar max = std::numeric_limits<double>::lowest();
	for (uint i = 0; i < PointType::DIM; ++i){
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
		node.splitValue = Scalar(0.5*(aabb.max[dim] + aabb.min[dim]));
	}

	//midId is the index of the first element in the second partition
	unsigned int midId = split(start, end, dim, node.splitValue);

	node.firstChildId = nodes.size();
	nodes.resize(nodes.size() + 2);
	bool flag = (midId == start) || (midId == end);
	int leftLevel, rightLevel;

	// left child
	unsigned int childId = nodes[nodeId].firstChildId;
	Node& childL = nodes[childId];
	if (flag || (midId - start) <= pointsPerCell || level >= maxDepth) {
		childL.leaf = 1;
		childL.start = start;
		childL.size = midId - start;
		leftLevel = level;
	}
	else {
		childL.leaf = 0;
		leftLevel = createTree(childId, start, midId, level + 1, balanced);
	}

	// right child
	childId = nodes[nodeId].firstChildId + 1;
	Node& childR = nodes[childId];
	if (flag || (end - midId) <= pointsPerCell || level >= maxDepth) {
		childR.leaf = 1;
		childR.start = midId;
		childR.size = end - midId;
		rightLevel = level;
	}
	else {
		childR.leaf = 0;
		rightLevel = createTree(childId, midId, end, level + 1, balanced);
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
	for (l = start, r = end -1; l < r; ++l, --r) {
		while (l < end && points[l][dim] < splitValue)
			l++;
		while (r >= start && points[r][dim] >= splitValue)
			r--;
		if (l > r)
			break;
		std::swap(points[l], points[r]);
		std::swap(indices[l], indices[r]);
	}

	//returns the index of the first element on the second part
	return (points[l][dim] < splitValue ? l + 1 : l);
}

} // namespace vcl
