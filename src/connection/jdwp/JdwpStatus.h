/*
 * JdwpStatus.h
 *
 *  Created on: 2016. 7. 19.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_JDWPSTATUS_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_JDWPSTATUS_H_

#include <ostream>

#include "JdwpTypes.h"

class JdwpStatus final {
public:
	// Status
	static const Int VERIFIED = 1;
	static const Int PREPARED = 2;
	static const Int INITIALIZED = 4;
	static const Int ERROR = 8;

	JdwpStatus() {}
	JdwpStatus(Int status) : _status(status) {}
	virtual ~JdwpStatus() {}

	const JdwpStatus& operator =(Int status) { _status = status; return *this; }
	operator Int() { return _status; }

	void print(std::ostream &out) const;

private:
	Int _status = 0;
};

std::ostream& operator<<(std::ostream &out, const JdwpStatus &rhs);

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_JDWPSTATUS_H_ */
