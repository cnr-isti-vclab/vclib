/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_SPACE_COMPLEX_KD_TREE_H
#define VCL_SPACE_COMPLEX_KD_TREE_H

#include <numeric>
#include <queue>
#include <vector>

#include <vclib/concepts/mesh.h>
#include <vclib/space/core/box.h>

namespace vcl {

template<PointConcept PointType>
class KDTree
{
    using Scalar = PointType::ScalarType;

    struct Node
    {
        union
        {
            // standard node
            struct
            {
                Scalar splitValue;
                uint   firstChildId : 24;
                uint   dim          : 2;
                uint   leaf         : 1;
            };

            // leaf
            struct
            {
                uint           start;
                unsigned short size;
            };
        };
    };

    struct QueryNode
    {
        QueryNode() {}

        QueryNode(uint id) : nodeId(id) {}

        uint   nodeId; // id of the next node
        Scalar sq;     // squared distance to the next node
    };

    // dummy values
    inline static Scalar              dummyScalar;
    inline static std::vector<Scalar> dummyScalars;

    std::vector<PointType> mPoints;
    std::vector<uint>      mIndices;
    std::vector<Node>      mNodes;

    uint mPointsPerCell = 16; // min number of point in a leaf
    uint mMaxDepth      = 64; // max tree depth
    uint mDepth         = 0;  // actual tree depth

public:
    KDTree() {}

