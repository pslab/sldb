/*
 * JdwpCommandTrframesPacket.hpp
 *
 *  Created on: 2016. 3. 29.
 *      Author: lockpick
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRFRAMESPACKET_HPP_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRFRAMESPACKET_HPP_

#include "JdwpCommandTrframesPacket.h"

template <typename Vm>
JdwpCommandTrframesPacket<Vm>::JdwpCommandTrframesPacket(const JdwpCommandPacket &packet)  : JdwpCommandPacket(packet)
{
	readData();
}

template <typename Vm>
JdwpCommandTrframesPacket<Vm>::~JdwpCommandTrframesPacket()
{

}

template <typename Vm>
void JdwpCommandTrframesPacket<Vm>::print(std::ostream &out) const
{
	JdwpCommandPacket::print(out);
	out << " : (";
	out << "thread : ";
	out << static_cast<std::int64_t>(_thread);
	out << ", startFrame : ";
	out << _startFrame;
	out << ", length : ";
	out << _length;
	out << ")";
}

template <typename Vm>
void JdwpCommandTrframesPacket<Vm>::readData()
{
	auto iter = _dataBuf.cbegin();

	::unpack(iter, &_thread);
	iter += sizeof(_thread);

	::unpack(iter, &_startFrame);
	iter += sizeof(_startFrame);

	::unpack(iter, &_length);
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRFRAMESPACKET_HPP_ */
