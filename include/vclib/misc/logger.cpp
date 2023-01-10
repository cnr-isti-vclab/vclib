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

#include "logger.h"
#include <iomanip>

namespace vcl {

inline Logger::Logger()
{
	stack.push({0, 100});
	updateStep();
}

inline void Logger::enableIndentation()
{
	indent = true;
}

inline void Logger::disableIndentation()
{
	indent = false;
}

inline void Logger::reset()
{
	while (!stack.empty())
		stack.pop();
	stack.push({0, 100});
	updateStep();
}

inline void Logger::setMaxLineWidth(uint w)
{
	lineW = w;
}

inline void Logger::setPrintTimer(bool b)
{
	printTimer = b;
}

inline void Logger::startTimer()
{
	timer.start();
}

inline void Logger::startNewTask(double fromPerc, double toPerc, const std::string& action)
{
	uint s = printPercentage(o);
	s += printIndentation(o);
	printMessage(o, action, 4, s);
	printElapsedTime(o);
	o << std::endl;

	assert(fromPerc >= 0);
	assert(toPerc <= 100);
	std::pair<double, double> newP;
	newP.first = stack.top().first * (fromPerc / 100);
	newP.second = stack.top().second * (toPerc / 100);
	progress = newP.first;
	stack.push(newP);
	updateStep();
}

inline void Logger::endTask(const std::string& action)
{
	progress = stack.top().second;
	if (stack.size() > 1) {
		stack.pop();
		updateStep();

		uint s = printPercentage(o);
		s += printIndentation(o);
		printMessage(o, action, 5, s);
		printElapsedTime(o);
		o << std::endl;
	}
}

inline double Logger::percentage() const
{
	double k = std::pow(10, percPrecision);
	uint c = progress * k;
	return c / k;
}

inline void Logger::log(const std::string& msg)
{
	log(101, PROGRESS, msg);
}

inline void Logger::log(LogLevel lvl, const std::string& msg)
{
	log(101, lvl, msg);
}

inline void Logger::log(uint perc, const std::string& msg)
{
	log(perc, PROGRESS, msg);
}

inline void Logger::log(uint perc, LogLevel lvl, const std::string& msg)
{
	uint s = 0;
	if (perc >= 0 && perc <= 100)
		setLocalPerc(perc);
	switch (lvl) {
	case ERROR:
	case WARNING:
		s += printPercentage(e);
		s += printIndentation(e);
		printMessage(e, msg, lvl, s);
		printElapsedTime(e);
		e << std::endl;
		break;
	case PROGRESS:
	case DEBUG:
		s += printPercentage(o);
		s += printIndentation(o);
		printMessage(o, msg, lvl, s);
		printElapsedTime(o);
		o << std::endl;
		break;
	}
}

inline void Logger::updateStep()
{
	step = (stack.top().second - stack.top().first) / 100;
}

inline void Logger::setLocalPerc(uint perc)
{
	assert(perc >= 0 && perc <= 100);
	progress = step * perc;
}

inline uint Logger::printPercentage(std::ostream& o) const
{
	uint size = 3;
	if (percPrecision > 0 )
		size += 1 + percPrecision;

	o << "[";
	o << std::right << std::setw(size) << percentage() << "%]";
	return size + 3;
}

inline uint Logger::printIndentation(std::ostream& o) const
{
	uint s = 0;
	if (indent) {
		uint n = stack.size() - 1;
		for (uint i = 0; i < n; i++) {
			o << "  ";
			s += 2;
		}
	}
	return s;
}

inline void Logger::printMessage(std::ostream& o, const std::string& msg, uint lvl, uint n)
{
	uint maxMsgSize = lineW - n;
	if (printTimer)
		maxMsgSize -= TIMER_SIZE;
	switch (lvl) {
	case ERROR:
		maxMsgSize -= 8;
		o << " ERROR: ";
		break;
	case WARNING:
		maxMsgSize -= 10;
		o << " WARNING: ";
		break;
	case PROGRESS:
		maxMsgSize -= 1;
		o << " ";
		break;
	case DEBUG:
		maxMsgSize -= 9;
		o << " (debug) ";
		break;
	case 4:
		maxMsgSize -= 7;
		o << " Start ";
		break;
	case 5:
		maxMsgSize -= 5;
		o << " End ";
		break;
	}
	o << std::setw(maxMsgSize) << std::left << msg;
}

inline void Logger::printElapsedTime(std::ostream& o) const
{
	if (printTimer) {
		o << "[" << std::setw(TIMER_SIZE - 3) << std::right << timer.delay() << "s]";
	}
}

} // namespace vcl
