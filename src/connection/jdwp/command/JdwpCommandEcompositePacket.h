/*
 * JdwpCommandEcompositePacket.h
 *
 *  Created on: 2016. 3. 15.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDECOMPOSITEPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDECOMPOSITEPACKET_H_

#include "../JdwpCommandPacket.h"

#include <vector>
#include <memory>
#include <cstdint>

#include "../JdwpTypes.h"

/**
 * EventRequest Command Set (15)
 * Set Command (1)
 */
template <typename Vm>
class JdwpCommandEcompositePacket final : public JdwpCommandPacket {
public:
	// Event
	class Event {
	public:
		JdwpEventKind getEventKind() const { return _eventKind; }

		virtual std::size_t size() const = 0;
		virtual void print(std::ostream &out) const = 0;

		virtual std::vector<std::uint8_t> pack() const { std::vector<std::uint8_t> buf(size()); pack(buf.begin()); return buf; }
		virtual void pack(std::vector<std::uint8_t>::iterator iter) const = 0;

	protected:
		Event(JdwpEventKind eventKind) : _eventKind(eventKind) {}
		~Event() {}

	private:
		JdwpEventKind _eventKind = 0;
	};

	class VmStart final : public Event {
	public:
		VmStart() : Event(JdwpEventKind::VM_START) {}
		virtual ~VmStart() {}

		void setRequestID(Int requestID) { _requestID = requestID; }
		void setThread(typename JdwpTypes<Vm>::ThreadID thread) { _thread = thread; }

		virtual std::size_t size() const override { return sizeof(Byte)+sizeof(_requestID)+sizeof(_thread); }
		virtual void print(std::ostream &out) const override;
		virtual void pack(std::vector<std::uint8_t>::iterator iter) const;

	private:
		Int _requestID = 0;
		typename JdwpTypes<Vm>::ThreadID _thread = 0;
	};

	class SingleStep final : public Event {
	public:
		SingleStep() : Event(JdwpEventKind::SINGLE_STEP) {}
		virtual ~SingleStep() {}

		void setRequestID(Int requestID) { _requestID = requestID; }
		void setThread(typename JdwpTypes<Vm>::ThreadID thread) { _thread = thread; }
		void setLocation(typename JdwpTypes<Vm>::Location location) { _location = location; }

		virtual std::size_t size() const override { return sizeof(Byte)+sizeof(_requestID)+sizeof(_thread)+_location.size(); }
		virtual void print(std::ostream &out) const override;
		virtual void pack(std::vector<std::uint8_t>::iterator iter) const;

	private:
		Int _requestID = 0;
		typename JdwpTypes<Vm>::ThreadID _thread = 0;
		typename JdwpTypes<Vm>::Location _location = 0;
	};

	class BreakPoint final : public Event {
	public:
		BreakPoint() : Event(JdwpEventKind::BREAKPOINT) {}
		virtual ~BreakPoint() {}

		void setRequestID(Int requestID) { _requestID = requestID; }
		void setThread(typename JdwpTypes<Vm>::ThreadID thread) { _thread = thread; }
		void setLocation(typename JdwpTypes<Vm>::Location location) { _location = location; }

		virtual std::size_t size() const override { return sizeof(Byte)+sizeof(_requestID)+sizeof(_thread)+_location.size(); }
		virtual void print(std::ostream &out) const override;
		virtual void pack(std::vector<std::uint8_t>::iterator iter) const;

	private:
		Int _requestID = 0;
		typename JdwpTypes<Vm>::ThreadID _thread = 0;
		typename JdwpTypes<Vm>::Location _location = 0;
	};

	class MethodEntry final : public Event {
	public:
		MethodEntry() : Event(JdwpEventKind::METHOD_ENTRY) {}
		virtual ~MethodEntry() {}

		void setRequestID(Int requestID) { _requestID = requestID; }
		void setThread(typename JdwpTypes<Vm>::ThreadID thread) { _thread = thread; }
		void setLocation(typename JdwpTypes<Vm>::Location location) { _location = location; }

		virtual std::size_t size() const override { return sizeof(Byte)+sizeof(_requestID)+sizeof(_thread)+_location.size(); }
		virtual void print(std::ostream &out) const override;
		virtual void pack(std::vector<std::uint8_t>::iterator iter) const;

	private:
		Int _requestID = 0;
		typename JdwpTypes<Vm>::ThreadID _thread = 0;
		typename JdwpTypes<Vm>::Location _location = 0;
	};

	class MethodExit final : public Event {
	public:
		MethodExit() : Event(JdwpEventKind::METHOD_EXIT) {}
		virtual ~MethodExit() {}

