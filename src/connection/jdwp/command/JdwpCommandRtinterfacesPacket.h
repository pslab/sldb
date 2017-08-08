/*
 * JdwpCommandRtinterfacesPacket.h
 *
 *  Created on: 2016. 4. 14.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDRTINTERFACESPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDRTINTERFACESPACKET_H_

#include "../JdwpCommandPacket.h"


#include "../JdwpTypes.h"

/**
 * ReferenceType Command Set (2)
 * Interfaces Command (10)
 */
template <typename Vm>
class JdwpCommandRtinterfacesPacket final : public JdwpCommandPacket {
public:
	JdwpCommandRtinterfacesPacket(const JdwpCommandPacket &packet);
	virtual ~JdwpCommandRtinterfacesPacket();

	typename JdwpTypes<Vm>::ReferenceTypeID getRefType() const { return _refType; }

	virtual void print(std::ostream &out) const override;

private:
	virtual void readData() final override;

	typename JdwpTypes<Vm>::ReferenceTypeID _refType = 0;
};

template <typename Vm>
JdwpCommandRtinterfacesPacket<Vm>::JdwpCommandRtinterfacesPacket(const JdwpCommandPacket &packet) : JdwpCommandPacket(packet)
{
	readData();
}

template <typename Vm>
JdwpCommandRtinterfacesPacket<Vm>::~JdwpCommandRtinterfacesPacket()
{

}

template <typename Vm>
void JdwpCommandRtinterfacesPacket<Vm>::print(std::ostream &out) const
{
	JdwpCommandPacket::print(out);
	out << " : (";
	out << "refType : ";
	out << static_cast<std::uint64_t>(_refType);
	out << ")";
}

template <typename Vm>
void JdwpCommandRtinterfacesPacket<Vm>::readData()
{
	auto iter = _dataBuf.cbegin();

	::unpack(iter, &_refType);
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDRTINTERFACESPACKET_H_ */
