/*
 * JdwpCommandTrresumePacket.h
 *
 *  Created on: 2016. 5. 03.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRRESUMEPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRRESUMEPACKET_H_

#include "../JdwpCommandPacket.h"

#include "../JdwpTypes.h"

/**
 * ThreadReference Command Set (11)
 * Resume Command (3)
 */
template <typename Vm>
class JdwpCommandTrresumePacket final : public JdwpCommandPacket {
public:
	JdwpCommandTrresumePacket(const JdwpCommandPacket &packet);
	virtual ~JdwpCommandTrresumePacket();

	typename JdwpTypes<Vm>::ThreadID getThread() const { return _thread; }

	virtual void print(std::ostream &out) const override;

private:
	virtual void readData() final override;

	typename JdwpTypes<Vm>::ThreadID _thread = 0;
};

#include "JdwpCommandTrresumePacket.hpp"

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRRESUMEPACKET_H_ */
