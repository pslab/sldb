/*
 * JdwpCommandTrsuspendPacket.hpp
 *
 *  Created on: 2016. 4. 26.
 *      Author: lockpick
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRSUSPENDPACKET_HPP_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRSUSPENDPACKET_HPP_

#include "JdwpCommandTrsuspendPacket.h"

template <typename Vm>
JdwpCommandTrsuspendPacket<Vm>::JdwpCommandTrsuspendPacket(const JdwpCommandPacket &packet)  : JdwpCommandPacket(packet)
{
	readData();
}

template <typename Vm>
JdwpCommandTrsuspendPacket<Vm>::~JdwpCommandTrsuspendPacket()
{

}

template <typename Vm>
void JdwpCommandTrsuspendPacket<Vm>::print(std::ostream &out) const
{
	JdwpCommandPacket::print(out);
	out << " : (";
	out << "thread : ";
	out << static_cast<std::int64_t>(_thread);
	out << ")";
}

template <typename Vm>
void JdwpCommandTrsuspendPacket<Vm>::readData()
{
	auto iter = _dataBuf.cbegin();

	::unpack(iter, &_thread);
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRSUSPENDPACKET_HPP_ */
