/*
 * Discover.cpp
 *
 *  Created on: 2016. 5. 31.
 *      Author: Seunghun Han
 */

#include "Discover.h"

#include <cassert>

#include <stdexcept>
#include <chrono>

#include "JikesRvm.h"

/*
 *
 *
 *
 */
bool Discover::discover(Debugger &d, const Pid pid, const std::uintptr_t bootRecord)
{
	bool success = false;

	_d = &d;

	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    for(std::intptr_t offset = 0x0; !success; offset+=0x04) {
		auto candidateObject = _d->read(pid, bootRecord+offset);
		if (_d->isValidPointer(pid, candidateObject)) {
			success = discoverObjectHeader(pid, candidateObject);
		}
	}

	if (success) {
		success = discoverTib(pid, _rootTIB);
	}

	if (success) {
		auto rvmclass_tib = _d->read(pid, _rootTIB + _tib.typeOffset);
		success = discoverRvmclass(pid, rvmclass_tib);
	}

	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	std::chrono::microseconds micro = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
	test_time = micro.count();

	if (success) {
		success = discoverJtoc(pid, bootRecord);
	}
	return success;
}

bool Discover::rescanJtoc(Pid pid)
{
	_types.clear();

	return discoverJtoc(pid, _bootRecord);
}

std::ostream& Discover::print(std::ostream& out)
{
	out << "duration1 : " << test_time1 << "microseconds" << std::endl;
	out << "duration2 : " << test_time2 << "microseconds" << std::endl;
	out << "duration3 : " << test_time3 << "microseconds" << std::endl;
	out << "duration4 : " << test_time4 << "microseconds" << std::endl;
	out << "duration5 : " << test_time5 << "microseconds" << std::endl;
	out << "duration : " << test_time << "microseconds" << std::endl;

	out << "_rootTIB : " << std::hex << _rootTIB << std::dec << std::endl;
	out << "_jtocRegister : " << std::hex << _jtocRegister << std::dec << std::endl;

	out << "_object.tibOffset : " << _object.tibOffset << std::endl;

	out << "_array.tibOffset : " << _array.tibOffset << std::endl;
	out << "_array.lengthOffset : " << _array.lengthOffset << std::endl;

	out << "_tib.typeOffset : " << _tib.typeOffset << std::endl;

	out << "_typeReference.nameOffset : " << _typeReference.nameOffset << std::endl;

	out << "_atom.valueOffset : " << _atom.valueOffset << std::endl;

	out << "_rvmClass.typeRefOffset : " <<  _rvmClass.typeRefOffset << std::endl;
	out << "_rvmClass.superClassOffset : " <<  _rvmClass.superClassOffset << std::endl;
	out << "_rvmClass.declaredFieldsOffset : " <<  _rvmClass.declaredFieldsOffset << std::endl;

	out << "_rvmMember.memRefOffset : " << _rvmMember.memRefOffset << std::endl;
	out << "_rvmMember.offsetOffset : " << _rvmMember.offsetOffset << std::endl;

	out << "_memberReference.nameOffset : " << _memberReference.nameOffset << std::endl;

	return out;
}

std::string Discover::getTypename(Pid pid, std::uintptr_t object, std::uintptr_t tib, std::uintptr_t rvmClass, std::uintptr_t typeRef)
{
	std::string str;

	if (object != 0) {
		auto address = object+_object.tibOffset;
		if (_d->isValidPointer(pid, address)) {
			tib = _d->read(pid, address);
		}
	}

	if (tib != 0) {
		auto address = tib+_tib.typeOffset;
		if (_d->isValidPointer(pid, address)) {
			rvmClass = _d->read(pid, address);
		}
	}

	if (rvmClass != 0) {
		auto address = rvmClass+_rvmClass.typeRefOffset;
		if (_d->isValidPointer(pid, address)) {
			typeRef = _d->read(pid, address);
		}
	}

	auto address = typeRef+_typeReference.nameOffset;
	if (_d->isValidPointer(pid, address)) {
		auto name = _d->read(pid, address);
		str = convertAtomToString(pid, name);
	}

	return str;
}

