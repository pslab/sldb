/*
 * JdwpReplyErsetPacket.h
 *
 *  Created on: 2016. 3. 9.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYERSETPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYERSETPACKET_H_

#include "../JdwpReplyPacket.h"

#include "../JdwpTypes.h"

/**
 * EventRequest Command Set (15)
 * Set Command (1)
 */
template <typename Vm>
class JdwpReplyErsetPacket final : public JdwpReplyPacket {
public:
	JdwpReplyErsetPacket();
	virtual ~JdwpReplyErsetPacket();

	virtual void setRequestID(Int requestID);
	virtual void print(std::ostream &out) const override;

private:
	virtual void writeData() final override;

	Int _requestID = 0;
};

template <typename Vm>
JdwpReplyErsetPacket<Vm>::JdwpReplyErsetPacket()
{
	Int id = 0;
	_dataBuf = ::pack(id);
}

template <typename Vm>
JdwpReplyErsetPacket<Vm>::~JdwpReplyErsetPacket()
{

}

template <typename Vm>
void JdwpReplyErsetPacket<Vm>::setRequestID(Int requestID)
{
	_requestID = requestID;
	writeData();
}

template <typename Vm>
void JdwpReplyErsetPacket<Vm>::print(std::ostream &out) const
{
	JdwpReplyPacket::print(out);
	out << " ER_SET : ";
	out << "(";
	out << "requestID : ";
	out << _requestID;
	out << ")";
}

template <typename Vm>
void JdwpReplyErsetPacket<Vm>::writeData()
{
	_dataBuf = ::pack(_requestID);
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYERSETPACKET_H_ */
