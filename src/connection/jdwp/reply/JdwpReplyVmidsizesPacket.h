/*
 * JdwpReplyVmidsizesPacket.h
 *
 *  Created on: 2016. 3. 10.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYVMIDSIZESPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYVMIDSIZESPACKET_H_

#include "../JdwpReplyPacket.h"

#include "../JdwpTypes.h"

/**
 * VirtualMachine Command Set (1)
 * Version Command (1)
 */
template <typename Vm>
class JdwpReplyVmidsizesPacket final: public JdwpReplyPacket {
public:
	JdwpReplyVmidsizesPacket();
	virtual ~JdwpReplyVmidsizesPacket();

	virtual void print(std::ostream &out) const override;

private:
	virtual void writeData() final override;
};

template <typename Vm>
JdwpReplyVmidsizesPacket<Vm>::JdwpReplyVmidsizesPacket()
{
	writeData();
}

template <typename Vm>
JdwpReplyVmidsizesPacket<Vm>::~JdwpReplyVmidsizesPacket()
{
}

template <typename Vm>
void JdwpReplyVmidsizesPacket<Vm>::print(std::ostream &out) const
{
	JdwpReplyPacket::print(out);
	out << " VM_ID_SIZES : ";
	out << "(";
	out << "fieldIDSize : ";
	out << sizeof(typename JdwpTypes<Vm>::FieldID);
	out << ", methodIDSize : ";
	out << sizeof(typename JdwpTypes<Vm>::MethodID);
	out << ", objectIDSize : ";
	out << sizeof(typename JdwpTypes<Vm>::ObjectID);
	out << ", referenceTypeIDSize : ";
	out << sizeof(typename JdwpTypes<Vm>::ReferenceTypeID);
	out << ", frameIDSize : ";
	out << sizeof(typename JdwpTypes<Vm>::FrameID);
	out << ")";
}

template <typename Vm>
void JdwpReplyVmidsizesPacket<Vm>::writeData()
{
	_dataBuf.resize(sizeof(Int)*5);
	auto iter = _dataBuf.begin();

	Int fieldIDSize = sizeof(typename JdwpTypes<Vm>::FieldID);
	::pack(fieldIDSize, iter);
	iter += sizeof(fieldIDSize);

	Int methodIDSize = sizeof(typename JdwpTypes<Vm>::MethodID);
	::pack(methodIDSize, iter);
	iter += sizeof(methodIDSize);

	Int objectIDSize = sizeof(typename JdwpTypes<Vm>::ObjectID);
	::pack(objectIDSize, iter);
	iter += sizeof(objectIDSize);

	Int referenceTypeIDSize = sizeof(typename JdwpTypes<Vm>::ReferenceTypeID);
	::pack(referenceTypeIDSize, iter);
	iter += sizeof(referenceTypeIDSize);

	Int frameIDSize = sizeof(typename JdwpTypes<Vm>::FrameID);
	::pack(frameIDSize, iter);

}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYVMIDSIZESPACKET_H_ */
