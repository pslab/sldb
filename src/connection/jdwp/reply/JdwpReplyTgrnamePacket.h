/*
 * JdwpReplyTgrnamePacket.h
 *
 *  Created on: 2016. 3. 31.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYTGRNAMEPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYTGRNAMEPACKET_H_

#include "../JdwpReplyPacket.h"

#include <string>

#include "../JdwpTypes.h"

/**
 * EventRequest Command Set (15)
 * Set Command (1)
 */
template <typename Vm>
class JdwpReplyTgrnamePacket final : public JdwpReplyPacket {
public:
	JdwpReplyTgrnamePacket();
	virtual ~JdwpReplyTgrnamePacket();

	void setGroupName(std::string groupName);

	virtual void print(std::ostream &out) const override;

private:
	virtual void writeData() final override;

	std::string _groupName;
};

template <typename Vm>
JdwpReplyTgrnamePacket<Vm>::JdwpReplyTgrnamePacket()
{
	writeData();
}

template <typename Vm>
JdwpReplyTgrnamePacket<Vm>::~JdwpReplyTgrnamePacket()
{

}

template <typename Vm>
void JdwpReplyTgrnamePacket<Vm>::setGroupName(std::string groupName)
{
	_groupName = groupName;
	writeData();
}

template <typename Vm>
void JdwpReplyTgrnamePacket<Vm>::print(std::ostream &out) const
{
	JdwpReplyPacket::print(out);
	out << "TGR_NAME : ";
	out << "(";
	out << "groupName : ";
	out << _groupName;
	out << ")";
}

template <typename Vm>
void JdwpReplyTgrnamePacket<Vm>::writeData()
{
	typename JdwpTypes<Vm>::String groupName(_groupName);

	_dataBuf = groupName.pack();
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYTGRNAMEPACKET_H_ */
