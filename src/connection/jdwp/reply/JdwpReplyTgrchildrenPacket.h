/*
 * JdwpReplyTgrchildrenPacket.h
 *
 *  Created on: 2016. 3. 31.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYTGRCHILDRENPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYTGRCHILDRENPACKET_H_

#include "../JdwpReplyPacket.h"

#include <vector>

#include "../JdwpTypes.h"

/**
 * EventRequest Command Set (15)
 * Children Command (3)
 */
template <typename Vm>
class JdwpReplyTgrchildrenPacket final : public JdwpReplyPacket {
public:
	JdwpReplyTgrchildrenPacket();
	virtual ~JdwpReplyTgrchildrenPacket();

	void setChildThreads(std::vector<typename JdwpTypes<Vm>::ThreadID> childThreads);
	void setChildGroups(std::vector<typename JdwpTypes<Vm>::ThreadGroupID> childGroups);

	virtual void print(std::ostream &out) const override;

private:
	virtual void writeData() final override;

	std::vector<typename JdwpTypes<Vm>::ThreadID> _childThreads;
	std::vector<typename JdwpTypes<Vm>::ThreadGroupID> _childGroups;
};

template <typename Vm>
JdwpReplyTgrchildrenPacket<Vm>::JdwpReplyTgrchildrenPacket()
{
	writeData();
}

template <typename Vm>
JdwpReplyTgrchildrenPacket<Vm>::~JdwpReplyTgrchildrenPacket()
{

}

template <typename Vm>
void JdwpReplyTgrchildrenPacket<Vm>::setChildThreads(std::vector<typename JdwpTypes<Vm>::ThreadID> childThreads)
{
	_childThreads = childThreads;
	writeData();
}

template <typename Vm>
void JdwpReplyTgrchildrenPacket<Vm>::setChildGroups(std::vector<typename JdwpTypes<Vm>::ThreadGroupID> childGroups)
{
	_childGroups = childGroups;
	writeData();
}

template <typename Vm>
void JdwpReplyTgrchildrenPacket<Vm>::print(std::ostream &out) const
{
	JdwpReplyPacket::print(out);
	out << "TGR_CHILDREN : ";
	out << "(";
	out << "childThreads : ";
	Int childThreads = _childThreads.size();
	out << childThreads;

	for(Int i=0; i<childThreads; i++) {
		out << ", childThread" << i << " : ";
		out << static_cast<std::int64_t>(_childThreads[i]);
	}

	out << "childGroups : ";
	Int childGroups = _childGroups.size();
	out << childGroups;

	for(Int i=0; i<childGroups; i++) {
		out << ", childGroup" << i << " : ";
		out << static_cast<std::int64_t>(_childGroups[i]);
	}

	out << ")";
}

template <typename Vm>
void JdwpReplyTgrchildrenPacket<Vm>::writeData()
{
	_dataBuf.clear();

	Int childThreads = _childThreads.size();
	auto a = ::pack(childThreads);
	_dataBuf.insert(_dataBuf.end(), a.cbegin(), a.cend());

	for(Int i = 0; i<childThreads; i++) {
		typename JdwpTypes<Vm>::ThreadID childThread = _childThreads[i];
		auto thread = ::pack(childThread);
		_dataBuf.insert(_dataBuf.end(), thread.cbegin(), thread.cend());
	}

	Int childGroups = _childGroups.size();
	auto b = ::pack(childGroups);
	_dataBuf.insert(_dataBuf.end(), b.cbegin(), b.cend());

	for(Int i = 0; i<childGroups; i++) {
		typename JdwpTypes<Vm>::ThreadGroupID childGroup = _childGroups[i];
		auto group = ::pack(childGroup);
		_dataBuf.insert(_dataBuf.end(), group.cbegin(), group.cend());
	}
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYTGRCHILDRENPACKET_H_ */
