/*
 * JdwpTypes.h
 *
 *  Created on: 2016. 3. 9.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_JDWPTYPES_HPP_
#define JIKESDBG_SRC_CONNECTION_JDWP_JDWPTYPES_HPP_

#include "JdwpTypes.h"
#include <algorithm>
#include <cassert>

template <typename T>
T unpack(const std::vector<std::uint8_t> &vector)
{
	T var = 0;
	unpack(vector.cbegin(), &var);
	return var;
}

template <typename T>
void unpack(std::vector<std::uint8_t>::const_iterator iter, T *var)
{
	assert(var != nullptr);
	*var = 0;
	std::size_t bits = (sizeof(T)-1)*8;
	for(std::size_t i=0; i<sizeof(T); i++) {
		*var += *iter++ << bits;
		bits -= 8;
	}
}

template <typename T>
std::vector<std::uint8_t> pack(const T &val)
{
	std::vector<std::uint8_t> bytes(sizeof(val));
	pack(val, bytes.begin());
	return bytes;
}

template <typename T>
void pack(const T val, std::vector<std::uint8_t>::iterator iter)
{
	std::size_t bits = (sizeof(T)-1)*8;
	for(std::size_t i=0; i<sizeof(T); i++) {
		*iter = static_cast<std::uint8_t>(val >> bits);
		++iter;
		bits -= 8;
	}
}


template <typename T>
void JdwpTypes<T>::TaggedObjectID::print(std::ostream &out) const
{
	out << "(";
	out << "tag : " << getTag();
	out << ", objectID : " << static_cast<std::int64_t>(getObjectID());
	out << ")";
}

template <typename T>
typename JdwpTypes<T>::TaggedObjectID JdwpTypes<T>::TaggedObjectID::unpack(const std::vector<std::uint8_t> &vec)
{
	return unpack(vec.cbegin());
}

template <typename T>
typename JdwpTypes<T>::TaggedObjectID JdwpTypes<T>::TaggedObjectID::unpack(std::vector<std::uint8_t>::const_iterator iter)
{
	TaggedObjectID var;

	Byte tag;
	::unpack(iter, &tag);
	var._tag = tag;
	iter += sizeof(var._tag);

	::unpack<ObjectID>(iter, &var._objectID);
	return var;
}

template <typename T>
std::vector<std::uint8_t> JdwpTypes<T>::TaggedObjectID::pack() const
{
	std::vector<std::uint8_t> vec(size());
	pack(vec.begin());
	return vec;
}

template <typename T>
void JdwpTypes<T>::TaggedObjectID::pack(std::vector<std::uint8_t>::iterator iter) const
{
	::pack(_tag, iter);
	iter += sizeof(_tag);

	::pack(_objectID, iter);
}


template <typename T>
void JdwpTypes<T>::Location::print(std::ostream &out) const {
	out << "(";
	out << "typeTag : " << getTypeTag();
	out << ", classID : " << static_cast<std::int64_t>(getClassID());
	out << ", methodID : " << static_cast<std::int64_t>(getMethodID());
	out << ", index : " << getIndex();
	out << ")";
}

template <typename T>
typename JdwpTypes<T>::Location JdwpTypes<T>::Location::unpack(const std::vector<std::uint8_t> &vec)
{
	return unpack(vec.cbegin());
}
template <typename T>
typename JdwpTypes<T>::Location JdwpTypes<T>::Location::unpack(std::vector<std::uint8_t>::const_iterator iter)
{
	Location var;

	Byte typeTag;
	::unpack(iter, &typeTag);
	var._typeTag = typeTag;
	iter += sizeof(var._typeTag);

	::unpack(iter, &var._classID);
	iter += sizeof(var._classID);

	::unpack<MethodID>(iter, &var._methodID);
	iter += sizeof(var._methodID);

	::unpack(iter, &var._index);

	return var;
}

template <typename T>
std::vector<std::uint8_t> JdwpTypes<T>::Location::pack() const
{
	std::vector<std::uint8_t> vec(size());
	pack(vec.begin());
	return vec;
}
template <typename T>
void JdwpTypes<T>::Location::pack(std::vector<std::uint8_t>::iterator iter) const
{
	::pack(_typeTag, iter);
	iter += sizeof(_typeTag);

	::pack(_classID, iter);
	iter += sizeof(_classID);

	::pack(_methodID, iter);
	iter += sizeof(_methodID);

	::pack(_index, iter);
}

template <typename T>
void JdwpTypes<T>::String::print(std::ostream &out) const {
	out << "(";
	out << "length : " << get().length();
	out << ", string : " << get();
	out << ")";
}

template <typename T>
typename JdwpTypes<T>::String JdwpTypes<T>::String::unpack(const std::vector<std::uint8_t> &vec)
{
	return unpack(vec.cbegin());
}

template <typename T>
typename JdwpTypes<T>::String JdwpTypes<T>::String::unpack(std::vector<std::uint8_t>::const_iterator iter)
{
	String var;

	Int length = 0;
	::unpack<Int>(iter, &length);
	iter += sizeof(length);

	std::string str;
	for(Int i=0; i<length; i++) {
		char c = 0;
		::unpack(iter, &c);
		iter += sizeof(c);
		str.push_back(c);
	}
	var.set(str);

	return var;
}

template <typename T>
std::vector<std::uint8_t> JdwpTypes<T>::String::pack() const
{
	std::vector<std::uint8_t> vec(size());
	pack(vec.begin());
	return vec;
}
template <typename T>
void JdwpTypes<T>::String::pack(std::vector<std::uint8_t>::iterator iter) const
{
	Int length = static_cast<Int>(_string.length());
	::pack(length, iter);
	iter += sizeof(length);

	for (Int i=0; i<length; i++) {
		unsigned char c = _string[i];
		::pack(c, iter);
		iter += sizeof(c);
	}
}

template <typename T>
void JdwpTypes<T>::Value::setValue()
{
	_tag = JdwpTag::VOID;
	_value.clear();
}

template <typename T>
void JdwpTypes<T>::Value::setValue(Byte val)
{
	_tag = JdwpTag::BYTE;
	_value = ::pack(val);
}

template <typename T>
void JdwpTypes<T>::Value::setValue(bool val)
{
	_tag = JdwpTag::BOOLEAN;
	_value = ::pack(static_cast<Byte>(val));
}

template <typename T>
void JdwpTypes<T>::Value::setValue(Byte tag, std::int16_t val)
{
	_tag = tag;
	_value = ::pack(val);
}

template <typename T>
void JdwpTypes<T>::Value::setValue(Byte tag, std::int32_t val)
{
	_tag = tag;
	_value = ::pack(val);
}

template <typename T>
void JdwpTypes<T>::Value::setValue(Byte tag, std::int64_t val)
{
	_tag = tag;
	_value = ::pack(val);
}

template <typename T>
void JdwpTypes<T>::Value::setValue(float val)
{
	_tag = JdwpTag::FLOAT;
	ReinterpretConversion tmp;
	tmp._float = val;
	_value = ::pack(tmp._uint32_t);
}

template <typename T>
void JdwpTypes<T>::Value::setValue(double val)
{
	_tag = JdwpTag::DOUBLE;
	ReinterpretConversion tmp;
	tmp._double = val;
	_value = ::pack(tmp._uint64_t);
}

template <typename T>
void JdwpTypes<T>::Value::print(std::ostream &out) const {
	out << "(";
	out << "tag : " << getTag();
	out << ", value : ";
	switch (_tag) {
	case JdwpTag::VOID:
		break;

	case JdwpTag::BYTE:
		out << static_cast<int>(::unpack<Byte>(_value));
		break;

	case JdwpTag::BOOLEAN:
		out << static_cast<int>(::unpack<Boolean>(_value));
		break;

	case JdwpTag::CHAR:
		out << static_cast<char>(::unpack<std::int16_t>(_value));
		break;

	case JdwpTag::SHORT:
		out << ::unpack<std::int16_t>(_value);
		break;

	case JdwpTag::FLOAT: {
		ReinterpretConversion tmp;
		tmp._uint32_t = ::unpack<std::uint32_t>(_value);
		out << tmp._float;
		break;
	}
	case JdwpTag::INT:
		out << ::unpack<Int>(_value);
		break;

	case JdwpTag::DOUBLE: {
		ReinterpretConversion tmp;
		tmp._uint64_t = ::unpack<std::uint64_t>(_value);
		out << tmp._double;
		break;
	}

	case JdwpTag::LONG:
		out << ::unpack<Long>(_value);
		break;

	case JdwpTag::ARRAY:
	case JdwpTag::OBJECT:
	case JdwpTag::STRING:
	case JdwpTag::THREAD:
	case JdwpTag::THREAD_GROUP:
	case JdwpTag::CLASS_LOADER:
	case JdwpTag::CLASS_OBJECT:
		out << static_cast<std::int64_t>(::unpack<ObjectID>(_value));
		break;
	}
	out << ")";
}

template <typename T>
typename JdwpTypes<T>::Value JdwpTypes<T>::Value::unpack(const std::vector<std::uint8_t> &vec)
{
	return unpack(vec.cbegin());
}
template <typename T>
typename JdwpTypes<T>::Value JdwpTypes<T>::Value::unpack(std::vector<std::uint8_t>::const_iterator iter)
{
	Value var;

	Byte tag;
	::unpack(iter, &tag);
	var._tag = tag;
	iter += sizeof(var._tag);

	switch (var._tag) {
	case JdwpTag::VOID:
		break;

	case JdwpTag::BYTE:
	case JdwpTag::BOOLEAN:
		var._value.assign(iter, iter+1);
		break;

	case JdwpTag::CHAR:
	case JdwpTag::SHORT:
		var._value.assign(iter, iter+2);
		break;

	case JdwpTag::FLOAT:
	case JdwpTag::INT:
		var._value.assign(iter, iter+4);
		break;

	case JdwpTag::DOUBLE:
	case JdwpTag::LONG:
		var._value.assign(iter, iter+8);
		break;

	case JdwpTag::ARRAY:
	case JdwpTag::OBJECT:
	case JdwpTag::STRING:
	case JdwpTag::THREAD:
	case JdwpTag::THREAD_GROUP:
	case JdwpTag::CLASS_LOADER:
	case JdwpTag::CLASS_OBJECT:
		var._value.assign(iter, iter+sizeof(ObjectID));
		break;
	}

	return var;
}

template <typename T>
std::vector<std::uint8_t> JdwpTypes<T>::Value::pack() const
{
	std::vector<std::uint8_t> vec(size());
	pack(vec.begin());
	return vec;
}
template <typename T>
void JdwpTypes<T>::Value::pack(std::vector<std::uint8_t>::iterator iter) const
{
	::pack(_tag, iter);
	iter += sizeof(_tag);

	std::copy(_value.cbegin(), _value.cend(), iter);
}


#endif /* JIKESDBG_SRC_CONNECTION_JDWP_JDWPTYPES_HPP_ */
