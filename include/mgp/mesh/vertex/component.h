/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_VERTEX_COMPONENT_H
#define MGP_MESH_VERTEX_COMPONENT_H

#include <iostream>

#include "color.h"
#include "normal.h"
#include "position.h"

namespace mgp {
namespace vert {

template<class T>
class Component : public T
{
};

template<class Scalar, int N>
class Component< vert::Position<Scalar, N> > : public vert::Position<Scalar, N>
{
};

template<class Scalar, int N>
class Component< vert::Normal<Scalar, N> > : public vert::Normal<Scalar, N>
{
};

template<>
class Component< vert::Color > : public vert::Color
{
};

}

}

#endif // MGP_MESH_VERTEX_COMPONENT_H
