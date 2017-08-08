/*
 * JdwpCommandTgrparentPacket.h
 *
 *  Created on: 2016. 3. 31.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTGRPARENTPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTGRPARENTPACKET_H_

#include "../JdwpCommandPacket.h"

#include <vector>

#include "../JdwpTypes.h"

/**
 * ThreadGroupReference Command Set (12)
 * Parent Command (2)
 */
template <typename Vm>
class JdwpCommandTgrparentPacket final : public JdwpCommandPacket {
public:
	JdwpCommandTgrparentPacket(const JdwpCommandPacket &packet);
	virtual ~JdwpCommandTgrparentPacket();

	typename JdwpTypes<Vm>::ThreadGroupID getGroup() const { return _group; }

	virtual void print(std::ostream &out) const override;

private:
	virtual void readData() final override;

	typename JdwpTypes<Vm>::ThreadGroupID _group = 0;
};

template <typename Vm>
JdwpCommandTgrparentPacket<Vm>::JdwpCommandTgrparentPacket(const JdwpCommandPacket &packet) : JdwpCommandPacket(packet)
{
	readData();
}

template <typename Vm>
JdwpCommandTgrparentPacket<Vm>::~JdwpCommandTgrparentPacket()
{

}

template <typename Vm>
void JdwpCommandTgrparentPacket<Vm>::print(std::ostream &out) const
{
	JdwpCommandPacket::print(out);
	out << " : (";
	out << "group : ";
	out << static_cast<std::uint64_t>(_group);
	out << ")";
}

template <typename Vm>
void JdwpCommandTgrparentPacket<Vm>::readData()
{
	auto iter = _dataBuf.cbegin();

	::unpack(iter, &_group);
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTGRPARENTPACKET_H_ */