    KDTree(
        const std::vector<PointType>& points,
        uint                          pointsPerCell = 16,
        uint                          maxDepth      = 64,
        bool                          balanced      = false) :
            mPoints(points), mIndices(points.size()),
            mPointsPerCell(pointsPerCell), mMaxDepth(maxDepth)
    {
        std::iota(std::begin(mIndices), std::end(mIndices), 0);
        mNodes.resize(1);
        mNodes.back().leaf = 0;

        mDepth = createTree(0, 0, points.size(), 1, balanced);
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
    template<MeshConcept MeshType>
    KDTree(
        const MeshType& m,
        uint            pointsPerCell = 16,
        uint            maxDepth      = 64,
        bool            balanced      = false)
        requires (std::is_same_v<
                     typename MeshType::VertexType::CoordType,
                     PointType>)
            :
            mPoints(m.vertexNumber()), mIndices(m.vertexNumber()),
            mPointsPerCell(pointsPerCell), mMaxDepth(maxDepth)
    {
        using VertexType = MeshType::VertexType;

        uint i = 0;
        for (const VertexType& v : m.vertices()) {
            mPoints[i]  = v.coord();
            mIndices[i] = m.index(v);
            i++;
        }
        mNodes.resize(1);
        mNodes.back().leaf = 0;

        mDepth = createTree(0, 0, mPoints.size(), 1, balanced);
    }

    /**
     * @brief Searchs the closest point.
     *
     * The result of the query, the closest point to the query point, is the
     * index of the point and and the distance from the query point.
     */
    uint nearestNeighborIndex(
        const PointType& queryPoint,
        Scalar&          dist = dummyScalar) const
    {
        std::vector<QueryNode> mNodeStack(mDepth + 1);
        mNodeStack[0].nodeId = 0;
        mNodeStack[0].sq     = 0.;
        unsigned int count   = 1;

        int    minIndex = mIndices.size() / 2;
        Scalar minDist  = queryPoint.squaredDist(mPoints[minIndex]);
        minIndex        = mIndices[minIndex];

        while (count) {
            QueryNode&  qnode = mNodeStack[count - 1];
            const Node& node  = mNodes[qnode.nodeId];

            if (qnode.sq < minDist) {
                if (node.leaf) {
                    --count; // pop
                    uint end = node.start + node.size;
                    for (uint i = node.start; i < end; ++i) {
                        Scalar pointSquareDist =
                            queryPoint.squaredDist(mPoints[i]);
                        if (pointSquareDist < minDist) {
                            minDist  = pointSquareDist;
                            minIndex = mIndices[i];
                        }
                    }
                }
                else {
                    // replace the stack top by the farthest and push the
                    // closest
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

    PointType nearestNeighbor(
        const PointType& queryPoint,
        Scalar&          dist = dummyScalar) const
    {
        return mPoints[nearestNeighborIndex(queryPoint, dist)];
    }

    /**
     * @brief Performs the k nearest neighbour query.
     *
     * This algorithm uses the simple distance to the split plane to prune
     * nodes. A more elaborated approach consists to track the closest corner of
     * the cell relatively to the current query point. This strategy allows to
     * save about 5% of the leaves. However, in practice the slight overhead due
     * to this tracking reduces the overall performance.
     *
     * This algorithm also use a simple stack while a priority queue using the
     * squared distances to the cells as a priority values allows to save about
     * 10% of the leaves. But, again, priority queue insertions and deletions
     * are quite involved, and therefore a simple stack is by far much faster.
     *
     * The result of the query, the k-nearest neighbors, are stored into a
     * vector, and they are sorted on order of neighbohood.
     */
    std::vector<uint> kNearestNeighborsIndices(
        const PointType&     queryPoint,
        uint                 k,
        std::vector<Scalar>& distances = dummyScalars) const
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

        Comparator cmp(queryPoint);

        std::priority_queue<P, std::vector<P>, Comparator> neighborQueue(cmp);

        std::vector<QueryNode> mNodeStack(mDepth + 1);
        mNodeStack[0].nodeId = 0;
        mNodeStack[0].sq     = 0.;
        unsigned int count   = 1;

        while (count) {
            // we select the last node (AABB) inserted in the stack
            QueryNode& qnode = mNodeStack[count - 1];

            // while going down the tree qnode.nodeId is the nearest sub-tree,
            // otherwise, in backtracking, qnode.nodeId is the other sub-tree
            // that will be visited iff the actual nearest node is further than
            // the split distance.
            const Node& node = mNodes[qnode.nodeId];

            // if the distance is less than the top of the max-heap, it could be
            // one of the k-nearest neighbours
            if (neighborQueue.size() < k ||
                qnode.sq < queryPoint.squaredDist(neighborQueue.top().p)) {
                // when we arrive to a leaf
                if (node.leaf) {
                    --count; // pop of the leaf

                    // end is the index of the last element of the leaf in
                    // mPoints
                    unsigned int end = node.start + node.size;
                    // adding the element of the leaf to the heap
                    for (unsigned int i = node.start; i < end; ++i)
                        neighborQueue.push(P(mPoints[i], mIndices[i]));
                }
                // otherwise, if we're not on a leaf
                else {
                    // the new offset is the distance between the searched point
                    // and the actual split coordinate
                    Scalar new_off = queryPoint[node.dim] - node.splitValue;

                    // left sub-tree
                    if (new_off < 0.) {
                        mNodeStack[count].nodeId = node.firstChildId;
                        // in the father's nodeId we save the index of the other
                        // sub-tree (for backtracking)
                        qnode.nodeId = node.firstChildId + 1;
                    }
                    // right sub-tree (same as above)
                    else {
                        mNodeStack[count].nodeId = node.firstChildId + 1;
                        qnode.nodeId             = node.firstChildId;
                    }
                    // distance is inherited from the father (while descending
                    // the tree it's equal to 0)
                    mNodeStack[count].sq = qnode.sq;
                    // distance of the father is the squared distance from the
                    // split plane
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

        uint i = 0;
        while (!neighborQueue.empty() && i < k) {
            res.push_back(neighborQueue.top().i);
            distances.push_back(queryPoint.dist(neighborQueue.top().p));
            neighborQueue.pop();
            i++;
        }
        return res;
    }

    std::vector<PointType> kNearestNeighbors(
        const PointType&     queryPoint,
        uint                 k,
        std::vector<Scalar>& distances = dummyScalars) const
    {
        std::vector<uint> dists =
            kNearestNeighborsIndices(queryPoint, k, distances);
        std::vector<PointType> res;
        res.reserve(dists.size());
        for (uint k : dists) {
            res.push_back(mPoints[k]);
        }
        return res;
    }

    /**
     * @brief Performs the distance query.
     *
     * The result of the query, all the points within the distance dist form the
     * query point, is the vector of the indeces and the vector of the distances
     * from the query point.
     */
    std::vector<uint> neighborsIndicesInDistance(
        const PointType&     queryPoint,
        Scalar               dist,
        std::vector<Scalar>& distances = dummyScalars) const
    {
        std::vector<uint> queryPoints;
        distances.clear();
        std::vector<QueryNode> mNodeStack(mDepth + 1);
        mNodeStack[0].nodeId = 0;
        mNodeStack[0].sq     = 0.;
        unsigned int count   = 1;

        Scalar squareDist = dist * dist;
        while (count) {
            QueryNode&  qnode = mNodeStack[count - 1];
            const Node& node  = mNodes[qnode.nodeId];

            if (qnode.sq < squareDist) {
                if (node.leaf) {
                    --count; // pop
                    unsigned int end = node.start + node.size;
                    for (unsigned int i = node.start; i < end; ++i) {
                        Scalar pointSquareDist =
                            queryPoint.squareDist(mPoints[i]);
                        if (pointSquareDist < squareDist) {
                            queryPoints.push_back(mIndices[i]);
                            distances.push_back(queryPoint.dist(mPoints[i]));
                        }
                    }
                }
                else {
                    // replace the stack top by the farthest and push the
                    // closest
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

    std::vector<PointType> neighborsInDistance(
        const PointType&     queryPoint,
        Scalar               dist,
        std::vector<Scalar>& distances = dummyScalars) const
    {
        std::vector<uint> dists =
            neighborsIndicesInDistance(queryPoint, dist, distances);
        std::vector<PointType> res;
        res.reserve(dists.size());
        for (uint k : dists) {
            res.push_back(mPoints[k]);
        }
        return res;
    }

private:
    /**
     * @brief Rrecursively builds the kdtree
     *
     * The heuristic is the following:
     *  - if the number of points in the node is lower than targetCellsize then
     *    make a leaf
     *  - else compute the AABB of the points of the node and split it at the
     *    middle of the largest AABB dimension.
     *
     * This strategy might look not optimal because it does not explicitly prune
     * empty space, unlike more advanced SAH-like techniques used for RT. On the
     * other hand it leads to a shorter tree, faster to traverse and our
     * experience shown that in the special case of kNN queries, this strategy
     * is indeed more efficient (and much faster to build). Moreover, for volume
     * data (e.g., fluid simulation) pruning the empty space is useless.
     *
     * Actually, storing at each node the exact AABB (we therefore have a binary
     * BVH) allows to prune only about 10% of the leaves, but the overhead of
     * this pruning (ball/ABBB intersection) is more expensive than the gain it
     * provides and the memory consumption is x4 higher!
     */
    uint createTree(
        uint nodeId,
        uint start,
        uint end,
        uint level,
        bool balanced)
    {
        // select the first node
        Node&          node = mNodes[nodeId];
        Box<PointType> aabb;

        // putting all the points in the bounding box
        aabb.add(mPoints[start]);
        for (uint i = start + 1; i < end; ++i)
            aabb.add(mPoints[i]);

        // bounding box diagonal
        PointType diag = aabb.max() - aabb.min();

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
        // we divide the points using the median value along the "dim" dimension
        if (balanced) {
            std::vector<Scalar> tempVector;
            for (uint i = start + 1; i < end; ++i)
                tempVector.push_back(mPoints[i][dim]);
            std::sort(tempVector.begin(), tempVector.end());
            node.splitValue = (tempVector[tempVector.size() / 2.0] +
                               tempVector[tempVector.size() / 2.0 + 1]) /
                              2.0;
        }
        // we divide the bounding box in 2 partitions, considering the average
        // of the "dim" dimension
        else {
            node.splitValue = Scalar(0.5 * (aabb.max()[dim] + aabb.min()[dim]));
        }

        // midId is the index of the first element in the second partition
        unsigned int midId = split(start, end, dim, node.splitValue);

        node.firstChildId = mNodes.size();
        mNodes.resize(mNodes.size() + 2);
        bool flag = (midId == start) || (midId == end);
        int  leftLevel, rightLevel;

        // left child
        unsigned int childId = mNodes[nodeId].firstChildId;
        Node&        childL  = mNodes[childId];
        if (flag || (midId - start) <= mPointsPerCell || level >= mMaxDepth) {
            childL.leaf  = 1;
            childL.start = start;
            childL.size  = midId - start;
            leftLevel    = level;
        }
        else {
            childL.leaf = 0;
            leftLevel = createTree(childId, start, midId, level + 1, balanced);
        }

        // right child
        childId      = mNodes[nodeId].firstChildId + 1;
        Node& childR = mNodes[childId];
        if (flag || (end - midId) <= mPointsPerCell || level >= mMaxDepth) {
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
     * @brief Split the subarray between start and end in two part, one with the
     * elements less than splitValue, the other with the elements greater or
     * equal than splitValue. The elements are compared using the "dim"
     * coordinate [0 = x, 1 = y, 2 = z].
     */
    uint split(uint start, uint end, uint dim, Scalar splitValue)
    {
        uint l, r;
        for (l = start, r = end - 1; l < r; ++l, --r) {
            while (l < end && mPoints[l][dim] < splitValue)
                l++;
            while (r >= start && mPoints[r][dim] >= splitValue)
                r--;
            if (l > r)
                break;
            std::swap(mPoints[l], mPoints[r]);
            std::swap(mIndices[l], mIndices[r]);
        }

        // returns the index of the first element on the second part
        return (mPoints[l][dim] < splitValue ? l + 1 : l);
    }
};

/* Deduction guides */

template<MeshConcept MeshType>
KDTree(const MeshType& m) -> KDTree<typename MeshType::VertexType::CoordType>;

template<MeshConcept MeshType>
KDTree(const MeshType& m, uint pointsPerCell)
    -> KDTree<typename MeshType::VertexType::CoordType>;

template<MeshConcept MeshType>
KDTree(const MeshType& m, uint pointsPerCell, uint maxDepth)
    -> KDTree<typename MeshType::VertexType::CoordType>;

template<MeshConcept MeshType>
KDTree(const MeshType& m, uint pointsPerCell, uint maxDepth, bool balanced)
    -> KDTree<typename MeshType::VertexType::CoordType>;

} // namespace vcl

#endif // VCL_SPACE_COMPLEX_KD_TREE_H
