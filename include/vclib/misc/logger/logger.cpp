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

template<typename Stream>
Logger<Stream>::Logger()
{
	stack.push({0, 100});
	updateStep();
}

template<typename Stream>
void Logger<Stream>::enableIndentation()
{
	indent = true;
}

template<typename Stream>
void Logger<Stream>::disableIndentation()
{
	indent = false;
}

template<typename Stream>
void Logger<Stream>::reset()
{
	while (!stack.empty())
		stack.pop();
	stack.push({0, 100});
	updateStep();
}

template<typename Stream>
void Logger<Stream>::setMaxLineWidth(uint w)
{
	lineW = w;
}

template<typename Stream>
void Logger<Stream>::setPrintTimer(bool b)
{
	printTimer = b;
}

template<typename Stream>
void Logger<Stream>::startTimer()
{
	timer.start();
}

template<typename Stream>
void Logger<Stream>::startNewTask(double fromPerc, double toPerc, const std::string& action)
{
	printLine(action, START);

	assert(fromPerc >= 0);
	assert(toPerc <= 100);
	std::pair<double, double> newP;
	newP.first = stack.top().first * (fromPerc / 100);
	newP.second = stack.top().second * (toPerc / 100);
	progress = newP.first;
	stack.push(newP);
	updateStep();
}

template<typename Stream>
void Logger<Stream>::endTask(const std::string& action)
{
	progress = stack.top().second;
	if (stack.size() > 1) {
		stack.pop();
		updateStep();

		printLine(action, END);
	}
}

template<typename Stream>
double Logger<Stream>::percentage() const
{
	double k = std::pow(10, percPrecision);
	uint c = progress * k;
	return c / k;
}

template<typename Stream>
void Logger<Stream>::setPercentage(uint newPerc)
{
	assert(newPerc >= 0 && newPerc <= 100);
	progress = step * newPerc;
}

template<typename Stream>
void Logger<Stream>::log(const std::string& msg)
{
	log(101, PROGRESS, msg);
}

template<typename Stream>
void Logger<Stream>::log(LogLevel lvl, const std::string& msg)
{
	log(101, lvl, msg);
}

template<typename Stream>
void Logger<Stream>::log(uint perc, const std::string& msg)
{
	log(perc, PROGRESS, msg);
}

template<typename Stream>
void Logger<Stream>::log(uint perc, LogLevel lvl, const std::string& msg)
{
	if (perc >= 0 && perc <= 100)
		setPercentage(perc);

	printLine(msg, lvl);
}

template<typename Stream>
void Logger<Stream>::updateStep()
{
	step = (stack.top().second - stack.top().first) / 100;
}

template<typename Stream>
void Logger<Stream>::printLine(const std::string& msg, uint lvl)
{
	LogLevel l = PROGRESS;
	if (lvl < DEBUG){
		l = (LogLevel) lvl;
	}
	std::ostream* stream = levelStream(l);

	if (stream) {
		uint s = 0;
		s = printPercentage(*stream);
		s += printIndentation(*stream);
		printMessage(*stream, msg, lvl, s);
		printElapsedTime(*stream);
		*stream << std::endl;
	}
}

template<typename Stream>
uint Logger<Stream>::printPercentage(std::ostream& o) const
{
	uint size = 3;
	if (percPrecision > 0 )
		size += 1 + percPrecision;

	o << "[";
	o << std::right << std::setw(size) << percentage() << "%]";
	return size + 3;
}

template<typename Stream>
uint Logger<Stream>::printIndentation(std::ostream& o) const
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

template<typename Stream>
void Logger<Stream>::printMessage(std::ostream& o, const std::string& msg, uint lvl, uint n)
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
	case START:
		maxMsgSize -= 7;
		o << " Start ";
		break;
	case END:
		maxMsgSize -= 5;
		o << " End ";
		break;
	}
	o << std::setw(maxMsgSize) << std::left << msg;
}

template<typename Stream>
void Logger<Stream>::printElapsedTime(std::ostream& o) const
{
	if (printTimer) {
		o << "[" << std::setw(TIMER_SIZE - 3) << std::right << timer.delay() << "s]";
	}
}

} // namespace vcl
