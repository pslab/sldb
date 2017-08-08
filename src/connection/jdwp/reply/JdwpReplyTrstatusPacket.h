/*
 * JdwpReplyTrstatusPacket.h
 *
 *  Created on: 2016. 3. 29.
 *      Author: Seunghun Han
 */

#ifndef SRC_CONNECTION_JDWP_REPLY_JDWPREPLYTRSTATUSPACKET_H_
#define SRC_CONNECTION_JDWP_REPLY_JDWPREPLYTRSTATUSPACKET_H_

#include "../JdwpReplyPacket.h"

#include "../JdwpTypes.h"

/**
 * ThreadReference Command Set (11)
 * Status Command (4)
 */
template <typename Vm>
class JdwpReplyTrstatusPacket final : public JdwpReplyPacket {
public:
	JdwpReplyTrstatusPacket();
	virtual ~JdwpReplyTrstatusPacket();

	void setThreadStatus(Int threadStatus);
	void setSuspendStatus(Int suspendStatus);

	virtual void print(std::ostream &out) const override;

private:
	virtual void writeData() final override;

	Int _threadStatus = 0;
	Int _suspendStatus = 0;
};

template <typename Vm>
JdwpReplyTrstatusPacket<Vm>::JdwpReplyTrstatusPacket()
{
	writeData();
}

template <typename Vm>
JdwpReplyTrstatusPacket<Vm>::~JdwpReplyTrstatusPacket()
{

}

template <typename Vm>
void JdwpReplyTrstatusPacket<Vm>::setThreadStatus(Int threadStatus)
{
	_threadStatus = threadStatus;

	auto iter = _dataBuf.begin();
	::pack(_threadStatus, iter);
}

template <typename Vm>
void JdwpReplyTrstatusPacket<Vm>::setSuspendStatus(Int suspendStatus)
{
	_suspendStatus = suspendStatus;
	auto iter = _dataBuf.begin() + sizeof(_threadStatus);
	::pack(_suspendStatus, iter);
}

template <typename Vm>
void JdwpReplyTrstatusPacket<Vm>::print(std::ostream &out) const
{
	JdwpReplyPacket::print(out);
	out << " TR_STATUS : ";
	out << "(";
	out << "threadStatus : ";
	JdwpThreadStatus threadStatus(_threadStatus);
	threadStatus.print(out);
	out << ", suspendStatus : ";
	JdwpSuspendStatus suspendStatus(_suspendStatus);
	suspendStatus.print(out);
	out << ")";
}

template <typename Vm>
void JdwpReplyTrstatusPacket<Vm>::writeData()
{
	_dataBuf.clear();

	auto a = ::pack(_threadStatus);
	_dataBuf.insert(_dataBuf.end(), a.cbegin(), a.cend());

	auto b = ::pack(_suspendStatus);
	_dataBuf.insert(_dataBuf.end(), b.cbegin(), b.cend());
}

#endif /* SRC_CONNECTION_JDWP_REPLY_JDWPREPLYTRSTATUSPACKET_H_ */
