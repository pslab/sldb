/*
 * JdwpReplyVmidsizesPacket.h
 *
 *  Created on: 2016. 3. 8.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYVMVERSIONPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYVMVERSIONPACKET_H_

#include "../JdwpReplyPacket.h"

#include "../JdwpTypes.h"

/**
 * VirtualMachine Command Set (1)
 * IDSizes Command (7)
 */
template <typename Vm>
class JdwpReplyVmversionPacket final: public JdwpReplyPacket {
public:
	static const Int JDWP_MAJOR = 7;
	static const Int JDWP_MINOR = 0;

	JdwpReplyVmversionPacket();
	virtual ~JdwpReplyVmversionPacket();

	virtual void print(std::ostream &out) const override;

private:
	virtual void writeData() final override;
};

template <typename Vm>
JdwpReplyVmversionPacket<Vm>::JdwpReplyVmversionPacket()
{
	writeData();
}

template <typename Vm>
JdwpReplyVmversionPacket<Vm>::~JdwpReplyVmversionPacket()
{
}

template <typename Vm>
void JdwpReplyVmversionPacket<Vm>::print(std::ostream &out) const
{
	JdwpReplyPacket::print(out);
	out << " VM_VERSION : ";
	out << "(";
	out << "description : ";
	out << Vm::DESCRIPTION;
	out << ", jdwpMajor : ";
	out << JDWP_MAJOR;
	out << ", jdwpMinor : ";
	out << JDWP_MINOR;
	out << ", vmVersion : ";
	out << Vm::VERSION;
	out << ", vmName : ";
	out << Vm::NAME;
	out << ")";
}

template <typename Vm>
void JdwpReplyVmversionPacket<Vm>::writeData()
{
	_dataBuf.clear();

	typename JdwpTypes<Vm>::String description = Vm::DESCRIPTION;
	auto a = description.pack();
	_dataBuf.insert(_dataBuf.end(), a.cbegin(), a.cend());

	Int jdwpMajor = JDWP_MAJOR;
	auto b = ::pack(jdwpMajor);
	_dataBuf.insert(_dataBuf.end(), b.cbegin(), b.cend());

	Int jdwpMinor = JDWP_MINOR;
	auto c = ::pack(jdwpMinor);
	_dataBuf.insert(_dataBuf.end(), c.cbegin(), c.cend());

	typename JdwpTypes<Vm>::String version = Vm::VERSION;
	auto d = version.pack();
	_dataBuf.insert(_dataBuf.end(), d.cbegin(), d.cend());

	typename JdwpTypes<Vm>::String name = Vm::NAME;
	auto e = name.pack();
	_dataBuf.insert(_dataBuf.end(), e.cbegin(),e.cend());

}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYVMVERSIONPACKET_H_ */
