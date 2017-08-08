/*
 * JdwpCommandVmclassesbysignaturePacket.h
 *
 *  Created on: 2016. 5. 10.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDVMCLASSESBYSIGNATUREPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDVMCLASSESBYSIGNATUREPACKET_H_

#include "../JdwpCommandPacket.h"

#include <vector>

#include "../JdwpTypes.h"

/**
 * VirtualMachine Command Set (1)
 * ClassesBySignature Command (2)
 */
template <typename Vm>
class JdwpCommandVmclassesbysignaturePacket final : public JdwpCommandPacket {
public:
	JdwpCommandVmclassesbysignaturePacket(const JdwpCommandPacket &packet);
	virtual ~JdwpCommandVmclassesbysignaturePacket();

	std::string getSignature() const { return _signature; }

	virtual void print(std::ostream &out) const override;

private:
	virtual void readData() final override;

	std::string _signature;
};

template <typename Vm>
JdwpCommandVmclassesbysignaturePacket<Vm>::JdwpCommandVmclassesbysignaturePacket(const JdwpCommandPacket &packet) : JdwpCommandPacket(packet)
{
	readData();
}

template <typename Vm>
JdwpCommandVmclassesbysignaturePacket<Vm>::~JdwpCommandVmclassesbysignaturePacket()
{

}

template <typename Vm>
void JdwpCommandVmclassesbysignaturePacket<Vm>::print(std::ostream &out) const
{
	JdwpCommandPacket::print(out);
	out << " : (";
	out << "signature : ";
	out << _signature;
	out << ")";
}

template <typename Vm>
void JdwpCommandVmclassesbysignaturePacket<Vm>::readData()
{
	auto iter = _dataBuf.cbegin();

	auto string = JdwpTypes<Vm>::String::unpack(iter);
	_signature = string.get();
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDVMCLASSESBYSIGNATUREPACKET_H_ */
