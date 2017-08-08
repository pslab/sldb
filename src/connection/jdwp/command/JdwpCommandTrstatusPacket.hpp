/*
 * JdwpCommandTrstatusPacket.hpp
 *
 *  Created on: 2016. 3. 29.
 *      Author: lockpick
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRSTATUSPACKET_HPP_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRSTATUSPACKET_HPP_

#include "JdwpCommandTrstatusPacket.h"

template <typename Vm>
JdwpCommandTrstatusPacket<Vm>::JdwpCommandTrstatusPacket(const JdwpCommandPacket &packet)  : JdwpCommandPacket(packet)
{
	readData();
}

template <typename Vm>
JdwpCommandTrstatusPacket<Vm>::~JdwpCommandTrstatusPacket()
{

}

template <typename Vm>
void JdwpCommandTrstatusPacket<Vm>::print(std::ostream &out) const
{
	JdwpCommandPacket::print(out);
	out << " : (";
	out << "thread : ";
	out << static_cast<std::int64_t>(_thread);
	out << ")";
}

template <typename Vm>
void JdwpCommandTrstatusPacket<Vm>::readData()
{
	auto iter = _dataBuf.cbegin();

	::unpack(iter, &_thread);
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRSTATUSPACKET_HPP_ */
