/*
 * VmTypes.h
 *
 *  Created on: 2016. 3. 8.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_JDWPTYPES_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_JDWPTYPES_H_

#include <vector>
#include <string>
#include <ostream>
#include <cinttypes>

typedef std::int8_t Byte;
typedef std::uint8_t Boolean;
typedef std::int32_t Int;
typedef std::int64_t Long;
typedef std::uint32_t UnsignedInt;
typedef std::uint64_t UnsignedLong;

#include "JdwpEventKind.h"
#include "JdwpSuspendPolicy.h"
#include "JdwpTag.h"
#include "JdwpTypeTag.h"
#include "JdwpStatus.h"
#include "JdwpThreadStatus.h"
#include "JdwpSuspendStatus.h"

template <typename T>
T unpack(const std::vector<std::uint8_t> &vector);

template <typename T>
void unpack(std::vector<std::uint8_t>::const_iterator iter, T *var);

template <typename T>
std::vector<std::uint8_t> pack(const T &val);

template <typename T>
void pack(const T val, std::vector<std::uint8_t>::iterator iter);

template <typename T>
class JdwpTypes {
public:
	typedef typename T::ObjectID ObjectID;
	typedef ObjectID ThreadID;
	typedef ObjectID ThreadGroupID;
	typedef ObjectID StringID;
	typedef ObjectID ClassLoaderID;
	typedef ObjectID ClassObjectID;
	typedef ObjectID ArrayID;

	typedef typename T::ReferenceTypeID ReferenceTypeID;
	typedef ReferenceTypeID ClassID;
	typedef ReferenceTypeID InterfaceID;
	typedef ReferenceTypeID ArrayTypeID;

	typedef typename T::MethodID MethodID;
	typedef typename T::FieldID FieldID;
	typedef typename T::FrameID FrameID;

	class TaggedObjectID final {
	public:
		Byte  getTag() const {return _tag;}
		void setTag(Byte  tag) {_tag = tag;}
		ObjectID getObjectID() const {return _objectID;}
		void setObjectID(ObjectID objectID) {_objectID = objectID;}
		std::size_t size() const { return sizeof(_tag) + sizeof(_objectID); }

		void print(std::ostream &out) const;
		static TaggedObjectID unpack(const std::vector<std::uint8_t> &vec);
		static TaggedObjectID unpack(std::vector<std::uint8_t>::const_iterator iter);
		std::vector<std::uint8_t> pack() const;
		void pack(std::vector<std::uint8_t>::iterator iter) const;

	private:
		Byte _tag;
		ObjectID _objectID = 0;
	};

	class Location final {
	public:
		Byte getTypeTag() const { return _typeTag; }
		void setTypeTag(Byte typeTag) { _typeTag = typeTag; }
		ClassID getClassID() const { return _classID; }
		void setClassID(ClassID classID) { _classID = classID; }
		MethodID getMethodID() const { return _methodID; }
		void setMethodID(MethodID methodID) { _methodID = methodID; }
		UnsignedLong getIndex() const { return _index; }
		void setIndex(UnsignedLong index) { _index = index; }
		std::size_t size() const { return sizeof(_typeTag) + sizeof(_classID) + sizeof(_methodID) + sizeof(_index); }

		void print(std::ostream &out) const;
		static Location unpack(const std::vector<std::uint8_t> &vec);
		static Location unpack(std::vector<std::uint8_t>::const_iterator iter);
		std::vector<std::uint8_t> pack() const;
		void pack(std::vector<std::uint8_t>::iterator iter) const;

	private:
		Byte _typeTag;
		ClassID _classID = 0;
		MethodID _methodID = 0;
		UnsignedLong _index = 0;
	};

	class String final {
	public:
		String() {}
		String(std::string string) : _string(string) {}
		~String() {}

		std::string get() const { return _string; }
		void set(std::string string) { _string = string; }
		std::size_t size() const { return sizeof(Int) + _string.size(); }

		void print(std::ostream &out) const;
		static String unpack(const std::vector<std::uint8_t> &vec);
		static String unpack(std::vector<std::uint8_t>::const_iterator iter);
		std::vector<std::uint8_t> pack() const;
		void pack(std::vector<std::uint8_t>::iterator iter) const;

	private:
		std::string _string;
	};

	class Value final {
	public:
		Byte getTag() const { return _tag; }
		void setValue();
		void setValue(Byte val);
		void setValue(bool val);
		void setValue(Byte tag, std::int16_t val);
		void setValue(Byte tag, std::int32_t val);
		void setValue(Byte tag, std::int64_t val);
		void setValue(float val);
		void setValue(double val);
		std::size_t size() const { return sizeof(_tag) + _value.size(); }

		void print(std::ostream &out) const;
		static Value unpack(const std::vector<std::uint8_t> &vec);
		static Value unpack(std::vector<std::uint8_t>::const_iterator iter);
		std::vector<std::uint8_t> pack() const;
		void pack(std::vector<std::uint8_t>::iterator iter) const;

	private:
		union ReinterpretConversion
		{
			std::uint32_t _uint32_t;
			float _float;
			std::uint64_t _uint64_t;
			double _double;
		};

		Byte _tag = 0;
		std::vector<std::uint8_t> _value;
	};

private:
	JdwpTypes();
	~JdwpTypes();
};

#include "JdwpTypes.hpp"

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_JDWPTYPES_H_ */
