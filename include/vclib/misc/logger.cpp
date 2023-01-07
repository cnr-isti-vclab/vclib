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

namespace vcl {

inline Logger::Logger()
{
	stack.push({0, 100});
	updateStep();
}

inline void Logger::reset()
{
	while (!stack.empty())
		stack.pop();
	stack.push({0, 100});
	updateStep();
}

inline void Logger::startCurrentAction(double fromPerc, double toPerc, const std::string &action)
{
	assert(fromPerc >= 0);
	assert(toPerc <= 100);
	std::pair<double, double> newP;
	newP.first = stack.top().first * (fromPerc / 100);
	newP.second = stack.top().second * (toPerc / 100);
	progress = newP.first;
	stack.push(newP);
	updateStep();

	o << "[" << percentage() <<  "% ] Start " << action << std::endl;
}

inline void Logger::endCurrentAction(const std::string& action)
{
	progress = stack.top().second;
	if (stack.size() > 1) {
		o << "[" << percentage() <<  "% ] End " << action << std::endl;
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

inline void Logger::log(const std::string &msg)
{
	log(101, PROGRESS, msg);
}

inline void Logger::log(LogLevel lvl, const std::string &msg)
{
	log(101, lvl, msg);
}

inline void Logger::log(uint perc, const std::string &msg)
{
	log(perc, PROGRESS, msg);
}

inline void Logger::log(uint perc, LogLevel lvl, const std::string &msg)
{
	if (perc >= 0 && perc <= 100)
		setLocalPerc(perc);
	switch (lvl) {
	case ERROR:
		e << "[" << percentage() <<"% ] ERROR: " << msg << std::endl;
		break;
	case WARNING:
		e << "[" << percentage() <<"% ] WARNING: " << msg << std::endl;
		break;
	case PROGRESS:
		o << "[" << percentage() <<"% ] " << msg << std::endl;
		break;
	case DEBUG:
		o << "[" << percentage() <<"% ] " << msg << std::endl;
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

} // namespace vcl