		void setRequestID(Int requestID) { _requestID = requestID; }
		void setThread(typename JdwpTypes<Vm>::ThreadID thread) { _thread = thread; }
		void setLocation(typename JdwpTypes<Vm>::Location location) { _location = location; }

		virtual std::size_t size() const override { return sizeof(Byte)+sizeof(_requestID)+sizeof(_thread)+_location.size(); }
		virtual void print(std::ostream &out) const override;
		virtual void pack(std::vector<std::uint8_t>::iterator iter) const;

	private:
		Int _requestID = 0;
		typename JdwpTypes<Vm>::ThreadID _thread = 0;
		typename JdwpTypes<Vm>::Location _location = 0;
	};

	class MethodExitWithReturnValue final : public Event {
	public:
		MethodExitWithReturnValue() : Event(JdwpEventKind::METHOD_EXIT_WITH_RETURN_VALUE) {}
		virtual ~MethodExitWithReturnValue() {}

		void setRequestID(Int requestID) { _requestID = requestID; }
		void setThread(typename JdwpTypes<Vm>::ThreadID thread) { _thread = thread; }
		void setLocation(typename JdwpTypes<Vm>::Location location) { _location = location; }
		void setValue(typename JdwpTypes<Vm>::Value value) { _value = value; }

		virtual std::size_t size() const override { return sizeof(Byte)+sizeof(_requestID)+sizeof(_thread)+_location.size()+_value.size(); }
		virtual void print(std::ostream &out) const override;
		virtual void pack(std::vector<std::uint8_t>::iterator iter) const;

	private:
		Int _requestID = 0;
		typename JdwpTypes<Vm>::ThreadID _thread = 0;
		typename JdwpTypes<Vm>::Location _location = 0;
		typename JdwpTypes<Vm>::Value _value;
	};

	class MonitorContendedEnter final : public Event {
	public:
		MonitorContendedEnter() : Event(JdwpEventKind::MONITOR_CONTENDED_ENTER) {}
		virtual ~MonitorContendedEnter() {}

		void setRequestID(Int requestID) { _requestID = requestID; }
		void setThread(typename JdwpTypes<Vm>::ThreadID thread) { _thread = thread; }
		void setObject(typename JdwpTypes<Vm>::TaggedObjectID object) { _object = object; }
		void setLocation(typename JdwpTypes<Vm>::Location location) { _location = location; }

		virtual std::size_t size() const override { return sizeof(Byte)+sizeof(_requestID)+sizeof(_thread)+_object.size()+_location.size(); }
		virtual void print(std::ostream &out) const override;
		virtual void pack(std::vector<std::uint8_t>::iterator iter) const;

	private:
		Int _requestID = 0;
		typename JdwpTypes<Vm>::ThreadID _thread = 0;
		typename JdwpTypes<Vm>::TaggedObjectID _object;
		typename JdwpTypes<Vm>::Location _location;
	};

	class MonitorContendedEntered final : public Event {
	public:
		MonitorContendedEntered() : Event(JdwpEventKind::MONITOR_CONTENDED_ENTERED) {}
		virtual ~MonitorContendedEntered() {}

		void setRequestID(Int requestID) { _requestID = requestID; }
		void setThread(typename JdwpTypes<Vm>::ThreadID thread) { _thread = thread; }
		void setObject(typename JdwpTypes<Vm>::TaggedObjectID object) { _object = object; }
		void setLocation(typename JdwpTypes<Vm>::Location location) { _location = location; }

		virtual std::size_t size() const override { return sizeof(Byte)+sizeof(_requestID)+sizeof(_thread)+_object.size()+_location.size(); }
		virtual void print(std::ostream &out) const override;
		virtual void pack(std::vector<std::uint8_t>::iterator iter) const;

	private:
		Int _requestID = 0;
		typename JdwpTypes<Vm>::ThreadID _thread = 0;
		typename JdwpTypes<Vm>::TaggedObjectID _object;
		typename JdwpTypes<Vm>::Location _location;
	};

	class MonitorWait final : public Event {
	public:
		MonitorWait() : Event(JdwpEventKind::MONITOR_WAIT) {}
		virtual ~MonitorWait() {}

		void setRequestID(Int requestID) { _requestID = requestID; }
		void setThread(typename JdwpTypes<Vm>::ThreadID thread) { _thread = thread; }
		void setObject(typename JdwpTypes<Vm>::TaggedObjectID object) { _object = object; }
		void setLocation(typename JdwpTypes<Vm>::Location location) { _location = location; }
		void setTimeout(Long timeout) { _timeout = timeout; }

