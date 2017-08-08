/*
 * JdwpCommandTrframesPacket.h
 *
 *  Created on: 2016. 3. 29.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRFRAMESPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRFRAMESPACKET_H_

#include "../JdwpCommandPacket.h"

#include "../JdwpTypes.h"

/**
 * ThreadReference Command Set (11)
 * Frames Command (6)
 */
template <typename Vm>
class JdwpCommandTrframesPacket final : public JdwpCommandPacket {
public:
	JdwpCommandTrframesPacket(const JdwpCommandPacket &packet);
	virtual ~JdwpCommandTrframesPacket();

	typename JdwpTypes<Vm>::ThreadID getThread() const { return _thread; }
	Int getStartFrame() const { return _startFrame; }
	Int getLength() const { return _length; }

	virtual void print(std::ostream &out) const override;

private:
	virtual void readData() final override;

	typename JdwpTypes<Vm>::ThreadID _thread = 0;
	Int _startFrame = 0;
	Int _length = 0;
};

#include "JdwpCommandTrframesPacket.hpp"

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDTRFRAMESPACKET_H_ */
