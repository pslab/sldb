/*
 * JdwpTag.h
 *
 *  Created on: 2016. 3. 16.
 *      Author: Seunghun Han
 */

#ifndef IKESDBG_SRC_CONNECTION_JDWP_JDWPTAG_H_
#define IKESDBG_SRC_CONNECTION_JDWP_JDWPTAG_H_

#include <ostream>

#include "JdwpTypes.h"

class JdwpTag final {
public:
	// Tag
	static const Byte ARRAY = '[';
	static const Byte BYTE = 'B';
	static const Byte CHAR = 'C';
	static const Byte OBJECT = 'L';
	static const Byte FLOAT = 'F';
	static const Byte DOUBLE = 'D';
	static const Byte INT = 'I';
	static const Byte LONG = 'J';
	static const Byte SHORT = 'S';
	static const Byte VOID = 'V';
	static const Byte BOOLEAN = 'Z';
	static const Byte STRING = 's';
	static const Byte THREAD = 't';
	static const Byte THREAD_GROUP = 'g';
	static const Byte CLASS_LOADER = 'l';
	static const Byte CLASS_OBJECT = 'c';

	JdwpTag() {}
	JdwpTag(Byte tag) : _tag(tag) {}
	virtual ~JdwpTag() {}

	const JdwpTag& operator =(Byte tag) { _tag = tag; return *this; }
	operator Byte() { return _tag; }

	void print(std::ostream &out) const;

private:
	Byte _tag = 0;
};

std::ostream& operator<<(std::ostream &out, const JdwpTag &rhs);

#endif /* IKESDBG_SRC_CONNECTION_JDWP_JDWPTAG_H_ */
