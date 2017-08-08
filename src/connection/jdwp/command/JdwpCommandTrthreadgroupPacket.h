/*
 * JdwpCommandTrthreadgroupPacket.h
 *
 *  Created on: 2016. 3. 29.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRTHREADGROUPPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRTHREADGROUPPACKET_H_

#include "../JdwpCommandPacket.h"

#include "../JdwpTypes.h"

/**
 * ThreadReference Command Set (11)
 * ThreadGroup Command (5)
 */
template <typename Vm>
class JdwpCommandTrthreadgroupPacket final : public JdwpCommandPacket {
public:
	JdwpCommandTrthreadgroupPacket(const JdwpCommandPacket &packet);
	virtual ~JdwpCommandTrthreadgroupPacket();

	typename JdwpTypes<Vm>::ThreadID getThread() const { return _thread; }

	virtual void print(std::ostream &out) const override;

private:
	virtual void readData() final override;

	typename JdwpTypes<Vm>::ThreadID _thread = 0;
};

#include "JdwpCommandTrthreadgroupPacket.hpp"

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRTHREADGROUPPACKET_H_ */
