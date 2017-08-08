/*
 * JdwpSuspendPolicy.h
 *
 *  Created on: 2016. 3. 28.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_JDWPSUSPENDPOLICY_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_JDWPSUSPENDPOLICY_H_

#include <ostream>

#include "JdwpTypes.h"

class JdwpSuspendPolicy final {
public:
	static const Byte NONE = 0;
	static const Byte EVENT_THREAD = 1;
	static const Byte ALL = 2;

	JdwpSuspendPolicy() {}
	JdwpSuspendPolicy(Byte suspendPolicy) { _suspendPolicy = suspendPolicy;}
	~JdwpSuspendPolicy() {}

	const JdwpSuspendPolicy& operator =(Byte suspendPolicy) { _suspendPolicy = suspendPolicy; return *this; }
	operator Byte() { return _suspendPolicy; }

	void print(std::ostream &out) const;

private:
	Byte _suspendPolicy = 0;

};

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_JDWPSUSPENDPOLICY_H_ */
