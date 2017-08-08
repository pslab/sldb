/*
 * JdwpReplyVmclassesbysignaturePacket.h
 *
 *  Created on: 2016. 7. 19.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYVMCLASSESBYSIGNATUREPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYVMCLASSESBYSIGNATUREPACKET_H_

#include "../JdwpReplyPacket.h"

#include <vector>

#include "../JdwpTypes.h"

/**
 * VirtualMachine Command Set (1)
 * ClassesBySignature Command (2)
 */
template <typename Vm>
class JdwpReplyVmclassesbysignaturePacket final : public JdwpReplyPacket {
public:
	class Class final {
	public:
		void setRefTypeTag(Byte refTypeTag) { _refTypeTag = refTypeTag; }
		void setTypeID(typename JdwpTypes<Vm>::ReferenceTypeID typeID) { _typeID = typeID; }
		void setStatus(Int status) { _status = status; }

		virtual std::size_t size() const { return sizeof(_refTypeTag) + sizeof(_typeID) + sizeof(_status); }
		virtual void print(std::ostream &out) const;

		virtual std::vector<std::uint8_t> pack() const { std::vector<std::uint8_t> buf(size()); pack(buf.begin()); return buf; }
		virtual void pack(std::vector<std::uint8_t>::iterator iter) const;
	private:
		Byte _refTypeTag = 0;
		typename JdwpTypes<Vm>::ReferenceTypeID _typeID;
		Int _status = 0;
	};

	JdwpReplyVmclassesbysignaturePacket();
	virtual ~JdwpReplyVmclassesbysignaturePacket();

	void setClasses(std::vector<Class> classes);

	virtual void print(std::ostream &out) const override;

private:
	virtual void writeData() final override;
	std::vector<Class> _classes;
};


template <typename Vm>
void JdwpReplyVmclassesbysignaturePacket<Vm>::Class::print(std::ostream &out) const
{
	out << "(refTypeTag : ";
	out << static_cast<int>(_refTypeTag);
	out << ", typeID : ";
	out << static_cast<std::uint64_t>(_typeID);
	out << ", status : ";
	JdwpStatus(_status).print(out);
	out << ")";
}

template <typename Vm>
void JdwpReplyVmclassesbysignaturePacket<Vm>::Class::pack(std::vector<std::uint8_t>::iterator iter) const
{
	::pack(_refTypeTag, iter);
	iter += sizeof(_refTypeTag);

	::pack(_typeID ,iter);
	iter += sizeof(_typeID);

	::pack(_status, iter);
	iter += sizeof(_status);
}

template <typename Vm>
JdwpReplyVmclassesbysignaturePacket<Vm>::JdwpReplyVmclassesbysignaturePacket()
{
	writeData();
}

template <typename Vm>
JdwpReplyVmclassesbysignaturePacket<Vm>::~JdwpReplyVmclassesbysignaturePacket()
{

}

template <typename Vm>
void JdwpReplyVmclassesbysignaturePacket<Vm>::setClasses(std::vector<Class> classes)
{
	_classes = classes;
	writeData();
}

template <typename Vm>
void JdwpReplyVmclassesbysignaturePacket<Vm>::print(std::ostream &out) const
{
	JdwpReplyPacket::print(out);
	out << "VM_CLASSES_BY_SIGNATURE : ";
	out << "(";
	out << "classes : ";
	Int classes = _classes.size();
	out << classes;

	for(Int i=0; i<classes; i++) {
		out << ", ";
		_classes[i].print(out);
	}
	out << ")";
}

template <typename Vm>
void JdwpReplyVmclassesbysignaturePacket<Vm>::writeData()
{
	_dataBuf.clear();

	Int classes = _classes.size();
	auto a = ::pack(classes);
	_dataBuf.insert(_dataBuf.end(), a.cbegin(), a.cend());

	for(Int i=0; i<classes; i++) {
		auto c = _classes[i].pack();
		_dataBuf.insert(_dataBuf.end(), c.cbegin(), c.cend());
	}
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYVMCLASSESBYSIGNATUREPACKET_H_ */
