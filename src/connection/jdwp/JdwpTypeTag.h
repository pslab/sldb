/*
 * JdwpTypeTag.h
 *
 *  Created on: 2016. 3. 16.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_JDWPTYPETAG_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_JDWPTYPETAG_H_

#include <ostream>

#include "JdwpTypes.h"

class JdwpTypeTag final {
public:
	// Type Tag
	static const Byte CLASS = 1;
	static const Byte INTERFACE = 2;
	static const Byte ARRAY = 3;

	JdwpTypeTag() {}
	JdwpTypeTag(Byte typeTag) : _typeTag(typeTag) {}
	virtual ~JdwpTypeTag() {}

	const JdwpTypeTag& operator =(Byte typeTag) { _typeTag = typeTag; return *this; }
	operator Byte() { return _typeTag; }

	void print(std::ostream &out) const;

private:
	Byte _typeTag = 0;
};

std::ostream& operator<<(std::ostream &out, const JdwpTypeTag &rhs);

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_JDWPTYPETAG_H_ */
