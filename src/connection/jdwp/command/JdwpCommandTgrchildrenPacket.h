/*
 * JdwpCommandTgrchildrenPacket.h
 *
 *  Created on: 2016. 3. 31.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTGRCHILDRENPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTGRCHILDRENPACKET_H_

#include "../JdwpCommandPacket.h"

#include <vector>

#include "../JdwpTypes.h"

/**
 * ThreadGroupReference Command Set (12)
 * Children Command (3)
 */
template <typename Vm>
class JdwpCommandTgrchildrenPacket final : public JdwpCommandPacket {
public:
	JdwpCommandTgrchildrenPacket(const JdwpCommandPacket &packet);
	virtual ~JdwpCommandTgrchildrenPacket();

	typename JdwpTypes<Vm>::ThreadGroupID getGroup() const { return _group; }

	virtual void print(std::ostream &out) const override;

private:
	virtual void readData() final override;

	typename JdwpTypes<Vm>::ThreadGroupID _group = 0;
};

template <typename Vm>
JdwpCommandTgrchildrenPacket<Vm>::JdwpCommandTgrchildrenPacket(const JdwpCommandPacket &packet) : JdwpCommandPacket(packet)
{
	readData();
}

template <typename Vm>
JdwpCommandTgrchildrenPacket<Vm>::~JdwpCommandTgrchildrenPacket()
{

}

template <typename Vm>
void JdwpCommandTgrchildrenPacket<Vm>::print(std::ostream &out) const
{
	JdwpCommandPacket::print(out);
	out << " : (";
	out << "group : ";
	out << static_cast<std::uint64_t>(_group);
	out << ")";
}

template <typename Vm>
void JdwpCommandTgrchildrenPacket<Vm>::readData()
{
	auto iter = _dataBuf.cbegin();

	::unpack(iter, &_group);
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTGRCHILDRENPACKET_H_ */
