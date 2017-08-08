/*
 * JdwpCommandErsetPacket.hpp
 *
 *  Created on: 2016. 3. 10.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDERSETPACKET_HPP_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDERSETPACKET_HPP_

#include "JdwpCommandErsetPacket.h"

#include <cassert>

#include "../JdwpSuspendPolicy.h"

template <typename Vm>
JdwpCommandErsetPacket<Vm>::JdwpCommandErsetPacket(const JdwpCommandPacket &packet) : JdwpCommandPacket(packet)
{
	readData();
}

template <typename Vm>
JdwpCommandErsetPacket<Vm>::~JdwpCommandErsetPacket()
{
}

template <typename Vm>
void JdwpCommandErsetPacket<Vm>::print(std::ostream &out) const
{

	JdwpCommandPacket::print(out);
	out << " : (";
	out << "eventKind : ";
	out << _eventKind;
	out << ", suspendPolicy : ";
	auto suspendPolicy = JdwpSuspendPolicy(_suspendPolicy);
	suspendPolicy.print(out);
	out << ", modifiers : ";
	out << _modifiers;


	for (Int i=0; i<_modifiers; i++) {
		out << ", ";
		_modifierBuf[i]->print(out);
	}
	out << ")";
}



template <typename Vm>
void JdwpCommandErsetPacket<Vm>::readData()
{
	auto iter = _dataBuf.cbegin();

	Byte eventKind;
	::unpack(iter, &eventKind);
	iter += sizeof(eventKind);
	_eventKind = eventKind;

	::unpack<Byte>(iter, &_suspendPolicy);
	iter += sizeof(_suspendPolicy);

	::unpack<Int>(iter, &_modifiers);
	iter += sizeof(_modifiers );

	_modifierBuf.clear();
	for (Int i=0; i<_modifiers; i++){
		Byte modKind;
		::unpack<Byte>(iter, &modKind);

		std::shared_ptr<Modifier> modifier;
		switch(modKind) {
		case 1:
			modifier = Modifier1::unpack(iter);
			break;
		case 2:
			modifier = Modifier2::unpack(iter);
			break;
		case 3:
			modifier = Modifier3::unpack(iter);
			break;
		case 4:
			modifier = Modifier4::unpack(iter);
			break;
		case 5:
			modifier = Modifier5::unpack(iter);
			break;
		case 6:
			modifier = Modifier6::unpack(iter);
			break;
		case 7:
			modifier = Modifier7::unpack(iter);
			break;
		case 8:
			modifier = Modifier8::unpack(iter);
			break;
		case 9:
			modifier = Modifier9::unpack(iter);
			break;
		case 10:
			modifier = Modifier10::unpack(iter);
			break;
		case 11:
			modifier = Modifier11::unpack(iter);
			break;
		case 12:
			modifier = Modifier12::unpack(iter);
			break;
		default:
			assert(false);
			break;
		}
		iter += modifier->size();
		_modifierBuf.push_back(modifier);
	}

}

template <typename Vm>
void JdwpCommandErsetPacket<Vm>::Modifier1::print(std::ostream &out) const
{
	out << "(";
	out << "modKind : " << Modifier::getModKind();
	out << ", count : " << getCount();
	out << ")";
}

template <typename Vm>
std::shared_ptr<typename JdwpCommandErsetPacket<Vm>::Modifier1> JdwpCommandErsetPacket<Vm>::Modifier1::unpack(std::vector<std::uint8_t>::const_iterator iter)
{
	std::shared_ptr<Modifier1> modifier1;

	Byte modKind;
	::unpack(iter, &modKind);
	iter += sizeof(modKind);
	if (modKind == 1) {
		modifier1 = std::make_shared<Modifier1>();

		::unpack(iter, &modifier1->_count);
	}
	return modifier1;
}

template <typename Vm>
void JdwpCommandErsetPacket<Vm>::Modifier2::print(std::ostream &out) const
{
	out << "(";
	out << "modKind : " << Modifier::getModKind();
	out << ", exprID : " << getExprID();
	out << ")";
}

template <typename Vm>
std::shared_ptr<typename JdwpCommandErsetPacket<Vm>::Modifier2> JdwpCommandErsetPacket<Vm>::Modifier2::unpack(std::vector<std::uint8_t>::const_iterator iter)
{
	std::shared_ptr<Modifier2> modifier2;

	Byte modKind;
	::unpack(iter, &modKind);
	iter += sizeof(modKind);
	if (modKind == 2) {
		modifier2 = std::make_shared<Modifier2>();

		::unpack(iter, &modifier2->_exprID);
	}
	return modifier2;
}

template <typename Vm>
void JdwpCommandErsetPacket<Vm>::Modifier3::print(std::ostream &out) const
{
	out << "(";
	out << "modKind : " << Modifier::getModKind();
	out << ", thread : " << static_cast<std::int64_t>(getThread());
	out << ")";
}

template <typename Vm>
std::shared_ptr<typename JdwpCommandErsetPacket<Vm>::Modifier3> JdwpCommandErsetPacket<Vm>::Modifier3::unpack(std::vector<std::uint8_t>::const_iterator iter)
{
	std::shared_ptr<Modifier3> modifier3;

	Byte modKind;
	::unpack(iter, &modKind);
	iter += sizeof(modKind);
	if (modKind == 3) {
		modifier3 = std::make_shared<Modifier3>();

		::unpack(iter, &modifier3->_thread);
	}
	return modifier3;
}

template <typename Vm>
void JdwpCommandErsetPacket<Vm>::Modifier4::print(std::ostream &out) const
{
	out << "(";
	out << "modKind : " << Modifier::getModKind();
	out << ", clazz : " << static_cast<std::int64_t>(getClazz());
	out << ")";
}

template <typename Vm>
std::shared_ptr<typename JdwpCommandErsetPacket<Vm>::Modifier4> JdwpCommandErsetPacket<Vm>::Modifier4::unpack(std::vector<std::uint8_t>::const_iterator iter)
{
	std::shared_ptr<Modifier4> modifier4;

	Byte modKind;
	::unpack(iter, &modKind);
	iter += sizeof(modKind);
	if (modKind == 4) {
		modifier4 = std::make_shared<Modifier4>();

		::unpack(iter, &modifier4->_clazz);
	}
	return modifier4;
}

template <typename Vm>
void JdwpCommandErsetPacket<Vm>::Modifier5::print(std::ostream &out) const
{
	out << "(";
	out << "modKind : " << Modifier::getModKind();
	out << ", classPattern : ";
	getClassPattern().print(out);
	out << ")";
}

template <typename Vm>
std::shared_ptr<typename JdwpCommandErsetPacket<Vm>::Modifier5> JdwpCommandErsetPacket<Vm>::Modifier5::unpack(std::vector<std::uint8_t>::const_iterator iter)
{
	std::shared_ptr<Modifier5> modifier5;

	Byte modKind;
	::unpack(iter, &modKind);
	iter += sizeof(modKind);
	if (modKind == 5) {
		modifier5 = std::make_shared<Modifier5>();

		modifier5->_classPattern = JdwpTypes<Vm>::String::unpack(iter);
	}
	return modifier5;
}

template <typename Vm>
void JdwpCommandErsetPacket<Vm>::Modifier6::print(std::ostream &out) const
{
	out << "(";
	out << "modKind : " << Modifier::getModKind();
	out << ", classPattern : ";
	getClassPattern().print(out);
	out << ")";
}

template <typename Vm>
std::shared_ptr<typename JdwpCommandErsetPacket<Vm>::Modifier6> JdwpCommandErsetPacket<Vm>::Modifier6::unpack(std::vector<std::uint8_t>::const_iterator iter)
{
	std::shared_ptr<Modifier6> modifier6;

	Byte modKind;
	::unpack(iter, &modKind);
	iter += sizeof(modKind);
	if (modKind == 6) {
		modifier6 = std::make_shared<Modifier6>();

		modifier6->_classPattern = JdwpTypes<Vm>::String::unpack(iter);
	}
	return modifier6;
}

template <typename Vm>
void JdwpCommandErsetPacket<Vm>::Modifier7::print(std::ostream &out) const
{
	out << "(";
	out << "modKind : " << Modifier::getModKind();
	out << ", loc : ";
	getLoc().print(out);
	out << ")";
}

template <typename Vm>
std::shared_ptr<typename JdwpCommandErsetPacket<Vm>::Modifier7> JdwpCommandErsetPacket<Vm>::Modifier7::unpack(std::vector<std::uint8_t>::const_iterator iter)
{
	std::shared_ptr<Modifier7> modifier7;

	Byte modKind;
	::unpack(iter, &modKind);
	iter += sizeof(modKind);
	if (modKind == 7) {
		modifier7 = std::make_shared<Modifier7>();

		modifier7->_loc = JdwpTypes<Vm>::Location::unpack(iter);
	}
	return modifier7;
}


template <typename Vm>
void JdwpCommandErsetPacket<Vm>::Modifier8::print(std::ostream &out) const
{
	out << "(";
	out << "modKind : " << Modifier::getModKind();
	out << ", exceptionOrNull : " << getExceptionOrNull();
	out << ", caught : " << static_cast<int>(getCaught());
	out << ", uncaught : " << static_cast<int>(getUncaught());
	out << ")";
}

template <typename Vm>
std::shared_ptr<typename JdwpCommandErsetPacket<Vm>::Modifier8> JdwpCommandErsetPacket<Vm>::Modifier8::unpack(std::vector<std::uint8_t>::const_iterator iter)
{
	std::shared_ptr<Modifier8> modifier8;

	Byte modKind;
	::unpack(iter, &modKind);
	iter += sizeof(modKind);
	if (modKind == 8) {
		modifier8 = std::make_shared<Modifier8>();

		::unpack(iter, &modifier8->_exceptionOrNull);
		iter += sizeof(modifier8->_exceptionOrNull);

		::unpack(iter, &modifier8->_caught);
		iter += sizeof(modifier8->_caught);

		::unpack(iter, &modifier8->_uncaught);
	}

	return modifier8;
}

template <typename Vm>
void JdwpCommandErsetPacket<Vm>::Modifier9::print(std::ostream &out) const
{
	out << "(";
	out << "modKind : " << Modifier::getModKind();
	out << ", declaring : " << static_cast<std::int64_t>(getDeclaring());
	out << ", fieldID : " << static_cast<std::int64_t>(getFieldID());
	out << ")";
}

template <typename Vm>
std::shared_ptr<typename JdwpCommandErsetPacket<Vm>::Modifier9> JdwpCommandErsetPacket<Vm>::Modifier9::unpack(std::vector<std::uint8_t>::const_iterator iter)
{
	std::shared_ptr<Modifier9> modifier9;

	Byte modKind;
	::unpack(iter, &modKind);
	iter += sizeof(modKind);
	if (modKind == 9) {
		modifier9 = std::make_shared<Modifier9>();

		::unpack(iter, &modifier9->_declaring);
		iter += sizeof(modifier9->_declaring );

		::unpack(iter, &modifier9->_fieldID);
	}

	return modifier9;
}

template <typename Vm>
void JdwpCommandErsetPacket<Vm>::Modifier10::print(std::ostream &out) const
{
	out << "(";
	out << "modKind : " << Modifier::getModKind();
	out << ", thread : " << static_cast<std::int64_t>(getThread());
	out << ", size : " << getSize();
	out << ", depth : " << getDepth();
	out << ")";
}

template <typename Vm>
std::shared_ptr<typename JdwpCommandErsetPacket<Vm>::Modifier10> JdwpCommandErsetPacket<Vm>::Modifier10::unpack(std::vector<std::uint8_t>::const_iterator iter)
{
	std::shared_ptr<Modifier10> modifier10;

	Byte modKind;
	::unpack(iter, &modKind);
	iter += sizeof(modKind);
	if (modKind == 10) {
		modifier10 = std::make_shared<Modifier10>();

		::unpack(iter, &modifier10->_thread);
		iter += sizeof(modifier10->_thread);

		::unpack(iter, &modifier10->_size);
		iter += sizeof(modifier10->_size);

		::unpack(iter, &modifier10->_depth);
	}

	return modifier10;
}

template <typename Vm>
void JdwpCommandErsetPacket<Vm>::Modifier11::print(std::ostream &out) const
{
	out << "(";
	out << "modKind : " << Modifier::getModKind();
	out << ", instance : " << static_cast<std::int64_t>(getInstance());
	out << ")";
}

template <typename Vm>
std::shared_ptr<typename JdwpCommandErsetPacket<Vm>::Modifier11> JdwpCommandErsetPacket<Vm>::Modifier11::unpack(std::vector<std::uint8_t>::const_iterator iter)
{
	std::shared_ptr<Modifier11> modifier11;

	Byte modKind;
	::unpack(iter, &modKind);
	iter += sizeof(modKind);
	if (modKind == 11) {
		modifier11 = std::make_shared<Modifier11>();

		::unpack(iter, &modifier11->_instance);
	}

	return modifier11;
}

template <typename Vm>
void JdwpCommandErsetPacket<Vm>::Modifier12::print(std::ostream &out) const
{
	out << "(";
	out << "modKind : " << Modifier::getModKind();
	out << ", sourceNamePattern : ";
	getSourceNamePattern().print(out);
	out << ")";
}

template <typename Vm>
std::shared_ptr<typename JdwpCommandErsetPacket<Vm>::Modifier12> JdwpCommandErsetPacket<Vm>::Modifier12::unpack(std::vector<std::uint8_t>::const_iterator iter)
{
	std::shared_ptr<Modifier12> modifier12;

	Byte modKind;
	::unpack(iter, &modKind);
	iter += sizeof(modKind);
	if (modKind == 12) {
		modifier12 = std::make_shared<Modifier12>();

		modifier12->_sourceNamePattern  = JdwpTypes<Vm>::String::unpack(iter);
	}

	return modifier12;
}


#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDERSETPACKET_HPP_ */
