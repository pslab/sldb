/*
 * JdwpReplyTrthreadgroupPacket.h
 *
 *  Created on: 2016. 3. 29.
 *      Author: Seunghun Han
 */

#ifndef SRC_CONNECTION_JDWP_REPLY_JDWPREPLYTRTHREADGROUPPACKET_H_
#define SRC_CONNECTION_JDWP_REPLY_JDWPREPLYTRTHREADGROUPPACKET_H_

#include "../JdwpReplyPacket.h"

#include "../JdwpTypes.h"

/**
 * ThreadReference Command Set (11)
 * ThreadGroup Command (5)
 */
template <typename Vm>
class JdwpReplyTrthreadgroupPacket final : public JdwpReplyPacket {
public:
	JdwpReplyTrthreadgroupPacket();
	virtual ~JdwpReplyTrthreadgroupPacket();

	void setGroup(typename JdwpTypes<Vm>::ThreadGroupID group);

	virtual void print(std::ostream &out) const override;

private:
	virtual void writeData() final override;

	typename JdwpTypes<Vm>::ThreadGroupID _group = -1;
};

template <typename Vm>
JdwpReplyTrthreadgroupPacket<Vm>::JdwpReplyTrthreadgroupPacket()
{
	writeData();
}

template <typename Vm>
JdwpReplyTrthreadgroupPacket<Vm>::~JdwpReplyTrthreadgroupPacket()
{

}
template <typename Vm>
void JdwpReplyTrthreadgroupPacket<Vm>::setGroup(typename JdwpTypes<Vm>::ThreadGroupID group)
{
	_group = group;
	writeData();
}

template <typename Vm>
void JdwpReplyTrthreadgroupPacket<Vm>::print(std::ostream &out) const
{
	JdwpReplyPacket::print(out);
	out << " TR_THREAD_GROUP : ";
	out << "(";
	out << "group : ";
	out << static_cast<std::int64_t>(_group);
	out << ")";
}

template <typename Vm>
void JdwpReplyTrthreadgroupPacket<Vm>::writeData()
{
	_dataBuf = ::pack(_group);
}

#endif /* SRC_CONNECTION_JDWP_REPLY_JDWPREPLYTRTHREADGROUPPACKET_H_ */
