/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_TIMER_H
#define MGP_TIMER_H

#include <chrono>
#include <iostream>
#include <string>
#include <time.h>

namespace mgp {

/**
 * @brief The Timer class
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
	std::string                                    caption;
	std::chrono::high_resolution_clock::time_point begin, end;
	bool                                           isStopped;
};

} // namespace mgp

#include "timer.cpp"

#endif // MGP_TIMER_H
