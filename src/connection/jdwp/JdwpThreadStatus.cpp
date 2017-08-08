/*
 * JdwpThreadStatus.cpp
 *
 *  Created on: 2016. 3. 28.
 *      Author: lockpick
 */

#include "JdwpThreadStatus.h"

void JdwpThreadStatus::print(std::ostream &out) const
{
	switch(_threadStatus) {
	case ZOMBIE:
		out << "ZOMBIE";
		break;
	case RUNNING:
		out << "RUNNING";
		break;
	case SLEEPING:
		out << "SLEEPING";
		break;
	case MONITOR:
			out << "MONITOR";
			break;
	case WAIT:
			out << "WAIT";
			break;
	default:
		out << "UNKNOWN ThreadStatus: ";
		out << static_cast<int>(_threadStatus);
		break;
	}
}

std::ostream& operator<<(std::ostream &out, const JdwpThreadStatus &rhs)
{
	rhs.print(out);
	return out;
}
