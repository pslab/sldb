/*
 * JdwpEventKind.h
 *
 *  Created on: 2016. 3. 15.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPEVENTKIND_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPEVENTKIND_H_

#include <ostream>

#include "JdwpTypes.h"

class JdwpEventKind final {
public:
	//Event Kind
	static const Byte SINGLE_STEP = 1;
	static const Byte BREAKPOINT = 2;
	static const Byte FRAME_POP = 3;
	static const Byte EXCEPTION = 4;
	static const Byte USER_DEFINED = 5;
	static const Byte THREAD_START = 6;
	static const Byte THREAD_DEATH = 7;
	static const Byte CLASS_PREPARE = 8;
	static const Byte CLASS_UNLOAD = 9;
	static const Byte CLASS_LOAD = 10;
	static const Byte FIELD_ACCESS = 20;
	static const Byte FIELD_MODIFICATION = 21;
	static const Byte EXCEPTION_CATCH = 30;
	static const Byte METHOD_ENTRY = 40;
	static const Byte METHOD_EXIT = 41;
	static const Byte METHOD_EXIT_WITH_RETURN_VALUE = 42;
	static const Byte MONITOR_CONTENDED_ENTER = 43;
	static const Byte MONITOR_CONTENDED_ENTERED = 44;
	static const Byte MONITOR_WAIT = 45;
	static const Byte MONITOR_WAITED = 46;
	static const Byte VM_START = 90;
	static const Byte VM_DEATH = 99;
	static const Byte VM_DISCONNECTED = 100;

	JdwpEventKind() {}
	JdwpEventKind(Byte eventKind) : _eventKind(eventKind) {}
	~JdwpEventKind() {}

	const JdwpEventKind& operator =(Byte eventKind) { _eventKind = eventKind; return *this; }
	operator Byte() { return _eventKind; }

	void print(std::ostream &out) const;

private:
	Byte _eventKind = 0;
};


std::ostream& operator<<(std::ostream &out, const JdwpEventKind &rhs);


#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPEVENTKIND_H_ */
