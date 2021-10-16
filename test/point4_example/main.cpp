/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include <iostream>

#include <mgp/space/point.h>

int main()
{
	mgp::Point4d p1;
	p1 << 1, 0, 0, 0;

	mgp::Point4d p2(0, 1, 0, 0);

	std::cout << "Point p1 is [" << p1 << "].\n";
	std::cout << "Point p2 is [" << p2 << "].\n";

	std::cout << "p1 norm is " << p1.norm() << ".\n";
	std::cout << "p1 squared norm is " << p1.squaredNorm() << "\n";

	std::cout << "p1 dot p2: " << p1.dot(p2) << "\n";

	mgp::Point4f pf = p1;

	std::cout << "p1 converted to float: [" << pf << "]\n";

	return 0;
}
