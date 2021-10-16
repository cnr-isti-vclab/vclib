/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include <iostream>

#include <mgp/mesh/vertex.h>
#include <mgp/space/color.h>

int main()
{
	mgp::Vertex<
		mgp::vert::Position3d,
		mgp::vert::Normal3d,
		mgp::vert::Color> v;

	std::cerr << "Coord : " << v.position() << "\n";
	std::cerr << "Normal: " << v.normal() << "\n";
	std::cerr << "Color : " << v.color() << "\n";

	return 0;
}
