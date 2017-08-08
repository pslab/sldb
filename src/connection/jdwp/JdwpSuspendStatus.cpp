/*
 * JdwpSuspendStatus.cpp
 *
 *  Created on: 2016. 3. 28.
 *      Author: lockpick
 */

#include "JdwpSuspendStatus.h"

void JdwpSuspendStatus::print(std::ostream &out) const
{
	switch(_suspendStatus) {
	case SUSPEND_STATUS_SUSPENDED:
			out << "SUSPEND_STATUS_SUSPENDED";
			break;
	default:
		out << "UNKNOWN SuspendStatus: ";
		out << static_cast<int>(_suspendStatus);
		break;
	}
}

std::ostream& operator<<(std::ostream &out, const JdwpSuspendStatus &rhs)
{
	rhs.print(out);
	return out;
}
