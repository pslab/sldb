/*
 * JdwpReplyRtfieldswithgenericPacket.h
 *
 *  Created on: 2016. 4. 14.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYRTFIELDSWITHGENERICPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYRTFIELDSWITHGENERICPACKET_H_

#include "../JdwpReplyPacket.h"

#include <string>
#include <vector>

#include "../JdwpTypes.h"

/**
 * VirtualMachine Command Set (1)
 * AllThreads Command (4)
 */
template <typename Vm>
class JdwpReplyRtfieldswithgenericPacket final : public JdwpReplyPacket {
public:
	class Field final {
	public:
		void setFieldID(typename JdwpTypes<Vm>::FieldID fieldID) { _fieldID = fieldID; }
		void setName(typename JdwpTypes<Vm>::String name) { _name = name; }
		void setSignature(typename JdwpTypes<Vm>::String signature) { _signature = signature; }
		void setGenericSignature(typename JdwpTypes<Vm>::String genericSignature) { _genericSignature = genericSignature; }
		void setModBits(Int modBits) { _modBits = modBits; }

		virtual std::size_t size() const { return sizeof(_fieldID) + _name.size() + _signature.size() + _genericSignature.size() + sizeof(_modBits); }
		virtual void print(std::ostream &out) const;

		virtual std::vector<std::uint8_t> pack() const { std::vector<std::uint8_t> buf(size()); pack(buf.begin()); return buf; }
		virtual void pack(std::vector<std::uint8_t>::iterator iter) const;
	private:
		typename JdwpTypes<Vm>::FieldID _fieldID = 0;
		typename JdwpTypes<Vm>::String _name;
		typename JdwpTypes<Vm>::String _signature;
		typename JdwpTypes<Vm>::String _genericSignature;
		Int _modBits = 0;
	};

	JdwpReplyRtfieldswithgenericPacket();
	virtual ~JdwpReplyRtfieldswithgenericPacket();

	void setFields(const std::vector<Field> &fields);
	void pushField(const Field &field);

	virtual void print(std::ostream &out) const override;

private:
	virtual void writeData() final override;

	std::vector<Field> _fields;
};

template <typename Vm>
void JdwpReplyRtfieldswithgenericPacket<Vm>::Field::print(std::ostream &out) const
{
	out << "(fieldID : ";
	out << static_cast<std::int64_t>(_fieldID);
	out << ", name : ";
	_name.print(out);
	out << ", signature : ";
	_signature.print(out);
	out << ", genericSignature : ";
	_genericSignature.print(out);
	out << ", modBits : ";
	out << _modBits;
	out << ")";
}

template <typename Vm>
void JdwpReplyRtfieldswithgenericPacket<Vm>::Field::pack(std::vector<std::uint8_t>::iterator iter) const
{
	::pack(_fieldID, iter);
	iter += sizeof(_fieldID);

	_name.pack(iter);
	iter += _name.size();

	_signature.pack(iter);
	iter += _signature.size();

	_genericSignature.pack(iter);
	iter += _genericSignature.size();

	::pack(_modBits, iter);
}

template <typename Vm>
JdwpReplyRtfieldswithgenericPacket<Vm>::JdwpReplyRtfieldswithgenericPacket()
{
	writeData();
}

template <typename Vm>
JdwpReplyRtfieldswithgenericPacket<Vm>::~JdwpReplyRtfieldswithgenericPacket()
{
}

template <typename Vm>
void JdwpReplyRtfieldswithgenericPacket<Vm>::setFields(const std::vector<Field> &fields)
{
	_fields = fields;
	writeData();
}
template <typename Vm>
void JdwpReplyRtfieldswithgenericPacket<Vm>::pushField(const Field &field)
{
	_fields.push_back(field);

	Int declared = _fields.size();
	::pack(declared, _dataBuf.begin());

	auto a = field.pack();
	_dataBuf.insert(_dataBuf.end(), a.cbegin(), a.cend());
}

template <typename Vm>
void JdwpReplyRtfieldswithgenericPacket<Vm>::print(std::ostream &out) const
{
	JdwpReplyPacket::print(out);
	out << " RT_FIELDS_WITH_GENERIC : ";
	out << "(";
	out << "declared : ";
	Int declared = _fields.size();
	out << declared;

	for(Int i=0; i<declared; i++) {
		out << ", field" << i << " : ";
		_fields[i].print(out);
	}
	out << ")";
}

template <typename Vm>
void JdwpReplyRtfieldswithgenericPacket<Vm>::writeData()
{
	_dataBuf.clear();

	Int declared = _fields.size();
	auto a = ::pack(declared);
	_dataBuf.insert(_dataBuf.end(), a.cbegin(), a.cend());

	for(Int i=0; i<declared; i++) {
		auto field = _fields[i].pack();
		_dataBuf.insert(_dataBuf.end(), field.cbegin(), field.cend());
	}
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYRTFIELDSWITHGENERICPACKET_H_ */
