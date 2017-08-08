/*
 * JdwpCommandEcompositePacket.hpp
 *
 *  Created on: 2016. 3. 16.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDECOMPOSITEPACKET_HPP_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDECOMPOSITEPACKET_HPP_

#include "JdwpCommandEcompositePacket.h"

#include "../JdwpSuspendPolicy.h"

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::VmStart::print(std::ostream &out) const
{
	out << "(eventKind : ";
	out << Event::getEventKind();
	out << ", requestID : ";
	out << _requestID;

	out << ", thread : ";
	out << static_cast<std::int64_t>(_thread);

	out << ")";
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::VmStart::pack(std::vector<std::uint8_t>::iterator iter) const
{
	Byte eventKind = Event::getEventKind();
	::pack(eventKind, iter);
	iter += sizeof(eventKind);

	::pack(_requestID, iter);
	iter += sizeof(_requestID);

	::pack(_thread, iter);
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::SingleStep::print(std::ostream &out) const
{
	out << "(eventKind : ";
	out << Event::getEventKind();
	out << ", requestID : ";
	out << _requestID;

	out << ", thread : ";
	out << static_cast<std::int64_t>(_thread);
	out << ", location : ";
	_location.print(out);

	out << ")";
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::SingleStep::pack(std::vector<std::uint8_t>::iterator iter) const
{
	Byte eventKind = Event::getEventKind();
	::pack(eventKind, iter);
	iter += sizeof(eventKind);

	::pack(_requestID, iter);
	iter += sizeof(_requestID);

	::pack(_thread, iter);
	iter += sizeof(_thread);

	_location.pack(iter);
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::BreakPoint::print(std::ostream &out) const
{
	out << "(eventKind : ";
	out << Event::getEventKind();
	out << ", requestID : ";
	out << _requestID;

	out << ", thread : ";
	out << static_cast<std::int64_t>(_thread);
	out << ", location : ";
	_location.print(out);

	out << ")";
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::BreakPoint::pack(std::vector<std::uint8_t>::iterator iter) const
{
	Byte eventKind = Event::getEventKind();
	::pack(eventKind, iter);
	iter += sizeof(eventKind);

	::pack(_requestID, iter);
	iter += sizeof(_requestID);

	::pack(_thread, iter);
	iter += sizeof(_thread);

	_location.pack(iter);
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::MethodEntry::print(std::ostream &out) const
{
	out << "(eventKind : ";
	out << Event::getEventKind();
	out << ", requestID : ";
	out << _requestID;

	out << ", thread : ";
	out << static_cast<std::int64_t>(_thread);
	out << ", location : ";
	_location.print(out);

	out << ")";
}
template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::MethodEntry::pack(std::vector<std::uint8_t>::iterator iter) const
{
	Byte eventKind = Event::getEventKind();
	::pack(eventKind, iter);
	iter += sizeof(eventKind);

	::pack(_requestID, iter);
	iter += sizeof(_requestID);

	::pack(_thread, iter);
	iter += sizeof(_thread);

	_location.pack(iter);
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::MethodExit::print(std::ostream &out) const
{
	out << "(eventKind : ";
	out << Event::getEventKind();
	out << ", requestID : ";
	out << _requestID;

	out << ", thread : ";
	out << static_cast<std::int64_t>(_thread);
	out << ", location : ";
	_location.print(out);

	out << ")";
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::MethodExit::pack(std::vector<std::uint8_t>::iterator iter) const
{
	Byte eventKind = Event::getEventKind();
	::pack(eventKind, iter);
	iter += sizeof(eventKind);

	::pack(_requestID, iter);
	iter += sizeof(_requestID);

	::pack(_thread, iter);
	iter += sizeof(_thread);

	_location.pack(iter);
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::MethodExitWithReturnValue::print(std::ostream &out) const
{
	out << "(eventKind : ";
	out << Event::getEventKind();
	out << ", requestID : ";
	out << _requestID;

	out << ", thread : ";
	out << static_cast<std::int64_t>(_thread);
	out << ", location : ";
	_location.print(out);
	out << ", value : ";
	_value.print(out);

	out << ")";
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::MethodExitWithReturnValue::pack(std::vector<std::uint8_t>::iterator iter) const
{
	Byte eventKind = Event::getEventKind();
	::pack(eventKind, iter);
	iter += sizeof(eventKind);

	::pack(_requestID, iter);
	iter += sizeof(_requestID);

	::pack(_thread, iter);
	iter += sizeof(_thread);

	_location.pack(iter);
	iter += _location.size();

	_value.pack(iter);
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::MonitorContendedEnter::print(std::ostream &out) const
{
	out << "(eventKind : ";
	out << Event::getEventKind();
	out << ", requestID : ";
	out << _requestID;

	out << ", thread : ";
	out << static_cast<std::int64_t>(_thread);
	out << ", object : ";
	_object.print(out);
	out << ", location : ";
	_location.print(out);

	out << ")";
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::MonitorContendedEnter::pack(std::vector<std::uint8_t>::iterator iter) const
{
	Byte eventKind = Event::getEventKind();
	::pack(eventKind, iter);
	iter += sizeof(eventKind);

	::pack(_requestID, iter);
	iter += sizeof(_requestID);

	::pack(_thread, iter);
	iter += sizeof(_thread);

	_object.pack(iter);
	iter += _object.size();

	_location.pack(iter);
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::MonitorContendedEntered::print(std::ostream &out) const
{
	out << "(eventKind : ";
	out << Event::getEventKind();
	out << ", requestID : ";
	out << _requestID;

	out << ", thread : ";
	out << static_cast<std::int64_t>(_thread);
	out << ", object : ";
	_object.print(out);
	out << ", location : ";
	_location.print(out);

	out << ")";
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::MonitorContendedEntered::pack(std::vector<std::uint8_t>::iterator iter) const
{
	Byte eventKind = Event::getEventKind();
	::pack(eventKind, iter);
	iter += sizeof(eventKind);

	::pack(_requestID, iter);
	iter += sizeof(_requestID);

	::pack(_thread, iter);
	iter += sizeof(_thread);

	_object.pack(iter);
	iter += _object.size();

	_location.pack(iter);
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::MonitorWait::print(std::ostream &out) const
{
	out << "(eventKind : ";
	out << Event::getEventKind();
	out << ", requestID : ";
	out << _requestID;

	out << ", thread : ";
	out << static_cast<std::int64_t>(_thread);
	out << ", object : ";
	_object.print(out);
	out << ", location : ";
	_location.print(out);
	out << ", timeout : ";
	out << _timeout;

	out << ")";
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::MonitorWait::pack(std::vector<std::uint8_t>::iterator iter) const
{
	Byte eventKind = Event::getEventKind();
	::pack(eventKind, iter);
	iter += sizeof(eventKind);

	::pack(_requestID, iter);
	iter += sizeof(_requestID);

	::pack(_thread, iter);
	iter += sizeof(_thread);

	_object.pack(iter);
	iter += _object.size();

	_location.pack(iter);
	iter += _location.size();

	::pack(_timeout, iter);
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::MonitorWaited::print(std::ostream &out) const
{
	out << "(eventKind : ";
	out << Event::getEventKind();
	out << ", requestID : ";
	out << _requestID;

	out << ", thread : ";
	out << static_cast<std::int64_t>(_thread);
	out << ", object : ";
	_object.print(out);
	out << ", location : ";
	_location.print(out);
	out << ", timed_out : ";
	out << _timedOut;

	out << ")";
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::MonitorWaited::pack(std::vector<std::uint8_t>::iterator iter) const
{
	Byte eventKind = Event::getEventKind();
	::pack(eventKind, iter);
	iter += sizeof(eventKind);

	::pack(_requestID, iter);
	iter += sizeof(_requestID);

	::pack(_thread, iter);
	iter += sizeof(_thread);

	_object.pack(iter);
	iter += _object.size();

	_location.pack(iter);
	iter += _location.size();

	::pack(_timedOut, iter);
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::Exception::print(std::ostream &out) const
{
	out << "(eventKind : ";
	out << Event::getEventKind();
	out << ", requestID : ";
	out << _requestID;

	out << ", thread : ";
	out << static_cast<std::int64_t>(_thread);
	out << ", location : ";
	_location.print(out);
	out << ", exception : ";
	_exception.print(out);
	out << ", catch location : ";
	_catchLocation.print(out);

	out << ")";
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::Exception::pack(std::vector<std::uint8_t>::iterator iter) const
{
	Byte eventKind = Event::getEventKind();
	::pack(eventKind, iter);
	iter += sizeof(eventKind);

	::pack(_requestID, iter);
	iter += sizeof(_requestID);

	::pack(_thread, iter);
	iter += sizeof(_thread);

	_location.pack(iter);
	iter += _location.size();

	_exception.pack(iter);
	iter += _exception.size();

	_catchLocation.pack(iter);
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::ThreadStart::print(std::ostream &out) const
{
	out << "(eventKind : ";
	out << Event::getEventKind();
	out << ", requestID : ";
	out << _requestID;

	out << ", thread : ";
	out << static_cast<std::int64_t>(_thread);

	out << ")";
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::ThreadStart::pack(std::vector<std::uint8_t>::iterator iter) const
{
	Byte eventKind = Event::getEventKind();
	::pack(eventKind, iter);
	iter += sizeof(eventKind);

	::pack(_requestID, iter);
	iter += sizeof(_requestID);

	::pack(_thread, iter);
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::ThreadDeath::print(std::ostream &out) const
{
	out << "(eventKind : ";
	out << Event::getEventKind();
	out << ", requestID : ";
	out << _requestID;

	out << ", thread : ";
	out << static_cast<std::int64_t>(_thread);

	out << ")";
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::ThreadDeath::pack(std::vector<std::uint8_t>::iterator iter) const
{
	Byte eventKind = Event::getEventKind();
	::pack(eventKind, iter);
	iter += sizeof(eventKind);

	::pack(_requestID, iter);
	iter += sizeof(_requestID);

	::pack(_thread, iter);
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::ClassPrepare::print(std::ostream &out) const
{
	out << "(eventKind : ";
	out << Event::getEventKind();
	out << ", requestID : ";
	out << _requestID;

	out << ", thread : ";
	out << static_cast<std::int64_t>(_thread);
	out << ", refTypeTag : ";
	out << _refTypeTag;
	out << ", typeID : ";
	out << static_cast<std::int64_t>(_typeID);
	out << ", signature : ";
	_signature.print(out);
	out << ", status : ";
	out << _status;

	out << ")";
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::ClassPrepare::pack(std::vector<std::uint8_t>::iterator iter) const
{
	Byte eventKind = Event::getEventKind();
	::pack(eventKind, iter);
	iter += sizeof(eventKind);

	::pack(_requestID, iter);
	iter += sizeof(_requestID);

	::pack(_thread, iter);
	iter += sizeof(_thread);

	::pack(_refTypeTag, iter);
	iter += sizeof(_refTypeTag);

	::pack(_typeID, iter);
	iter += sizeof(_typeID);

	_signature.pack(iter);
	iter += _signature.size();

	::pack(_status, iter);
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::ClassUnload::print(std::ostream &out) const
{
	out << "(eventKind : ";
	out << Event::getEventKind();
	out << ", requestID : ";
	out << _requestID;

	out << ", signature : ";
	_signature.print(out);

	out << ")";
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::ClassUnload::pack(std::vector<std::uint8_t>::iterator iter) const
{
	Byte eventKind = Event::getEventKind();
	::pack(eventKind, iter);
	iter += sizeof(eventKind);

	::pack(_requestID, iter);
	iter += sizeof(_requestID);

	_signature.pack(iter);
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::FieldModification::print(std::ostream &out) const
{
	out << "(eventKind : ";
	out << Event::getEventKind();
	out << ", requestID : ";
	out << _requestID;

	out << ", thread : ";
	out << static_cast<std::int64_t>(_thread);
	out << ", location : ";
	_location.print(out);
	out << ", refTypeTag : ";
	out << _refTypeTag;
	out << ", typeID : ";
	out << static_cast<std::int64_t>(_typeID);
	out << ", fieldID : ";
	out << static_cast<std::int64_t>(_fieldID);
	out << ", object : ";
	_object.print(out);
	out << ", valueToBe :";
	_valueToBe.print(out);

	out << ")";
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::FieldModification::pack(std::vector<std::uint8_t>::iterator iter) const
{
	Byte eventKind = Event::getEventKind();
	::pack(eventKind, iter);
	iter += sizeof(eventKind);

	::pack(_requestID, iter);
	iter += sizeof(_requestID);

	::pack(_thread, iter);
	iter += sizeof(_thread);

	_location.pack(iter);
	iter += _location.size();

	::pack(_refTypeTag, iter);
	iter += sizeof(_refTypeTag);

	::pack(_typeID, iter);
	iter += sizeof(_typeID);

	::pack(_fieldID, iter);
	iter += sizeof(_fieldID);

	_object.pack(iter);
	iter += _object.size();

	_valueToBe.pack(iter);
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::VmDeath::print(std::ostream &out) const
{
	out << "(eventKind : ";
	out << Event::getEventKind();
	out << ", requestID : ";
	out << _requestID;

	out << ")";
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::VmDeath::pack(std::vector<std::uint8_t>::iterator iter) const
{
	Byte eventKind = Event::getEventKind();
	::pack(eventKind, iter);
	iter += sizeof(eventKind);

	::pack(_requestID, iter);
	iter += sizeof(_requestID);
}

template <typename Vm>
JdwpCommandEcompositePacket<Vm>::JdwpCommandEcompositePacket() : JdwpCommandPacket(E_COMPOSITE)
{
	writeData();
}

template <typename Vm>
JdwpCommandEcompositePacket<Vm>::~JdwpCommandEcompositePacket()
{
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::setSuspendPolicy(Byte suspendPolicy)
{
	_suspendPolicy = suspendPolicy;

	::pack(_suspendPolicy, _dataBuf.begin());
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::setEvent(std::shared_ptr<Event> event)
{
	_event.push_back(event);

	_events = _event.size();
	::pack(_events, _dataBuf.begin()+sizeof(_suspendPolicy));

	auto a = event->pack();
	_dataBuf.insert(_dataBuf.end(), a.cbegin(), a.cend());
}

template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::print(std::ostream &out) const
{
	JdwpCommandPacket::print(out);
	out << " : (";
	out << "suspendPolicy : ";
	auto suspendPolicy = JdwpSuspendPolicy(_suspendPolicy);
	suspendPolicy.print(out);
	out << ", events : ";
	out << _events;
	for(auto &e : _event) {
		out << ", ";
		e->print(out);
	}
}
template <typename Vm>
void JdwpCommandEcompositePacket<Vm>::writeData()
{
	_dataBuf.clear();

	auto suspendPolicy = ::pack(_suspendPolicy);
	_dataBuf.insert(_dataBuf.end(), suspendPolicy.cbegin(), suspendPolicy.cend());

	auto events = ::pack(_events);
	_dataBuf.insert(_dataBuf.end(), events.cbegin(), events.cend());

	for(auto &e : _event) {
		auto event = e->pack();
		_dataBuf.insert(_dataBuf.end(), event.cbegin(), event.cend());
	}

}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDECOMPOSITEPACKET_HPP_ */
