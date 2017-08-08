/*
 * JdwpReplyPacket.cpp
 *
 *  Created on: 2016. 3. 2.
 *      Author: Seunghun Han
 */

#include "JdwpReplyPacket.h"

JdwpReplyPacket::JdwpReplyPacket()
{
	setFlags(FLAG_REPLY_PACKET);
}

JdwpReplyPacket::JdwpReplyPacket(const JdwpReplyPacket &packet) : JdwpPacket(packet), _errorCode(packet._errorCode)
{
}

JdwpReplyPacket::~JdwpReplyPacket()
{
}

void JdwpReplyPacket::readSpecificHeaderFields(const std::vector<std::uint8_t> &bytes)
{
	_errorCode = (static_cast<std::uint16_t>(bytes[INDEX_ERROR_CODE]) << 8) + bytes[INDEX_ERROR_CODE+1];
}

void JdwpReplyPacket::writeSpecificHeaderFields(std::vector<std::uint8_t> &bytes) const
{
	bytes[INDEX_ERROR_CODE] = static_cast<std::uint8_t>(_errorCode >> 8);
	bytes[INDEX_ERROR_CODE + 1] = static_cast<std::uint8_t>(_errorCode);
}

void JdwpReplyPacket::print(std::ostream &out) const
{

	out << "[";
	out << getId();
	out << "] ";
	out << "(reply)";
	auto ec = getErrorCode();
	if (ec != JdwpReplyPacket::NONE) {
		out << " ERROR CODE: ";
		out << ec;
	}
}
