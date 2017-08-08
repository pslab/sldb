/*
 * JdwpReplyVmallthreadsPacket.h
 *
 *  Created on: 2016. 3. 10.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYVMALLTHREADSPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYVMALLTHREADSPACKET_H_

#include "../JdwpReplyPacket.h"

#include <vector>

#include "../JdwpTypes.h"

/**
 * VirtualMachine Command Set (1)
 * AllThreads Command (4)
 */
template <typename Vm>
class JdwpReplyVmallthreadsPacket final : public JdwpReplyPacket {
public:
	JdwpReplyVmallthreadsPacket();
	virtual ~JdwpReplyVmallthreadsPacket();

	void setThread(const std::vector<typename JdwpTypes<Vm>::ThreadID> &thread);

	virtual void print(std::ostream &out) const override;

private:
	virtual void writeData() final override;

	std::vector<typename JdwpTypes<Vm>::ThreadID> _thread;
};

template <typename Vm>
JdwpReplyVmallthreadsPacket<Vm>::JdwpReplyVmallthreadsPacket()
{
	Int threads = _thread.size();
	_dataBuf = ::pack(threads);
}

template <typename Vm>
JdwpReplyVmallthreadsPacket<Vm>::~JdwpReplyVmallthreadsPacket()
{
}

template <typename Vm>
void JdwpReplyVmallthreadsPacket<Vm>::setThread(const std::vector<typename JdwpTypes<Vm>::ThreadID> &thread)
{
	_thread = thread;
	writeData();
}

template <typename Vm>
void JdwpReplyVmallthreadsPacket<Vm>::print(std::ostream &out) const
{
	JdwpReplyPacket::print(out);
	out << " VM_ALL_THREADS : ";
	out << "(";
	out << "threads : ";
	Int threads = _thread.size();
	out << threads;

	for(Int i=0; i<threads; i++) {
		out << ", thread" << i << " : ";
		out << static_cast<std::int64_t>(_thread[i]);
	}
	out << ")";
}

template <typename Vm>
void JdwpReplyVmallthreadsPacket<Vm>::writeData()
{
	_dataBuf.clear();

	Int threads = _thread.size();
	auto a = ::pack(threads);
	_dataBuf.insert(_dataBuf.end(), a.cbegin(), a.cend());

	for(Int i=0; i<threads; i++) {
		auto thread = ::pack(_thread[i]);
		_dataBuf.insert(_dataBuf.end(), thread.cbegin(), thread.cend());
	}
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYVMALLTHREADSPACKET_H_ */
