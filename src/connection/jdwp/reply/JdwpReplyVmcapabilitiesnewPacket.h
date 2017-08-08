/*
 * JdwpReplyVmcapabilitiesnewPacket.h
 *
 *  Created on: 2016. 4. 14.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYVMCAPABILITIESNEWPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYVMCAPABILITIESNEWPACKET_H_

#include "../JdwpReplyPacket.h"

#include "../JdwpTypes.h"

/**
 * VirtualMachine Command Set (1)
 * CapabilitiesNew Command (17)
 */
template <typename Vm>
class JdwpReplyVmcapabilitiesnewPacket final: public JdwpReplyPacket {
public:
	JdwpReplyVmcapabilitiesnewPacket();
	virtual ~JdwpReplyVmcapabilitiesnewPacket();

	virtual void print(std::ostream &out) const override;

private:
	virtual void writeData() final override;
};

template <typename Vm>
JdwpReplyVmcapabilitiesnewPacket<Vm>::JdwpReplyVmcapabilitiesnewPacket()
{
	writeData();
}

template <typename Vm>
JdwpReplyVmcapabilitiesnewPacket<Vm>::~JdwpReplyVmcapabilitiesnewPacket()
{
}

template <typename Vm>
void JdwpReplyVmcapabilitiesnewPacket<Vm>::print(std::ostream &out) const
{
	JdwpReplyPacket::print(out);
	out << " VM_CAPABILITIES_NEW : ";
	out << "(";
	out << "canWatchFieldModification : ";
	out << Vm::canWatchFieldModification;
	out << ", canWatchFieldAccess : ";
	out << Vm::canWatchFieldAccess;
	out << ", canGetBytecodes : ";
	out << Vm::canGetBytecodes;
	out << ", canGetSyntheticAttribute : ";
	out << Vm::canGetSyntheticAttribute;
	out << ", canGetOwnedMonitorInfo : ";
	out << Vm::canGetOwnedMonitorInfo;
	out << ", canGetCurrentContendedMonitor : ";
	out << Vm::canGetCurrentContendedMonitor;
	out << ", canGetMonitorInfo : ";
	out << Vm::canGetMonitorInfo;
	out << ", canRedefineClasses : ";
	out << Vm::canRedefineClasses;
	out << ", canAddMethod : ";
	out << Vm::canAddMethod;
	out << ", canUnrestrictedlyRedefineClasses : ";
	out << Vm::canUnrestrictedlyRedefineClasses;
	out << ", canPopFrames : ";
	out << Vm::canPopFrames;
	out << ", canUseInstanceFilters : ";
	out << Vm::canUseInstanceFilters;
	out << ", canGetSourceDebugExtension : ";
	out << Vm::canGetSourceDebugExtension;
	out << ", canRequestVMDeathEvent : ";
	out << Vm::canRequestVMDeathEvent;
	out << ", canSetDefaultStratum : ";
	out << Vm::canSetDefaultStratum;
	out << ", canGetInstanceInfo : ";
	out << Vm::canGetInstanceInfo;
	out << ", canRequestMonitorEvents : ";
	out << Vm::canRequestMonitorEvents;
	out << ", canGetMonitorFrameInfo : ";
	out << Vm::canGetMonitorFrameInfo;
	out << ", canUseSourceNameFilters : ";
	out << Vm::canUseSourceNameFilters;
	out << ", canGetConstantPool : ";
	out << Vm::canGetConstantPool;
	out << ", canForceEarlyReturn : ";
	out << Vm::canForceEarlyReturn;
	out << ", reserved22 : ";
	out << false;
	out << ", reserved23 : ";
	out << false;
	out << ", reserved24 : ";
	out << false;
	out << ", reserved25 : ";
	out << false;
	out << ", reserved26 : ";
	out << false;
	out << ", reserved27 : ";
	out << false;
	out << ", reserved28 : ";
	out << false;
	out << ", reserved29 : ";
	out << false;
	out << ", reserved30 : ";
	out << false;
	out << ", reserved31 : ";
	out << false;
	out << ", reserved32 : ";
	out << false;
	out << ")";
}