std::intptr_t Discover::getOffset(Pid pid, std::string classname, std::string fieldname, std::uintptr_t rvmClass)
{
	std::intptr_t ret = INTPTR_MIN;

	if (rvmClass == 0) {
		try {
			rvmClass = _types.at(classname);
		} catch (std::out_of_range &e) {
			rescanJtoc(pid);
			try {
			rvmClass = _types.at(classname);
			} catch (std::out_of_range &e) {
			}
		}
	}

	if (rvmClass != 0) {
		auto address = rvmClass + _rvmClass.declaredFieldsOffset;
		if (_d->isValidPointer(pid, address)) {
			auto declaredFields = _d->read(pid, address);
			auto len = _d->read(pid, declaredFields+_array.lengthOffset);
			for(std::uintptr_t offset = 0; offset < 4*len; offset += 4) {
				auto address = declaredFields+offset;
				if (_d->isValidPointer(pid, address)) {
					auto element = _d->read(pid, address);
					auto address = element + _rvmMember.memRefOffset;
					if (_d->isValidPointer(pid, address)) {
						auto memRef = _d->read(pid, address);
						auto address = memRef + _memberReference.nameOffset;
						if (_d->isValidPointer(pid, address)) {
							auto name = _d->read(pid, address);
							auto val = convertAtomToString(pid, name);
							if (val == fieldname) {
								auto address = element + _rvmMember.offsetOffset;
								if (_d->isValidPointer(pid, address)) {
									auto offset = _d->read(pid, address);
									ret = offset;
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	return ret;
}

std::uintptr_t Discover::getStaticField(Pid pid, std::string classname, std::string fieldname, std::uintptr_t rvmClass)
{
	std::uintptr_t ret = 0;

	if (rvmClass == 0) {
		try {
			rvmClass = _types.at(classname);
		} catch (std::out_of_range &e) {
			rescanJtoc(pid);
			rvmClass = _types.at(classname);
		}
	}

	auto staticFieldsOffset = getOffset(pid, "Lorg/jikesrvm/classloader/RVMClass;", "staticFields");

	auto address = rvmClass + staticFieldsOffset;
	if (_d->isValidPointer(pid, address)) {
		auto declaredFields = _d->read(pid, address);
		auto len = _d->read(pid, declaredFields+_array.lengthOffset);
		for(std::uintptr_t offset = 0; offset < 4*len; offset += 4) {
			auto address = declaredFields+offset;
			if (_d->isValidPointer(pid, address)) {
				auto element = _d->read(pid, address);
				auto address = element + _rvmMember.memRefOffset;
				if (_d->isValidPointer(pid, address)) {
					auto memRef = _d->read(pid, address);
					auto address = memRef + _memberReference.nameOffset;
					if (_d->isValidPointer(pid, address)) {
						auto name = _d->read(pid, address);
						auto val = convertAtomToString(pid, name);
						if (val == fieldname) {
							auto address = element + _rvmMember.offsetOffset;
							if (_d->isValidPointer(pid, address)) {
								auto offset = _d->read(pid, address);
								ret = offset + _jtocRegister;
								break;
							}
						}
					}
				}
			}
		}
	}
	return ret;
}

std::uintptr_t Discover::getRvmclass(Pid pid, std::string classname)
{
	std::uintptr_t ret = 0;

	try {
		ret = _types.at(classname);
	} catch (std::out_of_range &e) {
		rescanJtoc(pid);
		ret = _types.at(classname);
	}
	return ret;
}

bool Discover::checkType(Pid pid, std::uintptr_t object, std::string classname)
{
	if (isObject(pid, object)) {
		auto tib = _d->read(pid, object + _object.tibOffset);
		auto rvmclass = _d->read(pid, tib + _tib.typeOffset);
		return rvmclass == getRvmclass(pid, classname);
	} else {
		return false;
	}
}

bool Discover::isObject(Pid pid, const std::uintptr_t object)
{
	assert(_d->isValidPointer(pid, _rootTIB));
	assert(_d->isValidPointer(pid, _rootTIB + _object.tibOffset));
	assert(_d->read(pid, _rootTIB + _object.tibOffset) == _rootTIB);

	bool result = false;
	if (_d->isValidPointer(pid, object)) {
		if (_d->isValidPointer(pid, object + _object.tibOffset)) {
			auto tib = _d->read(pid, object + _object.tibOffset);
			result = isTIB(pid, tib);
		}
	}
	return result;
}

bool Discover::isTIB(Pid pid, const std::uintptr_t object)
{
	assert(_d->isValidPointer(pid, _rootTIB));
	assert(_d->isValidPointer(pid, _rootTIB + _object.tibOffset));
	assert(_d->read(pid, _rootTIB + _object.tibOffset) == _rootTIB);

	bool result = false;
	if (_d->isValidPointer(pid, object)) {
		if (_d->isValidPointer(pid, object + _object.tibOffset)) {
			auto test = _d->read(pid, object + _object.tibOffset);
			result = test == _rootTIB;
		}
	}
	return result;
}

bool Discover::discoverJtoc(Pid pid, const std::uintptr_t bootRecord)
{
	std::string bootRecordType = "Lorg/jikesrvm/runtime/BootRecord;";
	bool success1 = false;

	auto object = bootRecord;
	while (!isObject(pid, object)) {
		object += 4;
	}

	auto name = getTypename(pid, object);
	if (name == bootRecordType) {
		auto address = object + _object.tibOffset;
		if (_d->isValidPointer(pid, address)) {
			auto tib = _d->read(pid, address);
			auto address = tib + _tib.typeOffset;
			if (_d->isValidPointer(pid, address)) {
				auto rvm = _d->read(pid, address);
				auto jtocOffset = getOffset(pid, bootRecordType, "tocRegister", rvm);
				_jtocRegister = _d->read(pid, object + jtocOffset);
				_bootRecord = bootRecord;
				success1 = true;
			}
		}
	}


	bool success2 = false;
	if (success1) {
		static std::string tibType = "Lorg/jikesrvm/objectmodel/TIB;";
		static std::string rvmClassType = "Lorg/jikesrvm/classloader/RVMClass;";
		auto address = _jtocRegister;
		for (int i=0; i<0x20000 && _d->isValidPointer(pid, address); ++i) {
			auto test = _d->read(pid, address);
			auto name = getTypename(pid, test);
			if (name == tibType) {
				name = getTypename(pid, 0, test);
				if (!name.empty()) {
					auto address = test + _tib.typeOffset;
					if (_d->isValidPointer(pid, address)) {
						auto rvm = _d->read(pid, address);
						if (getTypename(pid, rvm) == rvmClassType) {
							_types[name] = rvm;
							success2 = true;
						}
					}
				}
			}
			address += 4;
		}
	}

	return success1 && success2;
}

bool Discover::discoverObjectHeader(Pid pid, const std::uintptr_t object)
{
	assert(_d->isValidPointer(pid, object));
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

	bool success = false;
	bool retry = true;
	for(std::intptr_t offset = 0x0; !success && retry; offset-=0x04) {
		if (_d->isValidPointer(pid, object + offset)) {
			auto tib = _d->read(pid, object + offset);
			if (_d->isValidPointer(pid, tib) && _d->isValidPointer(pid, tib + offset)) {
				auto rootTIB = _d->read(pid, tib + offset);
				if (_d->isValidPointer(pid, rootTIB) && _d->isValidPointer(pid, rootTIB + offset)) {
					auto test = _d->read(pid, rootTIB + offset);
					if (test == rootTIB) {
						_array.tibOffset = _object.tibOffset = offset;
						_rootTIB = rootTIB;
						success = true;
					}
				}
			}
		} else {
			retry = false;
		}
	}
	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	std::chrono::microseconds micro = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
	test_time1 = micro.count();
	return success;
}

bool Discover::discoverTib(Pid pid, const std::uintptr_t root_tib)
{
	assert(_d->isValidPointer(pid, root_tib));
	assert(_d->isValidPointer(pid, root_tib + _object.tibOffset));
	assert(_d->read(pid, root_tib + _object.tibOffset) == root_tib);
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

	bool success = false;
	for(std::intptr_t offset1 = _object.tibOffset + 4; !success; offset1+=0x04) {
		if (!_d->isValidPointer(pid, root_tib + offset1)) {
			break;
		}
		auto rvmclass_tib = _d->read(pid, root_tib + offset1);
		if (isTIB(pid, rvmclass_tib)) {
			break;
		}
		if (isObject(pid, rvmclass_tib)) {
			auto tib_rvmclass = _d->read(pid, rvmclass_tib + _object.tibOffset);
			auto rvmclass_rvmclass = _d->read(pid, tib_rvmclass + offset1);
			if (rvmclass_tib != rvmclass_rvmclass) {
				for(std::intptr_t offset2 = _object.tibOffset + 4; !success; offset2+=0x04) {
					if (!_d->isValidPointer(pid, rvmclass_tib + offset2)) {
						break;
					}
					auto test = _d->read(pid, rvmclass_tib + offset2);
					if (isTIB(pid, test)) {
						break;
					}
					if (isObject(pid, test)) {
						auto tib_test = _d->read(pid, test + _object.tibOffset);
						if (tib_test == tib_rvmclass) {
							_tib.typeOffset = offset1;
							_rootRvmclass = rvmclass_rvmclass;
							_rvmClass.superClassOffset = offset2;
							success = true;
						}
					}
				}
			}
		}
	}

	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	std::chrono::microseconds micro = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
	test_time2 = micro.count();
	return success;
}

bool Discover::discoverRvmclass(Pid pid, const std::uintptr_t rvmClass)
{
	bool success1 = false;

	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

	auto rvmClass_object = _d->read(pid, rvmClass+_rvmClass.superClassOffset);

	// discover offset of field "typeRef"
	for(std::intptr_t offset = _object.tibOffset + 4; !success1; offset+=0x04) {
		_rvmClass.typeRefOffset = offset;
		if (!_d->isValidPointer(pid, rvmClass_object+_rvmClass.typeRefOffset)) {
			break;
		}
		auto test = _d->read(pid, rvmClass_object+_rvmClass.typeRefOffset);
		if (isTIB(pid, test)) {
			break;
		}
		if (isObject(pid, test)) {
			success1 = discoverTypereference(pid, test);
		}
	}

	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	std::chrono::microseconds micro = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
	test_time3 = micro.count();

	start = std::chrono::system_clock::now();
	bool success2 = false;
	if (success1) {
		std::string type = "[Lorg/jikesrvm/classloader/RVMField;";
		// discover Rvmmember
		for(std::intptr_t offset = _object.tibOffset + 4; !success2; offset+=0x04) {
			if (!_d->isValidPointer(pid, rvmClass+offset)) {
				break;
			}
			auto test = _d->read(pid, rvmClass+offset);
			if (isTIB(pid, test)) {
				break;
			}
			if (isObject(pid, test)) {
				auto name = getTypename(pid, test);
				if (type == name) {
					auto element = _d->read(pid, test);
					auto tib = _d->read(pid, element+_object.tibOffset);
					auto rvmClass = _d->read(pid, tib+_tib.typeOffset);
					auto rvmmember = _d->read(pid, rvmClass+_rvmClass.superClassOffset);
					success2 = discoverRvmmember1(pid, rvmmember);
				}
			}
		}
	}
	end = std::chrono::system_clock::now();
	micro = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
	test_time4 = micro.count();


	start = std::chrono::system_clock::now();
	bool success3 = false;
	if (success2) {
		success3 = discoverRvmmember2(pid);
	}
	end = std::chrono::system_clock::now();
	micro = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
	test_time5 = micro.count();


	return success1 && success2 && success3;
}

bool Discover::discoverTypereference(Pid pid, const std::uintptr_t typeReference)
{
	bool success1 = false;
	// discover offset of field "name"
	for(std::intptr_t offset = _object.tibOffset + 4; !success1; offset+=0x04) {
		_typeReference.nameOffset = offset;
		if (!_d->isValidPointer(pid, typeReference+_typeReference.nameOffset)) {
			break;
		}
		auto test = _d->read(pid, typeReference+_typeReference.nameOffset);
		if (isTIB(pid, test)) {
			break;
		}
		if (isObject(pid, test)) {
			success1 = discoverAtom(pid, test);
		}
	}

	return success1;
}

bool Discover::discoverAtom(Pid pid, const::uintptr_t atom)
{
	bool success = false;
	// discover offset of field "value"
	for(std::intptr_t offset = _object.tibOffset + 4; !success; offset+=0x04) {
		_atom.valueOffset = offset;
		if (!_d->isValidPointer(pid, atom+_atom.valueOffset)) {
			break;
		}
		auto address = _d->read(pid, atom+_atom.valueOffset);
		if (isTIB(pid, address)) {
			break;
		}
		if (isObject(pid, address)) {
			if (_d->isValidPointer(pid, address) && (_d->read(pid, address)&0xFF) == 'L') {
				unsigned int length = 1;
				while(_d->isValidPointer(pid, address+length) && (_d->read(pid, address+length)&0xFF) != ';') {
					length++;
				}
				if (_d->isValidPointer(pid, address+length) && (_d->read(pid, address+length)&0xFF) == ';') {
					++length;
					for(std::intptr_t offset = _object.tibOffset + 4;offset < 0 && !success; offset+=0x04) {
						_array.lengthOffset = offset;
						if (_d->isValidPointer(pid, address+_array.lengthOffset)) {
							success = _d->read(pid, address+_array.lengthOffset)==length;
						}
					}
				}
			}
		}
	}
	return success;
}

bool Discover::discoverRvmmember1(Pid pid, const std::uintptr_t rvmClass)
{
	const static std::string rvmField = "[Lorg/jikesrvm/classloader/RVMField;";

	bool success = false;
	for(std::intptr_t offset = _object.tibOffset + 4; !success; offset+=0x04) {
		_rvmClass.declaredFieldsOffset = offset;
		if (!_d->isValidPointer(pid, rvmClass+offset)) {
			break;
		}
		auto test = _d->read(pid, rvmClass+offset);
		if (isTIB(pid, test)) {
			break;
		}
		if (isObject(pid, test)) {
			auto name = getTypename(pid, test);
			if (rvmField == name) {
				auto len = _d->read(pid, test+_array.lengthOffset);
				for(std::uintptr_t offset = 0; offset < 4*len && !success; offset += 4) {
					auto address = test+offset;
					if (_d->isValidPointer(pid, address)) {
						_rootOffset = _d->read(pid, address);
						for(std::intptr_t offset = _object.tibOffset + 4; !success; offset+=0x04) {
							_rvmMember.offsetOffset = offset;
							if (!_d->isValidPointer(pid, _rootOffset + _rvmMember.offsetOffset)) {
								break;
							}
							auto val = _d->read(pid, _rootOffset + _rvmMember.offsetOffset);
							if (isTIB(pid, val)) {
								break;
							}
							if (!isObject(pid, val)) {
								success = offset == static_cast<std::intptr_t>(val);
							}
						}
					}
				}
			}
		}
	}

	return success;
}
bool Discover::discoverRvmmember2(Pid pid)
{
	const static std::string fieldRef = "Lorg/jikesrvm/classloader/FieldReference;";

	bool success = false;
	for(std::intptr_t offset = _object.tibOffset + 4; !success; offset+=0x04) {
		_rvmMember.memRefOffset = offset;
		if (!_d->isValidPointer(pid, _rootOffset + _rvmMember.memRefOffset)) {
			break;
		}
		auto test = _d->read(pid, _rootOffset + _rvmMember.memRefOffset);
		if (isTIB(pid, test)) {
			break;
		}
		if (isObject(pid, test)) {
			auto name = getTypename(pid, test);
			if (name == fieldRef) {
				success = discoverFieldReference(pid, test);
			}
		}
	}
	return success;
}

bool Discover::discoverFieldReference(Pid pid, const std::uintptr_t fieldRef)
{
	const static std::string atom = "Lorg/jikesrvm/classloader/Atom;";

	bool success = false;
	for(std::intptr_t offset = _object.tibOffset + 4; !success; offset+=0x04) {
		_memberReference.nameOffset = offset;
		if (!_d->isValidPointer(pid, fieldRef + _memberReference.nameOffset)) {
			break;
		}
		auto test = _d->read(pid, fieldRef + _memberReference.nameOffset);
		if (isTIB(pid, test)) {
			break;
		}
		if (isObject(pid, test)) {
			auto name = getTypename(pid, test);
			if (atom == name) {
				auto val = convertAtomToString(pid, test);
				success = val == "offset";
			}
		}
	}

	return success;
}

std::string Discover::convertAtomToString(Pid pid, const std::uintptr_t atom)
{
	std::string str;
	auto address = atom+_atom.valueOffset;
	if (_d->isValidPointer(pid, address)) {
		auto val = _d->read(pid, address);
		auto address = val+_array.lengthOffset;
		if (_d->isValidPointer(pid, address)) {
			auto length = _d->read(pid, address);

			std::string s;
			for (std::size_t offset=0; offset < length; offset++) {
				if (_d->isValidPointer(pid, val+offset)) {
					char c = _d->read(pid, val+offset);
					s.push_back(c);
				}
			}
			if (s.size() == length) {
				str = s;
			}
		}
	}
	return str;
}


