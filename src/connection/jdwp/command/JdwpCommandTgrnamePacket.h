/*
 * JdwpCommandTgrnamePacket.h
 *
 *  Created on: 2016. 3. 31.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTGRNAMEPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTGRNAMEPACKET_H_

#include "../JdwpCommandPacket.h"

#include <vector>

#include "../JdwpTypes.h"

/**
 * ThreadGroupReference Command Set (12)
 * Name Command (1)
 */
template <typename Vm>
class JdwpCommandTgrnamePacket final : public JdwpCommandPacket {
public:
	JdwpCommandTgrnamePacket(const JdwpCommandPacket &packet);
	virtual ~JdwpCommandTgrnamePacket();

	typename JdwpTypes<Vm>::ThreadGroupID getGroup() const { return _group; }

	virtual void print(std::ostream &out) const override;

private:
	virtual void readData() final override;

	typename JdwpTypes<Vm>::ThreadGroupID _group = 0;
};

template <typename Vm>
JdwpCommandTgrnamePacket<Vm>::JdwpCommandTgrnamePacket(const JdwpCommandPacket &packet) : JdwpCommandPacket(packet)
{
	readData();
}

template <typename Vm>
JdwpCommandTgrnamePacket<Vm>::~JdwpCommandTgrnamePacket()
{

}

template <typename Vm>
void JdwpCommandTgrnamePacket<Vm>::print(std::ostream &out) const
{
	JdwpCommandPacket::print(out);
	out << " : (";
	out << "group : ";
	out << static_cast<std::uint64_t>(_group);
	out << ")";
}

template <typename Vm>
void JdwpCommandTgrnamePacket<Vm>::readData()
{
	auto iter = _dataBuf.cbegin();

	::unpack(iter, &_group);
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTGRNAMEPACKET_H_ */
