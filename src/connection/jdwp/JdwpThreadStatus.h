/*
 * JdwpThreadStatus.h
 *
 *  Created on: 2016. 3. 28.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_JDWPTHREADSTATUS_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_JDWPTHREADSTATUS_H_

#include <ostream>

#include "JdwpTypes.h"

class JdwpThreadStatus final {
public:
	static const Int ZOMBIE = 0;
	static const Int RUNNING = 1;
	static const Int SLEEPING = 2;
	static const Int MONITOR = 3;
	static const Int WAIT = 4;

	JdwpThreadStatus() {}
	JdwpThreadStatus(Int threadStatus) { _threadStatus = threadStatus;}
	~JdwpThreadStatus() {}

	const JdwpThreadStatus& operator =(Int threadStatus) { _threadStatus = threadStatus; return *this; }
	operator Byte() { return _threadStatus; }

	void print(std::ostream &out) const;

private:
	Int _threadStatus = 0;

};

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_JDWPTHREADSTATUS_H_ */