template <typename Vm>
void JdwpReplyVmcapabilitiesnewPacket<Vm>::writeData()
{
	_dataBuf.resize(sizeof(Boolean)*32);
	auto iter = _dataBuf.begin();

	Boolean canWatchFieldModification = Vm::canWatchFieldModification;
	::pack(canWatchFieldModification, iter);
	iter += sizeof(canWatchFieldModification);

	Boolean canWatchFieldAccess = Vm::canWatchFieldAccess;
	::pack(canWatchFieldAccess, iter);
	iter += sizeof(canWatchFieldAccess);

	Boolean canGetBytecodes = Vm::canGetBytecodes;
	::pack(canGetBytecodes, iter);
	iter += sizeof(canGetBytecodes);

	Boolean canGetSyntheticAttribute = Vm::canGetSyntheticAttribute;
	::pack(canGetSyntheticAttribute, iter);
	iter += sizeof(canGetSyntheticAttribute);

	Boolean canGetOwnedMonitorInfo = Vm::canGetOwnedMonitorInfo;
	::pack(canGetOwnedMonitorInfo, iter);
	iter += sizeof(canGetOwnedMonitorInfo);

	Boolean canGetCurrentContendedMonitor = Vm::canGetCurrentContendedMonitor;
	::pack(canGetCurrentContendedMonitor, iter);
	iter += sizeof(canGetCurrentContendedMonitor);

	Boolean canGetMonitorInfo = Vm::canGetMonitorInfo;
	::pack(canGetMonitorInfo, iter);
	iter += sizeof(canGetMonitorInfo);

	Boolean canRedefineClasses = Vm::canRedefineClasses;
	::pack(canRedefineClasses, iter);
	iter += sizeof(canRedefineClasses);

	Boolean canAddMethod = Vm::canAddMethod;
	::pack(canAddMethod, iter);
	iter += sizeof(canAddMethod);

	Boolean canUnrestrictedlyRedefineClasses = Vm::canUnrestrictedlyRedefineClasses;
	::pack(canUnrestrictedlyRedefineClasses, iter);
	iter += sizeof(canUnrestrictedlyRedefineClasses);

	Boolean canPopFrames = Vm::canPopFrames;
	::pack(canPopFrames, iter);
	iter += sizeof(canPopFrames);

	Boolean canUseInstanceFilters = Vm::canUseInstanceFilters;
	::pack(canUseInstanceFilters, iter);
	iter += sizeof(canUseInstanceFilters);

	Boolean canGetSourceDebugExtension = Vm::canGetSourceDebugExtension;
	::pack(canGetSourceDebugExtension, iter);
	iter += sizeof(canGetSourceDebugExtension);

	Boolean canRequestVMDeathEvent = Vm::canRequestVMDeathEvent;
	::pack(canRequestVMDeathEvent, iter);
	iter += sizeof(canRequestVMDeathEvent);

	Boolean canSetDefaultStratum = Vm::canSetDefaultStratum;
	::pack(canSetDefaultStratum, iter);
	iter += sizeof(canSetDefaultStratum);

	Boolean canGetInstanceInfo = Vm::canGetInstanceInfo;
	::pack(canGetInstanceInfo, iter);
	iter += sizeof(canGetInstanceInfo);

	Boolean canRequestMonitorEvents = Vm::canRequestMonitorEvents;
	::pack(canRequestMonitorEvents, iter);
	iter += sizeof(canRequestMonitorEvents);

	Boolean canGetMonitorFrameInfo = Vm::canGetMonitorFrameInfo;
	::pack(canGetMonitorFrameInfo, iter);
	iter += sizeof(canGetMonitorFrameInfo);

	Boolean canUseSourceNameFilters = Vm::canUseSourceNameFilters;
	::pack(canUseSourceNameFilters, iter);
	iter += sizeof(canUseSourceNameFilters);

	Boolean canGetConstantPool = Vm::canGetConstantPool;
	::pack(canGetConstantPool, iter);
	iter += sizeof(canGetConstantPool);

	Boolean canForceEarlyReturn = Vm::canForceEarlyReturn;
	::pack(canForceEarlyReturn, iter);
	iter += sizeof(canForceEarlyReturn);

	Boolean reserved22 = false;
	::pack(reserved22, iter);
	iter += sizeof(reserved22);

	Boolean reserved23 = false;
	::pack(reserved23, iter);
	iter += sizeof(reserved23);

	Boolean reserved24 = false;
	::pack(reserved24, iter);
	iter += sizeof(reserved24);

	Boolean reserved25 = false;
	::pack(reserved25, iter);
	iter += sizeof(reserved25);

	Boolean reserved26 = false;
	::pack(reserved26, iter);
	iter += sizeof(reserved26);

	Boolean reserved27 = false;
	::pack(reserved27, iter);
	iter += sizeof(reserved27);

	Boolean reserved28 = false;
	::pack(reserved28, iter);
	iter += sizeof(reserved28);

	Boolean reserved29 = false;
	::pack(reserved29, iter);
	iter += sizeof(reserved29);

	Boolean reserved30 = false;
	::pack(reserved30, iter);
	iter += sizeof(reserved30);

	Boolean reserved31 = false;
	::pack(reserved31, iter);
	iter += sizeof(reserved31);

	Boolean reserved32 = false;
	::pack(reserved32, iter);
	iter += sizeof(reserved32);

}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYVMCAPABILITIESNEWPACKET_H_ */
