/*
 * Discover.h
 *
 *  Created on: 2016. 5. 31.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_VM_JIKESRVM_DISCOVER_H_
#define JIKESDBG_SRC_VM_JIKESRVM_DISCOVER_H_

#include <cstdint>
#include <ostream>
#include <string>
#include <unordered_map>

#include "../../Debugger.h"

class Discover {
public:
	Discover() : _rootTIB(0), _rootRvmclass(0), _rootOffset(0), _jtocRegister(0), test_time1(0), test_time2(0), test_time3(0), test_time4(0), test_time5(0), test_time(0), _d(nullptr), _bootRecord(0) {
	}
	virtual ~Discover() {
	}

	virtual bool discover(Debugger &d, const Pid pid, const std::uintptr_t bootRecord);
	virtual bool rescanJtoc(Pid pid);

	virtual std::ostream& print(std::ostream& out);

	std::string getTypename(Pid pid, std::uintptr_t object, std::uintptr_t tib = 0, std::uintptr_t rvmClass = 0, std::uintptr_t typeRef = 0);
	std::intptr_t getOffset(Pid pid, std::string classname, std::string fieldname, std::uintptr_t rvmClass = 0);
	std::uintptr_t getStaticField(Pid pid, std::string classname, std::string fieldname, std::uintptr_t rvmClass = 0);
	std::uintptr_t getRvmclass(Pid pid, std::string classname);
	bool checkType(Pid pid, std::uintptr_t object, std::string classname);
	virtual bool isObject(Pid pid, const std::uintptr_t object);
	virtual bool isTIB(Pid pid, const std::uintptr_t object);

	std::uintptr_t _rootTIB;
	std::uintptr_t _rootRvmclass;
	std::uintptr_t _rootOffset;
	std::uintptr_t _jtocRegister;
	std::unordered_map<std::string, std::uintptr_t> _types;

	struct Object {
		std::intptr_t tibOffset;
	} _object;

	struct Array {
		std::intptr_t tibOffset;
		std::intptr_t lengthOffset;
	} _array;

	struct TIB {
		std::intptr_t typeOffset;
	} _tib;

	struct RVMClass {
		std::intptr_t typeRefOffset;
		std::intptr_t superClassOffset;
		std::intptr_t declaredFieldsOffset;
	} _rvmClass;

	struct TypeReference {
		std::intptr_t nameOffset;
	} _typeReference;

	struct Atom {
		std::intptr_t valueOffset;
	} _atom;

	struct RVMMember {
		std::intptr_t memRefOffset;
		std::intptr_t offsetOffset;
	} _rvmMember;

	struct MemberReference {
		std::intptr_t nameOffset;
	} _memberReference;

protected:
	virtual bool discoverJtoc(Pid pid, const std::uintptr_t bootRecord);
	virtual bool discoverObjectHeader(Pid pid, const std::uintptr_t object);
	virtual bool discoverTib(Pid pid, const std::uintptr_t rootTib);
	virtual bool discoverRvmclass(Pid pid, const std::uintptr_t rvmClass);
	virtual bool discoverTypereference(Pid pid, const std::uintptr_t typeReference);
	virtual bool discoverAtom(Pid pid, const::uintptr_t atom);
	virtual bool discoverRvmmember1(Pid pid, const std::uintptr_t rvmClass);
	virtual bool discoverRvmmember2(Pid pid);
	virtual bool discoverFieldReference(Pid pid, const std::uintptr_t fieldRef);

	std::string convertAtomToString(Pid pid, const std::uintptr_t atom);

	double test_time1;
	double test_time2;
	double test_time3;
	double test_time4;
	double test_time5;
	double test_time;
	Debugger *_d;
	std::uintptr_t _bootRecord;
};

#endif /* JIKESDBG_SRC_VM_JIKESRVM_DISCOVER_H_ */
