/*
 * JdwpReplyTrnamePacket.h
 *
 *  Created on: 2016. 3. 31.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYTRNAMEPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYTRNAMEPACKET_H_

#include "../JdwpReplyPacket.h"

#include <string>

#include "../JdwpTypes.h"

/**
 * ThreadReference Command Set (11)
 * Name Command (1)
 */
template <typename Vm>
class JdwpReplyTrnamePacket final : public JdwpReplyPacket {
public:
	JdwpReplyTrnamePacket();
	virtual ~JdwpReplyTrnamePacket();

	void setThreadName(std::string threadName);

	virtual void print(std::ostream &out) const override;

private:
	virtual void writeData() final override;

	std::string _threadName;
};

template <typename Vm>
JdwpReplyTrnamePacket<Vm>::JdwpReplyTrnamePacket()
{
	writeData();
}

template <typename Vm>
JdwpReplyTrnamePacket<Vm>::~JdwpReplyTrnamePacket()
{

}

template <typename Vm>
void JdwpReplyTrnamePacket<Vm>::setThreadName(std::string threadName)
{
	_threadName = threadName;
	writeData();
}

template <typename Vm>
void JdwpReplyTrnamePacket<Vm>::print(std::ostream &out) const
{
	JdwpReplyPacket::print(out);
	out << "TR_NAME : ";
	out << "(";
	out << "threadName : ";
	out << _threadName;
	out << ")";
}

template <typename Vm>
void JdwpReplyTrnamePacket<Vm>::writeData()
{
	typename JdwpTypes<Vm>::String threadName(_threadName);

	_dataBuf = threadName.pack();
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYTRNAMEPACKET_H_ */
