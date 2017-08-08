/*
 * JdwpTypeTag.cpp
 *
 *  Created on: 2016. 7. 19.
 *      Author: Seunghun Han
 */

#include "JdwpStatus.h"

void JdwpStatus::print(std::ostream &out) const
{
	switch (_status) {
	case VERIFIED:
		out << "VERIFIED";
		break;
	case PREPARED:
		out << "PREPARED";
		break;
	case INITIALIZED:
		out << "INITIALIZED";
		break;
	case ERROR:
		out << "ERROR";
		break;
	default:
		out << "UNKNOWN Status: ";
		out << static_cast<int>(_status);
		break;
	}
}

std::ostream& operator<<(std::ostream &out, const JdwpStatus &rhs)
{
	rhs.print(out);
	return out;
}
