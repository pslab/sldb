/*
 * JdwpCommandTrnamePacket.hpp
 *
 *  Created on: 2016. 3. 29.
 *      Author: lockpick
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRNAMEPACKET_HPP_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRNAMEPACKET_HPP_

#include "JdwpCommandTrnamePacket.h"

template <typename Vm>
JdwpCommandTrnamePacket<Vm>::JdwpCommandTrnamePacket(const JdwpCommandPacket &packet)  : JdwpCommandPacket(packet)
{
	readData();
}

template <typename Vm>
JdwpCommandTrnamePacket<Vm>::~JdwpCommandTrnamePacket()
{

}

template <typename Vm>
void JdwpCommandTrnamePacket<Vm>::print(std::ostream &out) const
{
	JdwpCommandPacket::print(out);
	out << " : (";
	out << "thread : ";
	out << static_cast<std::int64_t>(_thread);
	out << ")";
}

template <typename Vm>
void JdwpCommandTrnamePacket<Vm>::readData()
{
	auto iter = _dataBuf.cbegin();

	::unpack(iter, &_thread);
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRNAMEPACKET_HPP_ */
