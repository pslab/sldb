/*
 * JdwpCommandTrsuspendPacket.h
 *
 *  Created on: 2016. 4. 26.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRSUSPENDPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRSUSPENDPACKET_H_

#include "../JdwpCommandPacket.h"

#include "../JdwpTypes.h"

/**
 * ThreadReference Command Set (11)
 * Suspend Command (2)
 */
template <typename Vm>
class JdwpCommandTrsuspendPacket final : public JdwpCommandPacket {
public:
	JdwpCommandTrsuspendPacket(const JdwpCommandPacket &packet);
	virtual ~JdwpCommandTrsuspendPacket();

	typename JdwpTypes<Vm>::ThreadID getThread() const { return _thread; }

	virtual void print(std::ostream &out) const override;

private:
	virtual void readData() final override;

	typename JdwpTypes<Vm>::ThreadID _thread = 0;
};

#include "JdwpCommandTrsuspendPacket.hpp"

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRSUSPENDPACKET_H_ */
