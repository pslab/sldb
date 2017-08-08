/*
 * JdwpCommandTrthreadgroupPacket.hpp
 *
 *  Created on: 2016. 3. 29.
 *      Author: lockpick
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRTHREADGROUPPACKET_HPP_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRTHREADGROUPPACKET_HPP_

#include "JdwpCommandTrthreadgroupPacket.h"

template <typename Vm>
JdwpCommandTrthreadgroupPacket<Vm>::JdwpCommandTrthreadgroupPacket(const JdwpCommandPacket &packet)  : JdwpCommandPacket(packet)
{
	readData();
}

template <typename Vm>
JdwpCommandTrthreadgroupPacket<Vm>::~JdwpCommandTrthreadgroupPacket()
{

}

template <typename Vm>
void JdwpCommandTrthreadgroupPacket<Vm>::print(std::ostream &out) const
{
	JdwpCommandPacket::print(out);
	out << " : (";
	out << "thread : ";
	out << static_cast<std::int64_t>(_thread);
	out << ")";
}

template <typename Vm>
void JdwpCommandTrthreadgroupPacket<Vm>::readData()
{
	auto iter = _dataBuf.cbegin();

	::unpack(iter, &_thread);
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRTHREADGROUPPACKET_HPP_ */
