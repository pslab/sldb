/*
 * JikesRvm.h
 *
 *  Created on: 2016. 3. 10.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_VM_JIKESRVM_JIKESRVM_H_
#define JIKESDBG_SRC_VM_JIKESRVM_JIKESRVM_H_

#include "../Vm.h"

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cinttypes>

#include "../../Debugger.h"
#include "Discover.h"

class JikesRvm: public Vm {
public:
	typedef std::int32_t ObjectID;
	typedef std::int32_t ReferenceTypeID;
	typedef std::int32_t MethodID;
	typedef std::int32_t FieldID;
	typedef std::int32_t FrameID;

	typedef ObjectID ThreadID;
	typedef ObjectID ThreadGroupID;
	typedef ObjectID StringID;
	typedef ObjectID ClassLoaderID;
	typedef ObjectID ClassObjectID;
	typedef ObjectID ArrayID;
	typedef ReferenceTypeID ClassID;
	typedef ReferenceTypeID InterfaceID;
	typedef ReferenceTypeID ArrayTypeID;

	struct ThreadInformation {
		Pid pid = 0;
		std::uintptr_t tr = 0;
		ThreadID tid = 0;
		std::uintptr_t thread = 0;
		std::u16string name;
	};

	struct ThreadGroupInformation {
		std::uintptr_t group = 0;
		ThreadGroupID gid = 0;
		ThreadGroupID parent = 0;
		std::u16string name;
		std::vector<ThreadID> threads;
		std::vector<ThreadGroupID> groups;
	};

	static const std::string DESCRIPTION;
	static const std::string VERSION;
	static const std::string NAME;

	static bool canWatchFieldModification;
	static bool canWatchFieldAccess;
	static bool canGetBytecodes;
	static bool canGetSyntheticAttribute;
	static bool canGetOwnedMonitorInfo;
	static bool canGetCurrentContendedMonitor;
	static bool canGetMonitorInfo;
	static bool canRedefineClasses;
	static bool canAddMethod;
	static bool canUnrestrictedlyRedefineClasses;
	static bool canPopFrames;
	static bool canUseInstanceFilters;
	static bool canGetSourceDebugExtension;
	static bool canRequestVMDeathEvent;
	static bool canSetDefaultStratum;
	static bool canGetInstanceInfo;
	static bool canRequestMonitorEvents;
	static bool canGetMonitorFrameInfo;
	static bool canUseSourceNameFilters;
	static bool canGetConstantPool;
	static bool canForceEarlyReturn;

	static ThreadID start(Debugger &d);
	static std::vector<ThreadID> getAllThreadID();
	static ThreadInformation getThreadInformation(ThreadID threadID);
	static ThreadGroupInformation getThreadGroupInformation(ThreadGroupID threadGroupID);
	static ThreadID threadStart(Debugger &d, Pid pid);
	static ThreadID threadDeath(Debugger &d, Pid pid);
	static ThreadID setThread(Debugger &d, Pid pid, std::uintptr_t tr);
	static ThreadGroupID setThreadGroup(Debugger &d, Pid pid, ThreadID tid);
	static ReferenceType getObjectReferenceType(Debugger &d, ObjectID objectID);
	static ReferenceTypeID getObjectReferenceID(Debugger &d, ObjectID objectID);
	static ClassID getSuperclassID(Debugger &d, ClassID classID);
	static FrameID getFramePointer(Debugger &d, ThreadID threadID);
	static MethodID getMethodID(Debugger &d, ThreadID threadID, FrameID frameID);
	static std::string getTypename(Pid pid, std::uintptr_t object);
	static std::unordered_set<std::string> getClassnames(Pid pid);
	static ReferenceTypeID getClassReferenceID(Debugger &d, std::string signature);
	static ClassStatus getClassStatus(Debugger &d, std::string signature);

private:
	JikesRvm();
	virtual ~JikesRvm();

	static std::vector<std::string> makeArgs();


	static Discover _discover;

	// native global function
	static std::uintptr_t _main;
	static std::uintptr_t _sysThreadCreate;
	static std::uintptr_t _sysThreadTerminate;

	static std::uintptr_t _bootRecord;

	static std::uintptr_t _RVMClass;
	static std::uintptr_t _RVMArray;


	static std::intptr_t _threadOffsetFromThreadRVM;
	static std::intptr_t _nameOffsetFromThreadRVM;
	static std::intptr_t _framepointerOffsetFromRVMThread;

	static std::intptr_t _groupOffsetFromThread;
	static std::intptr_t _nameOffsetFromThread;

	static std::intptr_t _parentOffsetFromGroup;
	static std::intptr_t _nameOffsetFromGroup;
	static std::intptr_t _threadsOffsetFromGroup;
	static std::intptr_t _groupsOffsetFromGroup;

	static std::intptr_t _valueOffsetFromString;
	static std::intptr_t _countOffsetFromString;

	static std::intptr_t _valueOffsetFromVector;
	static std::intptr_t _countOffsetFromVector;

	static std::vector<std::uintptr_t> _objIdToAddr;
	static std::unordered_map<ReferenceTypeID, std::uintptr_t> _refIdToAddr;

	static std::unordered_map<Pid, ThreadID> _pidToTid;
	static std::unordered_map<ThreadID, ThreadInformation> _threadInformations;
	static std::unordered_map<ThreadGroupID, ThreadGroupInformation> _threadGroupInformations;
};

#endif /* JIKESDBG_SRC_VM_JIKESRVM_JIKESRVM_H_ */
