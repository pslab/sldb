/*
 * JdwpCommandTrstatusPacket.h
 *
 *  Created on: 2016. 3. 29.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRSTATUSPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRSTATUSPACKET_H_

#include "../JdwpCommandPacket.h"

#include "../JdwpTypes.h"

/**
 * ThreadReference Command Set (11)
 * Status Command (4)
 */
template <typename Vm>
class JdwpCommandTrstatusPacket final : public JdwpCommandPacket {
public:
	JdwpCommandTrstatusPacket(const JdwpCommandPacket &packet);
	virtual ~JdwpCommandTrstatusPacket();

	typename JdwpTypes<Vm>::ThreadID getThread() const { return _thread; }

	virtual void print(std::ostream &out) const override;

private:
	virtual void readData() final override;

	typename JdwpTypes<Vm>::ThreadID _thread = 0;
};

#include "JdwpCommandTrstatusPacket.hpp"

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRSTATUSPACKET_H_ */
