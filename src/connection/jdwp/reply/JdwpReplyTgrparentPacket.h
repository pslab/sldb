/*
 * JdwpReplyTgrparentPacket.h
 *
 *  Created on: 2016. 3. 31.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYTGRPARENTPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYTGRPARENTPACKET_H_

#include "../JdwpReplyPacket.h"

#include "../JdwpTypes.h"

/**
 * EventRequest Command Set (15)
 * Parent Command (2)
 */
template <typename Vm>
class JdwpReplyTgrparentPacket final : public JdwpReplyPacket {
public:
	JdwpReplyTgrparentPacket();
	virtual ~JdwpReplyTgrparentPacket();

	void setParentGroup(typename JdwpTypes<Vm>::ThreadGroupID parentGroup);

	virtual void print(std::ostream &out) const override;

private:
	virtual void writeData() final override;

	typename JdwpTypes<Vm>::ThreadGroupID _parentGroup;
};

template <typename Vm>
JdwpReplyTgrparentPacket<Vm>::JdwpReplyTgrparentPacket()
{
	writeData();
}

template <typename Vm>
JdwpReplyTgrparentPacket<Vm>::~JdwpReplyTgrparentPacket()
{

}

template <typename Vm>
void JdwpReplyTgrparentPacket<Vm>::setParentGroup(typename JdwpTypes<Vm>::ThreadGroupID parentGroup)
{
	_parentGroup = parentGroup;
	writeData();
}

template <typename Vm>
void JdwpReplyTgrparentPacket<Vm>::print(std::ostream &out) const
{
	JdwpReplyPacket::print(out);
	out << "TGR_PARENT : ";
	out << "(";
	out << "parentGroup : ";
	out << _parentGroup;
	out << ")";
}

template <typename Vm>
void JdwpReplyTgrparentPacket<Vm>::writeData()
{
	_dataBuf = ::pack(_parentGroup);
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYTGRPARENTPACKET_H_ */
