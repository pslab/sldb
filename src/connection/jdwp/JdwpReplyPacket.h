/*
 * JdwpReplyPacket.h
 *
 *  Created on: 2016. 3. 2.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_JDWPREPLYPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_JDWPREPLYPACKET_H_

#include "JdwpPacket.h"

class JdwpReplyPacket: public JdwpPacket {
public:
	//Error code constants.
	static const std::uint16_t NONE = 0;
	static const std::uint16_t INVALID_THREAD = 10;
	static const std::uint16_t INVALID_THREAD_GROUP = 11;
	static const std::uint16_t INVALID_PRIORITY = 12;
	static const std::uint16_t THREAD_NOT_SUSPENDED = 13;
	static const std::uint16_t THREAD_SUSPENDED = 14;
	static const std::uint16_t THREAD_NOT_ALIVE = 15;
	static const std::uint16_t INVALID_OBJECT = 20;
	static const std::uint16_t INVALID_CLASS = 21;
	static const std::uint16_t CLASS_NOT_PREPARED = 22;
	static const std::uint16_t INVALID_METHODID = 23;
	static const std::uint16_t INVALID_LOCATION = 24;
	static const std::uint16_t INVALID_FIELDID = 25;
	static const std::uint16_t INVALID_FRAMEID = 30;
	static const std::uint16_t NO_MORE_FRAMES = 31;
	static const std::uint16_t OPAQUE_FRAME = 32;
	static const std::uint16_t NOT_CURRENT_FRAME = 33;
	static const std::uint16_t TYPE_MISMATCH = 34;
	static const std::uint16_t INVALID_SLOT = 35;
	static const std::uint16_t DUPLICATE = 40;
	static const std::uint16_t NOT_FOUND = 41;
	static const std::uint16_t INVALID_MONITOR = 50;
	static const std::uint16_t NOT_MONITOR_OWNER = 51;
	static const std::uint16_t INTERRUPT = 52;
	static const std::uint16_t INVALID_CLASS_FORMAT = 60;
	static const std::uint16_t CIRCULAR_CLASS_DEFINITION = 61;
	static const std::uint16_t FAILS_VERIFICATION = 62;
	static const std::uint16_t ADD_METHOD_NOT_IMPLEMENTED = 63;
	static const std::uint16_t SCHEMA_CHANGE_NOT_IMPLEMENTED = 64;
	static const std::uint16_t INVALID_TYPESTATE = 65;
	static const std::uint16_t HIERARCHY_CHANGE_NOT_IMPLEMENTED = 66;
	static const std::uint16_t DELETE_METHOD_NOT_IMPLEMENTED = 67;
	static const std::uint16_t UNSUPPORTED_VERSION = 68;
	static const std::uint16_t NAMES_DONT_MATCH = 69;
	static const std::uint16_t CLASS_MODIFIERS_CHANGE_NOT_IMPLEMENTED = 70;
	static const std::uint16_t METHOD_MODIFIERS_CHANGE_NOT_IMPLEMENTED = 71;
	static const std::uint16_t NOT_IMPLEMENTED = 99;
	static const std::uint16_t NULL_POINTER = 100;
	static const std::uint16_t ABSENT_INFORMATION = 101;
	static const std::uint16_t INVALID_EVENT_TYPE = 102;
	static const std::uint16_t ILLEGAL_ARGUMENT = 103;
	static const std::uint16_t OUT_OF_MEMORY = 110;
	static const std::uint16_t ACCESS_DENIED = 111;
	static const std::uint16_t VM_DEAD = 112;
	static const std::uint16_t INTERNAL = 113;
	static const std::uint16_t UNATTACHED_THREAD = 115;
	static const std::uint16_t INVALID_TAG = 500;
	static const std::uint16_t ALREADY_INVOKING = 502;
	static const std::uint16_t INVALID_INDEX = 503;
	static const std::uint16_t INVALID_LENGTH = 504;
	static const std::uint16_t INVALID_STRING = 506;
	static const std::uint16_t INVALID_CLASS_LOADER = 507;
	static const std::uint16_t INVALID_ARRAY = 508;
	static const std::uint16_t TRANSPORT_LOAD = 509;
	static const std::uint16_t TRANSPORT_INIT = 510;
	static const std::uint16_t NATIVE_METHOD = 511;
	static const std::uint16_t INVALID_COUNT = 512;
	static const std::uint16_t HCR_OPERATION_REFUSED = 900; // HCR specific.

	JdwpReplyPacket();
	JdwpReplyPacket(const JdwpReplyPacket &packet);
	virtual ~JdwpReplyPacket();

	std::uint16_t getErrorCode() const { return _errorCode; }
	void setErrorCode(std::uint16_t errorCode) { _errorCode = errorCode; }

	virtual void print(std::ostream &out) const override;

protected:
	virtual void readSpecificHeaderFields(const std::vector<std::uint8_t> &bytes) final override;
	virtual void writeSpecificHeaderFields(std::vector<std::uint8_t> &bytes) const final override;
	virtual void readData() override {}
	virtual void writeData() override {}

private:
	static const std::size_t INDEX_ERROR_CODE = 9;
	std::uint16_t _errorCode = NONE;
};

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_JDWPREPLYPACKET_H_ */
