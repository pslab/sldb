/*
 * JdwpTag.cpp
 *
 *  Created on: 2016. 3. 16.
 *      Author: Seunghun Han
 */

#include "JdwpTypes.h"

void JdwpTag::print(std::ostream &out) const
{
	switch (_tag) {
	case ARRAY:
		out << '[';
		break;
	case BYTE:
		out << 'B';
		break;
	case CHAR:
		out << 'C';
		break;
	case OBJECT:
		out << 'L';
		break;
	case FLOAT:
		out << 'F';
		break;
	case DOUBLE:
		out << 'D';
		break;
	case INT:
		out << 'I';
		break;
	case LONG:
		out << 'J';
		break;
	case SHORT:
		out << 'S';
		break;
	case VOID:
		out << 'V';
		break;
	case STRING:
		out << 's';
		break;
	case THREAD:
		out << 't';
		break;
	case THREAD_GROUP:
		out << 'g';
		break;
	case CLASS_LOADER:
		out << 'l';
		break;
	case CLASS_OBJECT:
		out << 'c';
		break;
	default:
		out << "UNKNOWN Tag: ";
		out << static_cast<int>(_tag);
		break;
	}
}

std::ostream& operator<<(std::ostream &out, const JdwpTag &rhs)
{
	rhs.print(out);
	return out;
}
