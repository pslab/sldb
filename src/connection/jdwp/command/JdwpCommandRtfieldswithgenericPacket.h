/*
 * JdwpCommandRtfieldswithgenericPacket.h
 *
 *  Created on: 2016. 4. 14.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDRTFIELDSWITHGENERICPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDRTFIELDSWITHGENERICPACKET_H_

#include "../JdwpCommandPacket.h"

#include "../JdwpTypes.h"

/**
 * ReferenceType Command Set (2)
 * FieldsWithGeneric Command (14)
 */
template <typename Vm>
class JdwpCommandRtfieldswithgenericPacket final : public JdwpCommandPacket {
public:
	JdwpCommandRtfieldswithgenericPacket(const JdwpCommandPacket &packet);
	virtual ~JdwpCommandRtfieldswithgenericPacket();

	typename JdwpTypes<Vm>::ReferenceTypeID getRefType() const { return _refType; }

	virtual void print(std::ostream &out) const override;

private:
	virtual void readData() final override;

	typename JdwpTypes<Vm>::ReferenceTypeID _refType = 0;
};

template <typename Vm>
JdwpCommandRtfieldswithgenericPacket<Vm>::JdwpCommandRtfieldswithgenericPacket(const JdwpCommandPacket &packet) : JdwpCommandPacket(packet)
{
	readData();
}

template <typename Vm>
JdwpCommandRtfieldswithgenericPacket<Vm>::~JdwpCommandRtfieldswithgenericPacket()
{

}

template <typename Vm>
void JdwpCommandRtfieldswithgenericPacket<Vm>::print(std::ostream &out) const
{
	JdwpCommandPacket::print(out);
	out << " : (";
	out << "refType : ";
	out << static_cast<std::uint64_t>(_refType);
	out << ")";
}

template <typename Vm>
void JdwpCommandRtfieldswithgenericPacket<Vm>::readData()
{
	auto iter = _dataBuf.cbegin();

	::unpack(iter, &_refType);
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDRTFIELDSWITHGENERICPACKET_H_ */
