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
	assert(fromPerc >= 0);
	assert(toPerc <= 100);
	std::pair<double, double> newP;
	newP.first = stack.top().first * (fromPerc / 100);
	newP.second = stack.top().second * (toPerc / 100);
	progress = newP.first;
	stack.push(newP);
	updateStep();

	printPercentage(o);
	o << " Start " + action;
	printElapsedTime(o, 7 + action.size());
	o << std::endl;
}

inline void Logger::endTask(const std::string& action)
{
	progress = stack.top().second;
	if (stack.size() > 1) {
		printPercentage(o);
		o << " End " + action;
		printElapsedTime(o, 5 + action.size());
		o << std::endl;
		stack.pop();
		updateStep();
	}
}

inline double Logger::percentage() const
{
	double k = std::pow(10, percPrecision);
	uint c = progress * k;
	return c / k;
}

inline std::string Logger::percentageString() const
{
	std::string s;
	double k = std::pow(10, percPrecision);
	uint c = progress * k;
	uint n = c / k;
	uint d = c % (uint)k;
	while (d != 0 && d % 10 == 0)
		d /= 10;

	s += std::to_string(n);
	if (percPrecision > 0 && d != 0) {
		s += "." + std::to_string(d);
	}
	return s;
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
	if (perc >= 0 && perc <= 100)
		setLocalPerc(perc);
	switch (lvl) {
	case ERROR:
		printPercentage(e);
		printIndentation(e);
		e << " ERROR: " + msg;
		printElapsedTime(e, 8 + msg.size());
		e << std::endl;
		break;
	case WARNING:
		printPercentage(e);
		printIndentation(e);
		e << " WARNING: " + msg;
		printElapsedTime(e, 10 + msg.size());
		e << std::endl;
		break;
	case PROGRESS:
		printPercentage(o);
		printIndentation(o);
		o << " " + msg;
		printElapsedTime(o, 1 + msg.size());
		o << std::endl;
		break;
	case DEBUG:
		printPercentage(o);
		printIndentation(o);
		o << " (debug) " + msg;
		printElapsedTime(o, 9 + msg.size());
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

inline void Logger::printElapsedTime(std::ostream& o, uint msgSize) const
{
	if (printTimer) {
		uint indentSize = 0;
		if (indent) {
			indentSize = (stack.size()-1)*2;
		}
		uint i = msgSize + percentageString().size() + 4 + percPrecision + indentSize;
		while (i < 80 - 12) {
			o << " ";
			i++;
		}

		o << "[ " << timer.delay() << "s]";
	}
}

inline void Logger::printPercentage(std::ostream& o) const
{
	std::string s = percentageString();

	o << "[";
	uint i = s.size();
	while (i < 4 + percPrecision) {
		o << " ";
		i++;
	}
	o << s << "%]";
}

inline void Logger::printIndentation(std::ostream& o) const
{
	if (indent) {
		uint n = stack.size() - 1;
		for (uint i = 0; i < n; i++)
			o << "  ";
	}
}

} // namespace vcl
