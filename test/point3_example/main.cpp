/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include <iostream>

#include <mgp/geometry/point.h>

int main()
{
	mgp::Point3<double> p;
	p << 1, 3, 5;

	std::cout << "Point y is : " << p.y() << ".\n";

	p.y() = 2;
	std::cout << "Point is [" << p << "].\n";

	std::cout << "Norm is " << p.norm() << ".\n";
	std::cout << "Squared norm is " << p.squaredNorm() << "\n";
	return 0;
}
