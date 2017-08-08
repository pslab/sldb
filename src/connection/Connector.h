/*
 * Connector.h
 *
 *  Created on: 2016. 2. 25.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_CONNECTOR_H_
#define JIKESDBG_SRC_CONNECTION_CONNECTOR_H_

#include <connection/jdwp/reply/JdwpReplyCtsuperclassPacket.h>
#include "socket/Socket.h"

#include <iostream>
#include <memory>

#include "jdwp/JdwpPacket.h"
#include "jdwp/JdwpTypes.h"

#include "jdwp/JdwpCommandPacket.h"
#include "jdwp/command/JdwpCommandVmclassesbysignaturePacket.h"
#include "jdwp/command/JdwpCommandEcompositePacket.h"
#include "jdwp/command/JdwpCommandErsetPacket.h"
#include "jdwp/command/JdwpCommandCtsuperclassPacket.h"
#include "jdwp/command/JdwpCommandRtinterfacesPacket.h"
#include "jdwp/command/JdwpCommandRtfieldswithgenericPacket.h"
#include "jdwp/command/JdwpCommandOrreferencetypePacket.h"
#include "jdwp/command/JdwpCommandTrnamePacket.h"
#include "jdwp/command/JdwpCommandTrsuspendPacket.h"
#include "jdwp/command/JdwpCommandTrresumePacket.h"
#include "jdwp/command/JdwpCommandTrstatusPacket.h"
#include "jdwp/command/JdwpCommandTrthreadgroupPacket.h"
#include "jdwp/command/JdwpCommandTrframesPacket.h"
#include "jdwp/command/JdwpCommandTgrnamePacket.h"
#include "jdwp/command/JdwpCommandTgrparentPacket.h"
#include "jdwp/command/JdwpCommandTgrchildrenPacket.h"

#include "jdwp/JdwpReplyPacket.h"
#include "jdwp/reply/JdwpReplyErsetPacket.h"
#include "jdwp/reply/JdwpReplyVmclassesbysignaturePacket.h"
#include "jdwp/reply/JdwpReplyVmallthreadsPacket.h"
#include "jdwp/reply/JdwpReplyVmidsizesPacket.h"
#include "jdwp/reply/JdwpReplyVmversionPacket.h"
#include "jdwp/reply/JdwpReplyVmcapabilitiesnewPacket.h"
#include "jdwp/reply/JdwpReplyCtsuperclassPacket.h"
#include "jdwp/reply/JdwpReplyRtinterfacesPacket.h"
#include "jdwp/reply/JdwpReplyRtfieldswithgenericPacket.h"
#include "jdwp/reply/JdwpReplyOrreferencetypePacket.h"
#include "jdwp/reply/JdwpReplyTrnamePacket.h"
#include "jdwp/reply/JdwpReplyTrstatusPacket.h"
#include "jdwp/reply/JdwpReplyTrthreadgroupPacket.h"
#include "jdwp/reply/JdwpReplyTrframesPacket.h"
#include "jdwp/reply/JdwpReplyTgrnamePacket.h"
#include "jdwp/reply/JdwpReplyTgrparentPacket.h"
#include "jdwp/reply/JdwpReplyTgrchildrenPacket.h"

class Connector {
public:
	Connector() {}
	virtual ~Connector() { disconnect(); }

	void printOn() { print = true; }
	void prointOff() { print = false; }

	bool connect(std::string address);

	template <typename Vm>
	std::shared_ptr<JdwpPacket> recvPacket() const;
	void sendPacket(std::shared_ptr<JdwpPacket> packet) const;

	void disconnect() noexcept;

private:
	bool print = false;

	std::vector<std::uint8_t> read(std::size_t count) const;
	void write(const std::vector<std::uint8_t> &bytes) const;

	template <typename Vm>
	static std::shared_ptr<JdwpPacket> castToSpecificPacket(const std::shared_ptr<JdwpPacket> &packet);

	std::unique_ptr<Socket> _socket;
};

template <typename Vm>
std::shared_ptr<JdwpPacket> Connector::recvPacket() const
{

	auto bytes = read(JdwpPacket::MIN_PACKET_LENGTH);
	auto length = JdwpPacket::getLengthFromHeader(bytes);
	if (length > JdwpPacket::MIN_PACKET_LENGTH) {
		auto data = read(length-JdwpPacket::MIN_PACKET_LENGTH);
		bytes.insert(bytes.end(), data.cbegin(), data.cend());
	}

	auto packet = JdwpPacket::unpack(bytes);
	packet = castToSpecificPacket<Vm>(packet);

	if (print == true) {
		std::cout << "recv : " << *packet << std::endl;
	}

	return packet;
}

template <typename Vm>
std::shared_ptr<JdwpPacket> Connector::castToSpecificPacket(const std::shared_ptr<JdwpPacket> &packet)
{
	std::shared_ptr<JdwpPacket> p = packet;
	if (packet->getFlags() != JdwpPacket::FLAG_REPLY_PACKET) {
		auto command = std::dynamic_pointer_cast<JdwpCommandPacket>(packet);
		switch(command->getCommand()) {
		case JdwpCommandPacket::VM_CLASSES_BY_SIGNATURE:
			p = std::make_shared<JdwpCommandVmclassesbysignaturePacket<Vm>>(*command);
			break;

		case JdwpCommandPacket::ER_SET:
			p = std::make_shared<JdwpCommandErsetPacket<Vm>>(*command);
			break;

		case JdwpCommandPacket::CT_SUPERCLASS :
			p = std::make_shared<JdwpCommandCtsuperclassPacket<Vm>>(*command);
			break;

		case JdwpCommandPacket::RT_INTERFACES :
			p = std::make_shared<JdwpCommandRtinterfacesPacket<Vm>>(*command);
			break;

		case JdwpCommandPacket::RT_FIELDS_WITH_GENERIC :
			p = std::make_shared<JdwpCommandRtfieldswithgenericPacket<Vm>>(*command);
			break;

		case JdwpCommandPacket::OR_REFERENCE_TYPE:
			p = std::make_shared<JdwpCommandOrreferencetypePacket<Vm>>(*command);
			break;

		case JdwpCommandPacket::TR_NAME:
			p = std::make_shared<JdwpCommandTrnamePacket<Vm>>(*command);
			break;

		case JdwpCommandPacket::TR_SUSPEND:
			p = std::make_shared<JdwpCommandTrsuspendPacket<Vm>>(*command);
			break;

		case JdwpCommandPacket::TR_RESUME:
			p = std::make_shared<JdwpCommandTrresumePacket<Vm>>(*command);
			break;

		case JdwpCommandPacket::TR_STATUS:
			p = std::make_shared<JdwpCommandTrstatusPacket<Vm>>(*command);
			break;

		case JdwpCommandPacket::TR_THREAD_GROUP:
			p = std::make_shared<JdwpCommandTrthreadgroupPacket<Vm>>(*command);
			break;

		case JdwpCommandPacket::TR_FRAMES:
			p = std::make_shared<JdwpCommandTrframesPacket<Vm>>(*command);
			break;

		case JdwpCommandPacket::TGR_NAME:
			p = std::make_shared<JdwpCommandTgrnamePacket<Vm>>(*command);
			break;

		case JdwpCommandPacket::TGR_PARENT:
			p = std::make_shared<JdwpCommandTgrparentPacket<Vm>>(*command);
			break;

		case JdwpCommandPacket::TGR_CHILDREN:
			p = std::make_shared<JdwpCommandTgrchildrenPacket<Vm>>(*command);
			break;

		default:
			break;
		}
	}
	return p;
}

#endif /* JIKESDBG_SRC_CONNECTION_CONNECTOR_H_ */
