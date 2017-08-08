/*
 * JdwpSuspendPolicy.cpp
 *
 *  Created on: 2016. 3. 28.
 *      Author: lockpick
 */

#include "JdwpSuspendPolicy.h"

void JdwpSuspendPolicy::print(std::ostream &out) const
{
	switch(_suspendPolicy) {
	case NONE:
		out << "NONE";
		break;
	case EVENT_THREAD:
		out << "EVENT_THREAD";
		break;
	case ALL:
		out << "ALL";
		break;
	default:
		out << "UNKNOWN SuspendPolicy: ";
		out << static_cast<int>(_suspendPolicy);
		break;
	}
}

std::ostream& operator<<(std::ostream &out, const JdwpSuspendPolicy &rhs)
{
	rhs.print(out);
	return out;
}