		virtual std::size_t size() const override { return sizeof(Byte)+sizeof(_requestID)+sizeof(_thread)+_object.size()+_location.size()+sizeof(_timeout); }
		virtual void print(std::ostream &out) const override;
		virtual void pack(std::vector<std::uint8_t>::iterator iter) const;

	private:
		Int _requestID = 0;
		typename JdwpTypes<Vm>::ThreadID _thread = 0;
		typename JdwpTypes<Vm>::TaggedObjectID _object;
		typename JdwpTypes<Vm>::Location _location;
		Long _timeout = 0;
	};

	class MonitorWaited final : public Event {
	public:
		MonitorWaited() : Event(JdwpEventKind::MONITOR_WAITED) {}
		virtual ~MonitorWaited() {}

		void setRequestID(Int requestID) { _requestID = requestID; }
		void setThread(typename JdwpTypes<Vm>::ThreadID thread) { _thread = thread; }
		void setObject(typename JdwpTypes<Vm>::TaggedObjectID object) { _object = object; }
		void setLocation(typename JdwpTypes<Vm>::Location location) { _location = location; }
		void setTimedOut(Long timedOut) { _timedOut = timedOut; }

		virtual std::size_t size() const override { return sizeof(Byte)+sizeof(_requestID)+sizeof(_thread)+_object.size()+_location.size()+sizeof(_timedOut); }
		virtual void print(std::ostream &out) const override;
		virtual void pack(std::vector<std::uint8_t>::iterator iter) const;

	private:
		Int _requestID = 0;
		typename JdwpTypes<Vm>::ThreadID _thread = 0;
		typename JdwpTypes<Vm>::TaggedObjectID _object;
		typename JdwpTypes<Vm>::Location _location;
		Long _timedOut = 0;
	};

	class Exception final : public Event {
	public:
		Exception() : Event(JdwpEventKind::EXCEPTION) {}
		virtual ~Exception() {}

		void setRequestID(Int requestID) { _requestID = requestID; }
		void setThread(typename JdwpTypes<Vm>::ThreadID thread) { _thread = thread; }
		void setLocation(typename JdwpTypes<Vm>::Location location) { _location = location; }
		void setException(typename JdwpTypes<Vm>::TaggedObjectID exception) { _exception = exception; }
		void setCatchLocation(typename JdwpTypes<Vm>::Location catchLocation) { _catchLocation = catchLocation; }

		virtual std::size_t size() const override { return sizeof(Byte)+sizeof(_requestID)+sizeof(_thread)+_location.size()+_exception.size()+_catchLocation.size(); }
		virtual void print(std::ostream &out) const override;
		virtual void pack(std::vector<std::uint8_t>::iterator iter) const;

	private:
		Int _requestID = 0;
		typename JdwpTypes<Vm>::ThreadID _thread = 0;
		typename JdwpTypes<Vm>::Location _location;
		typename JdwpTypes<Vm>::TaggedObjectID _exception;
		typename JdwpTypes<Vm>::Location _catchLocation;
	};

	class ThreadStart final : public Event {
	public:
		ThreadStart() : Event(JdwpEventKind::THREAD_START) {}
		virtual ~ThreadStart() {}

		void setRequestID(Int requestID) { _requestID = requestID; }
		void setThread(typename JdwpTypes<Vm>::ThreadID thread) { _thread = thread; }

		virtual std::size_t size() const override { return sizeof(Byte)+sizeof(_requestID)+sizeof(_thread); }
		virtual void print(std::ostream &out) const override;
		virtual void pack(std::vector<std::uint8_t>::iterator iter) const;

	private:
		Int _requestID = 0;
		typename JdwpTypes<Vm>::ThreadID _thread = 0;
	};

	class ThreadDeath final : public Event {
	public:
		ThreadDeath() : Event(JdwpEventKind::THREAD_DEATH) {}
		virtual ~ThreadDeath() {}

		void setRequestID(Int requestID) { _requestID = requestID; }
		void setThread(typename JdwpTypes<Vm>::ThreadID thread) { _thread = thread; }

		virtual std::size_t size() const override { return sizeof(Byte)+sizeof(_requestID)+sizeof(_thread); }
		virtual void print(std::ostream &out) const override;
		virtual void pack(std::vector<std::uint8_t>::iterator iter) const;

	private:
		Int _requestID = 0;
		typename JdwpTypes<Vm>::ThreadID _thread = 0;
	};

	class ClassPrepare final : public Event {
	public:
		ClassPrepare() : Event(JdwpEventKind::CLASS_PREPARE) {}
		virtual ~ClassPrepare() {}

