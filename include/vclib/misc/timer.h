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

#ifndef VCL_TIMER_H
#define VCL_TIMER_H

#include <chrono>
#include <iostream>
#include <string>
#include <time.h>

namespace vcl {

/**
 * @brief The Timer class allows to instantiate simple Timer objects that can be used everywhere.
 *
 * When a Timer object is create, by default the timer starts (you can change this option using the
 * bool argument of the Timer constructor). To each timer can be associated a caption, and the
 * printed/returned values are expressed in seconds.
 */
class Timer
{
public:
	Timer(bool _start = true);
	Timer(const char* caption, bool _start = true);
	Timer(const std::string& caption, bool _start = true);

	void start();
	void stopAndPrint();
	void stop();
	void print() const;
	double delay() const;

private:
	std::string caption;
	bool        isStopped;

	std::chrono::high_resolution_clock::time_point begin, end;
};

} // namespace vcl

#include "timer.cpp"

#endif // VCL_TIMER_H
