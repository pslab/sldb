/*
 * JdwpReplyCtsuperclassPacket.h
 *
 *  Created on: 2016. 4. 14.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYCTSUPERCLASSPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYCTSUPERCLASSPACKET_H_

#include "../JdwpReplyPacket.h"

#include "../JdwpTypes.h"

/**
 * ClassType Command Set (3)
 * Superclass Command (1)
 */
template <typename Vm>
class JdwpReplyCtsuperclassPacket final : public JdwpReplyPacket {
public:
	JdwpReplyCtsuperclassPacket();
	virtual ~JdwpReplyCtsuperclassPacket();

	void setSuperclass(typename JdwpTypes<Vm>::ClassID superclass);

	virtual void print(std::ostream &out) const override;

private:
	virtual void writeData() final override;

	typename JdwpTypes<Vm>::ClassID _superclass = 0;
};

template <typename Vm>
JdwpReplyCtsuperclassPacket<Vm>::JdwpReplyCtsuperclassPacket()
{
	writeData();
}

template <typename Vm>
JdwpReplyCtsuperclassPacket<Vm>::~JdwpReplyCtsuperclassPacket()
{

}

template <typename Vm>
void JdwpReplyCtsuperclassPacket<Vm>::setSuperclass(typename JdwpTypes<Vm>::ClassID superclass)
{
	_superclass = superclass;
	writeData();
}

template <typename Vm>
void JdwpReplyCtsuperclassPacket<Vm>::print(std::ostream &out) const
{
	JdwpReplyPacket::print(out);
	out << "CT_SUPERCLASS : ";
	out << "(superclass : ";
	out << static_cast<std::uint64_t>(_superclass);
	out << ")";
}

template <typename Vm>
void JdwpReplyCtsuperclassPacket<Vm>::writeData()
{
	_dataBuf = ::pack(_superclass);
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYCTSUPERCLASSPACKET_H_ */
