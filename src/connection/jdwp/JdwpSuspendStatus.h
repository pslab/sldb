/*
 * JdwpSuspendStatus.h
 *
 *  Created on: 2016. 3. 28.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_JDWPSUSPENDSTATUS_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_JDWPSUSPENDSTATUS_H_

#include <ostream>

#include "JdwpTypes.h"

class JdwpSuspendStatus final {
public:
	static const Int SUSPEND_STATUS_SUSPENDED = 1;

	JdwpSuspendStatus() {}
	JdwpSuspendStatus(Int suspendStatus) { _suspendStatus = suspendStatus;}
	~JdwpSuspendStatus() {}

	const JdwpSuspendStatus& operator =(Int suspendStatus) { _suspendStatus = suspendStatus; return *this; }
	operator Byte() { return _suspendStatus; }

	void print(std::ostream &out) const;

private:
	Int _suspendStatus = 0;

};

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_JDWPSUSPENDSTATUS_H_ */
