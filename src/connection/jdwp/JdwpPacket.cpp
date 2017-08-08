/*
 * JdwpPacket.cpp
 *
 *  Created on: 2016. 3. 2.
 *      Author: Seunghun Han
 */

#include "JdwpPacket.h"

#include "JdwpCommandPacket.h"
#include "JdwpReplyPacket.h"

JdwpPacket::JdwpPacket()
{
}

JdwpPacket::JdwpPacket(const JdwpPacket &packet) : _dataBuf(packet._dataBuf), _id(packet._id), _flags(packet._flags)
{
}

JdwpPacket::~JdwpPacket()
{
}


std::uint32_t JdwpPacket::getLengthFromHeader(const std::vector<std::uint8_t> &bytes)
{
	std::uint32_t packetLength = ::unpack<std::uint32_t>(bytes);
	return packetLength;
}

std::shared_ptr<JdwpPacket> JdwpPacket::unpack(const std::vector<std::uint8_t> &bytes)
{
	std::uint32_t length = JdwpPacket::getLengthFromHeader(bytes);

	Int id;
	::unpack(bytes.cbegin()+4, &id);

	std::uint8_t flags;
	::unpack(bytes.cbegin()+8, &flags);

	std::shared_ptr<JdwpPacket> packet;
	if ((flags & JdwpPacket::FLAG_REPLY_PACKET) != 0) {
		packet = std::make_shared<JdwpReplyPacket>();
	} else {
		packet = std::make_shared<JdwpCommandPacket>(JdwpCommandPacket::getCommandFromHeader(bytes));
	}

	packet->setId(id);
	packet->setFlags(flags);
	packet->readSpecificHeaderFields(bytes);

	if(length > JdwpPacket::MIN_PACKET_LENGTH) {
		packet->_dataBuf.assign(bytes.cbegin()+JdwpPacket::MIN_PACKET_LENGTH, bytes.cend());
	}

	return packet;
}

std::vector<std::uint8_t> JdwpPacket::pack()
{
	std::vector<std::uint8_t> bytes(MIN_PACKET_LENGTH);

	std::uint32_t len = getLength();
	::pack(len, bytes.begin());

	std::uint32_t id = getId();
	::pack(id, bytes.begin()+4);

	std::uint8_t flags = getFlags();
	::pack(flags, bytes.begin()+8);

	writeSpecificHeaderFields(bytes);

	if(getLength() > MIN_PACKET_LENGTH) {
		bytes.insert(bytes.end(), _dataBuf.cbegin(), _dataBuf.cend());
	}

	return bytes;
}

std::ostream& operator<<(std::ostream &out, const JdwpPacket &rhs)
{
	rhs.print(out);
	return out;
}

