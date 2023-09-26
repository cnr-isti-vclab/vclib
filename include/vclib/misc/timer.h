/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
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

#ifndef VCL_MISC_TIMER_H
#define VCL_MISC_TIMER_H

#include <chrono>
#include <iostream>
#include <string>
#include <time.h>

namespace vcl {

/******************************************************************************
 *                                Declarations                                *
 ******************************************************************************/

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

/******************************************************************************
 *                                Definitions                                 *
 ******************************************************************************/

/**
 * @brief Creates a timer with the caption "Timer". If the given boolean is true, the timer starts.
 *
 * @param[in] _start: if true, the timer will start when the object is created.
 */
inline Timer::Timer(bool _start) : caption("Timer"), isStopped(false)
{
	if (_start)
		start();
}

/**
 * @brief Creates a timer with the given caption. If the given boolean is true, the timer starts.
 *
 * @param[in] caption: the caption of the timer.
 * @param[in] _start: if true, the timer will start when the object is created.
 */
inline Timer::Timer(const char* caption, bool _start) : caption(caption), isStopped(false)
{
	if (_start)
		start();
}

/**
 * @brief Creates a timer with the given caption. If the given boolean is true, the timer starts.
 *
 * @param[in] caption: the caption of the timer.
 * @param[in] _start: if true, the timer will start when the object is created.
 */
inline Timer::Timer(const std::string& caption, bool _start) : caption(caption), isStopped(false)
{
	if (_start)
		start();
}

/**
 * @brief Starts the timer.
 */
inline void Timer::start()
{
	begin = std::chrono::high_resolution_clock::now();
}

/**
 * @brief Stops the timer and prints the time passed between the call of start() and this member
 * function. Printed time is expressed in seconds.
 */
inline void Timer::stopAndPrint()
{
	stop();
	print();
}

/**
 * @brief Stops the timer.
 */
inline void Timer::stop()
{
	end       = std::chrono::high_resolution_clock::now();
	isStopped = true;
}

/**
 * @brief Prints the time passed between the call of start() and this member function. Works also if
 * the timer is not stopped. Printed time is expressed in seconds.
 */
inline void Timer::print() const
{
	double secs = delay();
	int    mins = (int) secs / 60;
	if (mins == 0)
		std::cout << "[" << secs << " secs]\t" << caption << std::endl;
	else {
		secs      = secs - mins * 60;
		int hours = mins / 60;
		if (hours == 0)
			std::cout << "[" << mins << " mins; " << secs << " secs]\t" << caption << std::endl;
		else {
			mins = mins - hours * 60;
			std::cout << "[" << hours << " hours; " << mins << " mins; " << secs << " secs]\t"
					  << caption << std::endl;
		}
	}
}

/**
 * @brief Returns the time passed between the call of start() and this member function. Works also
 * if the timer is not stopped. Returned time is expressed in seconds.
 */
inline double Timer::delay() const
{
	double secs;
	if (isStopped) {
		secs =
			(double) (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) /
			1000000;
	}
	else {
		std::chrono::high_resolution_clock::time_point s =
			std::chrono::high_resolution_clock::now();
		secs = (double) (std::chrono::duration_cast<std::chrono::microseconds>(s - begin).count()) /
			   1000000;
	}
	return secs;
}

} // namespace vcl

#endif // VCL_MISC_TIMER_H
