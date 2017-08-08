/*
 * JdwpCommandCtsuperclassPacket.h
 *
 *  Created on: 2016. 4. 14.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDCTSUPERCLASSPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDCTSUPERCLASSPACKET_H_

#include "../JdwpCommandPacket.h"


#include "../JdwpTypes.h"

/**
 * ClassType Command Set (3)
 * Superclass Command (1)
 */
template <typename Vm>
class JdwpCommandCtsuperclassPacket final : public JdwpCommandPacket {
public:
	JdwpCommandCtsuperclassPacket(const JdwpCommandPacket &packet);
	virtual ~JdwpCommandCtsuperclassPacket();

	typename JdwpTypes<Vm>::ClassID getClazz() const { return _clazz; }

	virtual void print(std::ostream &out) const override;

private:
	virtual void readData() final override;

	typename JdwpTypes<Vm>::ClassID _clazz = 0;
};

template <typename Vm>
JdwpCommandCtsuperclassPacket<Vm>::JdwpCommandCtsuperclassPacket(const JdwpCommandPacket &packet) : JdwpCommandPacket(packet)
{
	readData();
}

template <typename Vm>
JdwpCommandCtsuperclassPacket<Vm>::~JdwpCommandCtsuperclassPacket()
{

}

template <typename Vm>
void JdwpCommandCtsuperclassPacket<Vm>::print(std::ostream &out) const
{
	JdwpCommandPacket::print(out);
	out << " : (";
	out << "clazz : ";
	out << static_cast<std::uint64_t>(_clazz);
	out << ")";
}

template <typename Vm>
void JdwpCommandCtsuperclassPacket<Vm>::readData()
{
	auto iter = _dataBuf.cbegin();

	::unpack(iter, &_clazz);
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDCTSUPERCLASSPACKET_H_ */
