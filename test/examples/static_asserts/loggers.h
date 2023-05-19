#ifndef LOGGERS_H
#define LOGGERS_H

#include <vclib/misc/logger.h>

void loggersStaticAsserts()
{
	using namespace vcl;

	static_assert(LoggerConcept<NullLogger>, "NullLogger does not satisfy the LoggerConcept");
	static_assert(LoggerConcept<ConsoleLogger>, "ConsoleLogger does not satisfy the LoggerConcept");
}

#endif // LOGGERS_H
