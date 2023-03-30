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

#ifndef VCL_MISC_LOGGER_LOGGER_H
#define VCL_MISC_LOGGER_LOGGER_H

#include <cmath>
#include <iostream>
#include <mutex>
#include <sstream>
#include <stack>

#include <vclib/misc/timer.h>
#include <vclib/types.h>

namespace vcl {

template<typename Stream>
class Logger
{
public:
	enum LogLevel { ERROR = 0, WARNING, PROGRESS, DEBUG };

	Logger();

	void enableIndentation();
	void disableIndentation();

	void reset();

	void setMaxLineWidth(uint w);
	void setPrintTimer(bool b);
	void startTimer();

	void startNewTask(double fromPerc, double toPerc, const std::string& action);
	void endTask(const std::string& action);

	double percentage() const;
	virtual void setPercentage(uint newPerc);

	void log(const std::string& msg);
	void log(LogLevel lvl, const std::string& msg);
	void log(uint perc, const std::string& msg);
	void log(uint perc, LogLevel lvl, const std::string& msg);

	void startProgress(
		const std::string& msg,
		uint               progressSize,
		uint               percPrintProgress = 10,
		uint               startPerc = 0,
		uint               endPerc = 100);
	void endProgress();

	void progress(uint n);

protected:
	// you should override this member function if you want to use a different stream that are not
	// std::cout and std::cerr
	virtual Stream* levelStream(LogLevel lvl) = 0;

private:
	enum InternalLogLevel { START = DEBUG + 1, END };

	uint percPrecision = 0;

	// on top of the stack, we save the interval percentages of the current task
	// values are between 0 and 100
	std::stack<std::pair<double, double>> stack;

	// actual percentage (0 - 100), that is in the interval in top of the stack
	double globalPercProgress;

	double step; // the value that corresponds to 1% on the current task

	bool indent = true;
	uint lineW = 80;
	const uint TIMER_SIZE = 12;

	vcl::Timer timer;
	bool printTimer = false;

	// progress status members
	bool isProgressActive = false;
	std::string progressMessage;
	uint progressStep;
	uint progressPerc;
	uint progressPercStep;
	uint lastProgress;

	std::mutex mutex;

	void updateStep();

	void printLine(const std::string& msg, uint lvl);

	uint printPercentage(std::ostream& o) const;
	uint printIndentation(std::ostream& o) const;
	void printMessage(std::ostream& o, const std::string& msg, uint lvl, uint n);
	void printElapsedTime(std::ostream& o) const;
};

} // namespace vcl

#include "logger.cpp"

#endif // VCL_MISC_LOGGER_LOGGER_H
