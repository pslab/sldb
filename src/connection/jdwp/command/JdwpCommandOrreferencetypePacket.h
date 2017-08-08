/*
 * JdwpCommandOrreferencetypePacket.h
 *
 *  Created on: 2016. 3. 31.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDORREFERENCETYPEPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDORREFERENCETYPEPACKET_H_

#include "../JdwpCommandPacket.h"

#include <vector>

#include "../JdwpTypes.h"

/**
 * ThreadGroupReference Command Set (12)
 * Name Command (1)
 */
template <typename Vm>
class JdwpCommandOrreferencetypePacket final : public JdwpCommandPacket {
public:
	JdwpCommandOrreferencetypePacket(const JdwpCommandPacket &packet);
	virtual ~JdwpCommandOrreferencetypePacket();

	typename JdwpTypes<Vm>::ObjectID getObject() const { return _object; }

	virtual void print(std::ostream &out) const override;

private:
	virtual void readData() final override;

	typename JdwpTypes<Vm>::ObjectID _object = 0;
};

template <typename Vm>
JdwpCommandOrreferencetypePacket<Vm>::JdwpCommandOrreferencetypePacket(const JdwpCommandPacket &packet) : JdwpCommandPacket(packet)
{
	readData();
}

template <typename Vm>
JdwpCommandOrreferencetypePacket<Vm>::~JdwpCommandOrreferencetypePacket()
{

}

template <typename Vm>
void JdwpCommandOrreferencetypePacket<Vm>::print(std::ostream &out) const
{
	JdwpCommandPacket::print(out);
	out << " : (";
	out << "object : ";
	out << static_cast<std::uint64_t>(_object);
	out << ")";
}

template <typename Vm>
void JdwpCommandOrreferencetypePacket<Vm>::readData()
{
	auto iter = _dataBuf.cbegin();

	::unpack(iter, &_object);
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDORREFERENCETYPEPACKET_H_ */
