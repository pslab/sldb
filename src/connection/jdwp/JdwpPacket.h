/*
 * JdwpPacket.h
 *
 *  Created on: 2016. 3. 2.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_JDWPPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_JDWPPACKET_H_

#include <ostream>
#include <vector>
#include <memory>
#include <cstdint>

#include "JdwpTypes.h"

class JdwpPacket {
public:
	static const std::uint8_t FLAG_REPLY_PACKET = 0x80u;
	static const std::uint32_t MIN_PACKET_LENGTH = 11;

	JdwpPacket();
	JdwpPacket(const JdwpPacket &packet);
	virtual ~JdwpPacket();

	Int getId() const { return _id; }

	std::uint8_t getFlags() const { return _flags; }
	std::size_t getLength() const { return MIN_PACKET_LENGTH+getDataLength(); }
	std::size_t getDataLength() const { return _dataBuf.size(); }

	static std::uint32_t getLengthFromHeader(const std::vector<std::uint8_t> &bytes);

	static std::shared_ptr<JdwpPacket> unpack(const std::vector<std::uint8_t> &bytes);
	std::vector<std::uint8_t> pack();

	virtual void setId(Int id) { _id = id; }

	virtual void print(std::ostream &out) const = 0;

protected:
	std::vector<std::uint8_t> _dataBuf;
	void setFlags(std::uint8_t flags) { _flags = flags; }

	virtual void readSpecificHeaderFields(const std::vector<std::uint8_t> &bytes) = 0;
	virtual void writeSpecificHeaderFields(std::vector<std::uint8_t> &bytes) const = 0;
	virtual void readData() = 0;
	virtual void writeData() = 0;

private:
	Int _id = 0;
	std::uint8_t _flags = 0;
};

std::ostream& operator<<(std::ostream &out, const JdwpPacket &rhs);


#endif /* JIKESDBG_SRC_CONNECTION_JDWP_JDWPPACKET_H_ */