		void setRequestID(Int requestID) { _requestID = requestID; }
		void setThread(typename JdwpTypes<Vm>::ThreadID thread) { _thread = thread; }
		void setRefTypeTag(JdwpTypeTag refTypeTag) { _refTypeTag = refTypeTag; }
		void setTypeID(typename JdwpTypes<Vm>::ReferenceTypeID typeID) { _typeID = typeID; }
		void setSignature(typename JdwpTypes<Vm>::String signature) { _signature= signature; }
		void setStatus (Int status) { _status = status; }

		virtual std::size_t size() const override { return sizeof(Byte)+sizeof(_requestID)+sizeof(_thread)+sizeof(_refTypeTag)+sizeof(_typeID)+_signature.size()+sizeof(_status); }
		virtual void print(std::ostream &out) const override;
		virtual void pack(std::vector<std::uint8_t>::iterator iter) const;

	private:
		Int _requestID = 0;
		typename JdwpTypes<Vm>::ThreadID _thread = 0;
		JdwpTypeTag _refTypeTag;
		typename JdwpTypes<Vm>::ReferenceTypeID _typeID;
		typename JdwpTypes<Vm>::String _signature;
		Int _status = 0;
	};

	class ClassUnload final : public Event {
	public:
		ClassUnload() : Event(JdwpEventKind::CLASS_UNLOAD) {}
		virtual ~ClassUnload() {}

		void setRequestID(Int requestID) { _requestID = requestID; }
		void setSignature(typename JdwpTypes<Vm>::String signature) { _signature = signature; }

		virtual std::size_t size() const override { return sizeof(Byte)+sizeof(_requestID)+_signature.size(); }
		virtual void print(std::ostream &out) const override;
		virtual void pack(std::vector<std::uint8_t>::iterator iter) const;

	private:
		Int _requestID = 0;
		typename JdwpTypes<Vm>::String _signature;
	};

	class FieldModification final : public Event {
	public:
		FieldModification() : Event(JdwpEventKind::FIELD_MODIFICATION) {}
		virtual ~FieldModification() {}

		void setRequestID(Int requestID) { _requestID = requestID; }
		void setThread(typename JdwpTypes<Vm>::ThreadID thread) { _thread = thread; }
		void setLocation(typename JdwpTypes<Vm>::Location location) { _location = location; }
		void setRefTypeTag(JdwpTypeTag refTypeTag) { _refTypeTag = refTypeTag; }
		void setTypeID(typename JdwpTypes<Vm>::ReferenceTypeID typeID) { _typeID = typeID; }
		void setFieldID(typename JdwpTypes<Vm>::FieldID fieldID) { _fieldID = fieldID; }
		void setObject(typename JdwpTypes<Vm>::TaggedObjectID object) { _object = object; }
		void setValueToBe(typename JdwpTypes<Vm>::Value valueToBe) { _valueToBe = valueToBe; }

		virtual std::size_t size() const override { return sizeof(Byte)+sizeof(_requestID)+sizeof(_thread)+_location.size()+sizeof(_refTypeTag)+sizeof(_typeID)+sizeof(_fieldID)+_object.size()+_valueToBe.size(); }
		virtual void print(std::ostream &out) const override;
		virtual void pack(std::vector<std::uint8_t>::iterator iter) const;

	private:
		Int _requestID = 0;
		typename JdwpTypes<Vm>::ThreadID _thread = 0;
		typename JdwpTypes<Vm>::Location _location;
		JdwpTypeTag _refTypeTag;
		typename JdwpTypes<Vm>::ReferenceTypeID _typeID;
		typename JdwpTypes<Vm>::FieldID _fieldID;
		typename JdwpTypes<Vm>::TaggedObjectID _object;
		typename JdwpTypes<Vm>::Value _valueToBe;
	};

	class VmDeath final : public Event {
	public:
		VmDeath() : Event(JdwpEventKind::VM_DEATH) {}
		virtual ~VmDeath() {}

		void setRequestID(Int requestID) { _requestID = requestID; }

		virtual std::size_t size() const override { return sizeof(Byte)+sizeof(_requestID); }
		virtual void print(std::ostream &out) const override;
		virtual void pack(std::vector<std::uint8_t>::iterator iter) const;

	private:
		Int _requestID = 0;
	};

	JdwpCommandEcompositePacket();
	virtual ~JdwpCommandEcompositePacket();

	void setSuspendPolicy(Byte suspendPolicy);
	void setEvent(std::shared_ptr<Event> event);

	virtual void print(std::ostream &out) const override;

private:
	virtual void writeData() final override;

	Byte _suspendPolicy = 0;
	Int _events = 0;
	std::vector<std::shared_ptr<Event>> _event;
};

#include "JdwpCommandEcompositePacket.hpp"

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDECOMPOSITEPACKET_H_ */
