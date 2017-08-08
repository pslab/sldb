/*
 * JdwpCommandTrresumePacket.hpp
 *
 *  Created on: 2016. 5. 03.
 *      Author: lockpick
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRRESUMEPACKET_HPP_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRRESUMEPACKET_HPP_

#include "JdwpCommandTrresumePacket.h"

template <typename Vm>
JdwpCommandTrresumePacket<Vm>::JdwpCommandTrresumePacket(const JdwpCommandPacket &packet)  : JdwpCommandPacket(packet)
{
	readData();
}

template <typename Vm>
JdwpCommandTrresumePacket<Vm>::~JdwpCommandTrresumePacket()
{

}

template <typename Vm>
void JdwpCommandTrresumePacket<Vm>::print(std::ostream &out) const
{
	JdwpCommandPacket::print(out);
	out << " : (";
	out << "thread : ";
	out << static_cast<std::int64_t>(_thread);
	out << ")";
}

template <typename Vm>
void JdwpCommandTrresumePacket<Vm>::readData()
{
	auto iter = _dataBuf.cbegin();

	::unpack(iter, &_thread);
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRRESUMEPACKET_HPP_ */
