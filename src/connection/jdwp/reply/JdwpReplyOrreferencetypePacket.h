/*
 * JdwpReplyOrreferencetypePacket.h
 *
 *  Created on: 2016. 4. 14.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYORREFERENCETYPEPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYORREFERENCETYPEPACKET_H_

#include "../JdwpReplyPacket.h"

#include "../JdwpTypes.h"

/**
 * ObjectReference Command Set (9)
 * ReferenceType Command (1)
 */
template <typename Vm>
class JdwpReplyOrreferencetypePacket final : public JdwpReplyPacket {
public:
	JdwpReplyOrreferencetypePacket();
	virtual ~JdwpReplyOrreferencetypePacket();

	void setRefTypeTag(Byte refTypeTag);
	void setTypeID(typename JdwpTypes<Vm>::ReferenceTypeID typeID);

	virtual void print(std::ostream &out) const override;

private:
	virtual void writeData() final override;

	Byte _refTypeTag = 0;
	typename JdwpTypes<Vm>::ReferenceTypeID _typeID;
};

template <typename Vm>
JdwpReplyOrreferencetypePacket<Vm>::JdwpReplyOrreferencetypePacket()
{
	writeData();
}

template <typename Vm>
JdwpReplyOrreferencetypePacket<Vm>::~JdwpReplyOrreferencetypePacket()
{

}

template <typename Vm>
void JdwpReplyOrreferencetypePacket<Vm>::setRefTypeTag(Byte refTypeTag)
{
	_refTypeTag = refTypeTag;
	auto iter = _dataBuf.begin();
	::pack(_refTypeTag, iter);
}

template <typename Vm>
void JdwpReplyOrreferencetypePacket<Vm>::setTypeID(typename JdwpTypes<Vm>::ReferenceTypeID typeID)
{
	_typeID = typeID;
	auto iter = _dataBuf.begin() + sizeof(_refTypeTag);
	::pack(_typeID, iter);
}

template <typename Vm>
void JdwpReplyOrreferencetypePacket<Vm>::print(std::ostream &out) const
{
	JdwpReplyPacket::print(out);
	out << "OR_REFERENCE_TYPE : ";
	out << "(";
	out << "refTypeTag : ";
	JdwpTypeTag(_refTypeTag).print(out);
	out << ", typeID : ";
	out << static_cast<std::uint64_t>(_typeID);
	out << ")";
}

template <typename Vm>
void JdwpReplyOrreferencetypePacket<Vm>::writeData()
{
	_dataBuf.clear();

	auto a = ::pack(_refTypeTag);
	_dataBuf.insert(_dataBuf.end(), a.cbegin(), a.cend());

	auto b = ::pack(_typeID);
	_dataBuf.insert(_dataBuf.end(), b.cbegin(), b.cend());
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYORREFERENCETYPEPACKET_H_ */
