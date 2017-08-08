/*
 * JdwpEventKind.cpp
 *
 *  Created on: 2016. 3. 16.
 *      Author: lockpick
 */

#include "JdwpTypes.h"

void JdwpEventKind::print(std::ostream &out) const
{
	switch(_eventKind) {
	case VM_DISCONNECTED:
		out << "VM_DISCONNECTED";
		break;
	case SINGLE_STEP:
		out << "SINGLE_STEP";
		break;
	case BREAKPOINT:
		out << "BREAKPOINT";
		break;
	case FRAME_POP:
		out << "FRAME_POP";
		break;
	case EXCEPTION:
		out << "EXCEPTION";
		break;
	case USER_DEFINED:
		out << "USER_DEFINED";
		break;
	case THREAD_START:
		out << "THREAD_START";
		break;
	case THREAD_DEATH:
		out << "THREAD_DEATH";
		break;
	case CLASS_PREPARE:
		out << "CLASS_PREPARE";
		break;
	case CLASS_UNLOAD:
		out << "CLASS_UNLOAD";
		break;
	case CLASS_LOAD:
		out << "CLASS_LOAD";
		break;
	case FIELD_ACCESS:
		out << "FIELD_ACCESS";
		break;
	case FIELD_MODIFICATION:
		out << "FIELD_MODIFICATION";
		break;
	case EXCEPTION_CATCH:
		out << "EXCEPTION_CATCH";
		break;
	case METHOD_ENTRY:
		out << "METHOD_ENTRY";
		break;
	case METHOD_EXIT:
		out << "METHOD_EXIT";
		break;
	case METHOD_EXIT_WITH_RETURN_VALUE:
		out << "METHOD_EXIT_WITH_RETURN_VALUE";
		break;
	case MONITOR_CONTENDED_ENTER:
		out << "MONITOR_CONTENDED_ENTER";
		break;
	case MONITOR_CONTENDED_ENTERED:
		out << "MONITOR_CONTENDED_ENTERED";
		break;
	case MONITOR_WAIT:
		out << "MONITOR_WAIT";
		break;
	case MONITOR_WAITED:
		out << "MONITOR_WAITED";
		break;
	case VM_START:
		out << "VM_START";
		break;
	case VM_DEATH:
		out << "VM_DEATH";
		break;
	default:
		out << "UNKNOWN Event: ";
		out << static_cast<int>(_eventKind);
		break;
	}
}

std::ostream& operator<<(std::ostream &out, const JdwpEventKind &rhs)
{
	rhs.print(out);
	return out;
}
