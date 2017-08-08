/*
 * JdwpReplyRtinterfacesPacket.h
 *
 *  Created on: 2016. 4. 14.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYRTINTERFACESPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYRTINTERFACESPACKET_H_

#include "../JdwpReplyPacket.h"

#include <vector>

#include "../JdwpTypes.h"

/**
 * ReferenceType Command Set (2)
 * Interfaces Command (10)
 */
template <typename Vm>
class JdwpReplyRtinterfacesPacket final : public JdwpReplyPacket {
public:
	JdwpReplyRtinterfacesPacket();
	virtual ~JdwpReplyRtinterfacesPacket();

	void setInterfaceTypes(const std::vector<typename JdwpTypes<Vm>::InterfaceID> &interfaceTypes);
	void pushInterfaceType(typename JdwpTypes<Vm>::InterfaceID interfaceType);

	virtual void print(std::ostream &out) const override;

private:
	virtual void writeData() final override;

	std::vector<typename JdwpTypes<Vm>::InterfaceID> _interfaceTypes;
};

template <typename Vm>
JdwpReplyRtinterfacesPacket<Vm>::JdwpReplyRtinterfacesPacket()
{
	writeData();
}

template <typename Vm>
JdwpReplyRtinterfacesPacket<Vm>::~JdwpReplyRtinterfacesPacket()
{
}

template <typename Vm>
void JdwpReplyRtinterfacesPacket<Vm>::setInterfaceTypes(const std::vector<typename JdwpTypes<Vm>::InterfaceID> &interfaceTypes)
{
	_interfaceTypes = interfaceTypes;
	writeData();
}
template <typename Vm>
void JdwpReplyRtinterfacesPacket<Vm>::pushInterfaceType(typename JdwpTypes<Vm>::InterfaceID interfaceType)
{
	_interfaceTypes.push_back(interfaceType);

	Int interfaces = _interfaceTypes.size();
	::pack(interfaces, _dataBuf.begin());

	auto a = ::pack(interfaceType);
	_dataBuf.insert(_dataBuf.end(), a.cbegin(), a.cend());
}

template <typename Vm>
void JdwpReplyRtinterfacesPacket<Vm>::print(std::ostream &out) const
{
	JdwpReplyPacket::print(out);
	out << " RT_INTERFACES : ";
	out << "(";
	out << "interfaces : ";
	Int interfaces = _interfaceTypes.size();
	out << interfaces;

	for(Int i=0; i<interfaces; i++) {
		out << ", interfaceType" << i << " : ";
		out << static_cast<std::uint64_t>(_interfaceTypes[i]);
	}
	out << ")";
}

template <typename Vm>
void JdwpReplyRtinterfacesPacket<Vm>::writeData()
{
	_dataBuf.clear();

	Int interfaces = _interfaceTypes.size();
	auto a = ::pack(interfaces);
	_dataBuf.insert(_dataBuf.end(), a.cbegin(), a.cend());

	for(Int i=0; i<interfaces; i++) {
		auto interfaceType = ::pack(_interfaceTypes[i]);
		_dataBuf.insert(_dataBuf.end(), interfaceType.cbegin(), interfaceType.cend());
	}
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYRTINTERFACESPACKET_H_ */
