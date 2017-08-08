/*
 * JdwpTypeTag.cpp
 *
 *  Created on: 2016. 3. 16.
 *      Author: Seunghun Han
 */

#include "JdwpTypes.h"

void JdwpTypeTag::print(std::ostream &out) const
{
	switch (_typeTag) {
	case CLASS:
		out << "CLASS";
		break;
	case INTERFACE:
		out << "INTERFACE";
		break;
	case ARRAY:
		out << "ARRAY";
		break;
	default:
		out << "UNKNOWN TypeTag: ";
		out << static_cast<int>(_typeTag);
		break;
	}
}

std::ostream& operator<<(std::ostream &out, const JdwpTypeTag &rhs)
{
	rhs.print(out);
	return out;
}
