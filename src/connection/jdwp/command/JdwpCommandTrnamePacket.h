/*
 * JdwpCommandTrnamePacket.h
 *
 *  Created on: 2016. 3. 29.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRNAMEPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRNAMEPACKET_H_

#include "../JdwpCommandPacket.h"

#include "../JdwpTypes.h"

/**
 * ThreadReference Command Set (11)
 * Name Command (1)
 */
template <typename Vm>
class JdwpCommandTrnamePacket final : public JdwpCommandPacket {
public:
	JdwpCommandTrnamePacket(const JdwpCommandPacket &packet);
	virtual ~JdwpCommandTrnamePacket();

	typename JdwpTypes<Vm>::ThreadID getThread() const { return _thread; }

	virtual void print(std::ostream &out) const override;

private:
	virtual void readData() final override;

	typename JdwpTypes<Vm>::ThreadID _thread = 0;
};

#include "JdwpCommandTrnamePacket.hpp"

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRNAMEPACKET_H_ */
