/*
 * JikesRvm.cpp
 *
 *  Created on: 2016. 3. 8.
 *      Author: Seunghun Han
 */

#include "JikesRvm.h"

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <system_error>
#include <chrono>

#include <unistd.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <signal.h>

#include "../../ElfReader.h"
#include "../../CommandLineArgs.h"
#include "../../Debuggee.h"

const std::string JikesRvm::DESCRIPTION = "Jikes RVM";
const std::string JikesRvm::VERSION = "1.6.0";
const std::string JikesRvm::NAME = "JikesRVM";

bool JikesRvm::canWatchFieldModification = false;
bool JikesRvm::canWatchFieldAccess = false;
bool JikesRvm::canGetBytecodes = false;
bool JikesRvm::canGetSyntheticAttribute = false;
bool JikesRvm::canGetOwnedMonitorInfo = false;
bool JikesRvm::canGetCurrentContendedMonitor = false;
bool JikesRvm::canGetMonitorInfo = false;
bool JikesRvm::canRedefineClasses = false;
bool JikesRvm::canAddMethod = false;
bool JikesRvm::canUnrestrictedlyRedefineClasses = false;
bool JikesRvm::canPopFrames = false;
bool JikesRvm::canUseInstanceFilters = false;
bool JikesRvm::canGetSourceDebugExtension = false;
bool JikesRvm::canRequestVMDeathEvent = true;
bool JikesRvm::canSetDefaultStratum = false;
bool JikesRvm::canGetInstanceInfo = false;
bool JikesRvm::canRequestMonitorEvents = false;
bool JikesRvm::canGetMonitorFrameInfo = false;
bool JikesRvm::canUseSourceNameFilters = false;
bool JikesRvm::canGetConstantPool = false;
bool JikesRvm::canForceEarlyReturn = false;

Discover JikesRvm::_discover;

// native global function
std::uintptr_t JikesRvm::_main;
std::uintptr_t JikesRvm::_sysThreadCreate ;
std::uintptr_t JikesRvm::_sysThreadTerminate;

std::uintptr_t JikesRvm::_bootRecord;

std::uintptr_t JikesRvm::_RVMClass;
std::uintptr_t JikesRvm::_RVMArray;

std::intptr_t JikesRvm::_threadOffsetFromThreadRVM;
std::intptr_t JikesRvm::_nameOffsetFromThreadRVM;
std::intptr_t JikesRvm::_framepointerOffsetFromRVMThread;

std::intptr_t JikesRvm::_groupOffsetFromThread;
std::intptr_t JikesRvm::_nameOffsetFromThread;
std::intptr_t JikesRvm::_parentOffsetFromGroup;
std::intptr_t JikesRvm::_nameOffsetFromGroup;
std::intptr_t JikesRvm::_threadsOffsetFromGroup;
std::intptr_t JikesRvm::_groupsOffsetFromGroup;
std::intptr_t JikesRvm::_valueOffsetFromString;
std::intptr_t JikesRvm::_countOffsetFromString;
std::intptr_t JikesRvm::_valueOffsetFromVector;
std::intptr_t JikesRvm::_countOffsetFromVector;

std::vector<std::uintptr_t> JikesRvm::_objIdToAddr;
std::unordered_map<JikesRvm::ReferenceTypeID, std::uintptr_t> JikesRvm::_refIdToAddr;

std::unordered_map<Pid, JikesRvm::ThreadID> JikesRvm::_pidToTid;
std::unordered_map<JikesRvm::ThreadID, JikesRvm::ThreadInformation> JikesRvm::_threadInformations;
std::unordered_map<JikesRvm::ThreadGroupID, JikesRvm::ThreadGroupInformation> JikesRvm::_threadGroupInformations;


JikesRvm::ThreadID JikesRvm::start(Debugger &d)
{
	ThreadID threadID = -1;

	auto vmPath = CommandLineArgs::getVmPath();
	auto args = makeArgs();

	ElfReader symbol(vmPath);
	_main = symbol.getAddress("main");
	auto got_mmap = symbol.getGotAddress("mmap");
	if (got_mmap == 0) {
		got_mmap = symbol.getGotAddress("mmap64");
	}
	auto got_sigaction = symbol.getGotAddress("sigaction");
	_sysThreadCreate = symbol.getAddress("sysThreadCreate");
	_sysThreadTerminate = symbol.getAddress("sysThreadTerminate");


	Debuggee debuggee(vmPath, args);
	debuggee.run();

	auto pid = d.start();
	// set breakpoints.
	std::cout << "_main : " << std::hex <<  _main << std::dec << std::endl;
	d.setBreakpoint(pid, _main);
	d.cont(pid);

	pid = d.wait();
	assert( d.readEip(pid) - 1 == _main);

	d.removeBreakpoint(pid, _main);
	d.writeEip(pid, d.readEip(pid)-1);


	std::uintptr_t mmap = 0;
	if (got_mmap != 0) {
		mmap = d.read(pid, got_mmap);
		std::cout << "mmap : " << std::hex <<  mmap << std::dec << std::endl;
		d.setBreakpoint(pid, mmap);
	}

	std::uintptr_t sigaction = 0;
	if (got_sigaction != 0) {
		sigaction = d.read(pid, got_sigaction);
		std::cout << "sigaction : " << std::hex <<  sigaction << std::dec << std::endl;
		d.setBreakpoint(pid, sigaction);
	}

	if (got_mmap != 0) {
		d.cont(pid);

		pid = d.wait();
		assert( d.readEip(pid) - 1 == mmap);
		auto esp = d.readEsp(pid);
		_bootRecord = d.read(pid, esp+4);
		std::cout << "(got) ";
	} else {
  		d.syscall(pid);
		bool done = false;
		while (!done) {
			int status;
			pid = d.wait(&status);
			if (WSTOPSIG(status) & 0x80) {
			  int  syscall_num = d.readSysCallNumber(pid);
			  if (syscall_num == 192 || syscall_num == 90) {
		  		_bootRecord = d.readSysCallArg1(pid);

		  		done = true;
			  }
			  d.syscall(pid);
			} else {
			  d.cont(pid);
			}
		}
		pid = d.wait();
		std::cout << "(syscall) ";
	}

	std::cout << "_bootRecord : " << std::hex <<  _bootRecord << std::dec << std::endl;

	if (got_sigaction != 0) {
		d.cont(pid);
		pid = d.wait();
		assert( d.readEip(pid) - 1 == sigaction);
		auto eip = d.readEip(pid) - 1;
		if (sigaction == eip) {
			auto esp = d.readEsp(pid);
			auto sig = d.read(pid, esp+4);
			if (sig == SIGTRAP) {
				d.write(pid, esp+4, SIGSEGV);
			} else {
				do {
					d.cont(pid);
					pid = d.wait();
					auto new_sigaction = d.read(pid, got_sigaction);
					if (new_sigaction != sigaction) {
						sigaction = new_sigaction;
						d.setBreakpoint(pid, sigaction);
					}

					eip = d.readEip(pid) - 1;
					if (sigaction == eip) {
						auto esp = d.readEsp(pid);
						auto sig = d.read(pid, esp+4);
						if (sig == SIGTRAP) {
							d.write(pid, esp+4, SIGFPE);
						}
					}
				} while (sigaction == eip);
			}
		}
		std::cout << "(got) ";
	} else {
		d.syscall(pid);
		bool done = false;
		while (!done) {
			int status;
			pid = d.wait(&status);
			if (WSTOPSIG(status) & 0x80) {
		  	int  syscall_num = d.readSysCallNumber(pid);
		  	if (syscall_num == 174 || syscall_num == 67) {
		  		int sig = d.readSysCallArg1(pid);
		  		if (sig == SIGTRAP) {
			 	 d.writeSysCallArg1(pid, SIGFPE);
			  	done = true;
		  		}
		  	}
		  	d.syscall(pid);
			} else {
		  	d.cont(pid);
			}
		}
		pid = d.wait();
		std::cout << "(syscall) ";
	}
	std::cout << "handler for SIGTRAP removed" << std::endl;

	bool r = _discover.discover(d, pid, _bootRecord);
	_discover.print(std::cout);

	if (r == true) {
		_RVMClass = _discover.getRvmclass(pid,"Lorg/jikesrvm/classloader/RVMClass;");
		_RVMArray = _discover.getRvmclass(pid,"Lorg/jikesrvm/classloader/RVMArray;");


		_threadOffsetFromThreadRVM = _discover.getOffset(pid, "Lorg/jikesrvm/scheduler/RVMThread;", "thread");
		std::cout << "_threadOffsetFromThreadRVM : " << _threadOffsetFromThreadRVM << std::endl;

		_nameOffsetFromThreadRVM = _discover.getOffset(pid, "Lorg/jikesrvm/scheduler/RVMThread;", "name");
		std::cout << "_nameOffsetFromThreadRVM : " << _nameOffsetFromThreadRVM  << std::endl;

		_framepointerOffsetFromRVMThread = _discover.getOffset(pid, "Lorg/jikesrvm/scheduler/RVMThread;", "framePointer");
		std::cout << "_framepointerOffsetFromRVMThread : " << _framepointerOffsetFromRVMThread << std::endl;

		_groupOffsetFromThread = _discover.getOffset(pid, "Ljava/lang/Thread;", "group");
		std::cout << "_groupOffsetFromThread : " << _groupOffsetFromThread << std::endl;

		_nameOffsetFromThread = _discover.getOffset(pid, "Ljava/lang/Thread;", "name");
		std::cout << "_nameOffsetFromThread : " << _nameOffsetFromThread << std::endl;

		_parentOffsetFromGroup = _discover.getOffset(pid, "Ljava/lang/ThreadGroup;", "parent");
		std::cout << "_parentOffsetFromGroup : " << _parentOffsetFromGroup << std::endl;

		_nameOffsetFromGroup = _discover.getOffset(pid, "Ljava/lang/ThreadGroup;", "name");
		std::cout << "_nameOffsetFromGroup : " << _nameOffsetFromGroup << std::endl;

		_threadsOffsetFromGroup = _discover.getOffset(pid, "Ljava/lang/ThreadGroup;", "threads");
		std::cout << "_threadsOffsetFromGroup : " << _threadsOffsetFromGroup << std::endl;

		_groupsOffsetFromGroup = _discover.getOffset(pid, "Ljava/lang/ThreadGroup;", "groups");
		std::cout << "_groupsOffsetFromGroup : " << _groupsOffsetFromGroup << std::endl;

		_valueOffsetFromString = _discover.getOffset(pid, "Ljava/lang/String;", "value");
		std::cout << "_valueOffsetFromString : " << _valueOffsetFromString << std::endl;

		_countOffsetFromString = _discover.getOffset(pid, "Ljava/lang/String;", "count");
		std::cout << "_countOffsetFromString : " << _countOffsetFromString << std::endl;

		_valueOffsetFromVector = _discover.getOffset(pid, "Ljava/util/Vector;", "elementData");
		std::cout << "_valueOffsetFromVector : " << _valueOffsetFromVector << std::endl;

		_countOffsetFromVector = _discover.getOffset(pid, "Ljava/util/Vector;", "elementCount");
		std::cout << "_countOffsetFromVector : " << _countOffsetFromVector << std::endl;

		if (_sysThreadCreate == 0 || _sysThreadTerminate == 0) {
			auto bootRecord = _bootRecord;
			while (!_discover.isObject(pid, bootRecord)) {
				bootRecord += 4;
			}
			auto offset_threadCreate = _discover.getOffset(pid, "Lorg/jikesrvm/runtime/BootRecord;", "sysThreadCreateIP");
			_sysThreadCreate = d.read(pid, bootRecord + offset_threadCreate);
			std::cout << "_sysThreadCreate : " << std::hex << _sysThreadCreate << std::dec << std::endl;

			auto offset_threadTerminate = _discover.getOffset(pid, "Lorg/jikesrvm/runtime/BootRecord;", "sysThreadTerminateIP");
			_sysThreadTerminate = d.read(pid, bootRecord + offset_threadTerminate);
			std::cout << "_sysThreadTerminate : " << std::hex << _sysThreadTerminate << std::dec << std::endl;
		}

		d.setBreakpoint(pid, _sysThreadCreate);
		d.setBreakpoint(pid, _sysThreadTerminate);

		std::cout << "-- debugger initialized --" << std::endl;

		do {
			d.cont(pid);
			pid = d.wait();
		} while (d.readEip(pid) - 1 != _sysThreadCreate);
		threadID = threadStart(d, pid);

		std::cout << "-- thread(" << threadID << ") initialized --" << std::endl;


	} else {
		std::cerr << "fail to discover" << std::endl;
	}

	d.cont(pid);

	return threadID;
}

std::vector<JikesRvm::ThreadID> JikesRvm::getAllThreadID()
{
	std::vector<ThreadID> ids;

	for (auto &pair : _threadInformations) {
		ids.push_back(pair.first);
	}

	return ids;
}

JikesRvm::ThreadInformation JikesRvm::getThreadInformation(ThreadID threadID)
{
	return _threadInformations.at(threadID);
}

JikesRvm::ThreadGroupInformation JikesRvm::getThreadGroupInformation(ThreadGroupID threadGroupID)
{
	return _threadGroupInformations.at(threadGroupID);
}

JikesRvm::ThreadID JikesRvm::threadStart(Debugger &d, Pid pid)
{
	ThreadID tid = -1;

	auto eip = d.readEip(pid) - 1;
	if (eip == _sysThreadCreate) {
		static std::intptr_t trOffset = -1;
		auto esp = d.readEsp(pid);
		//  Address sysThreadCreate(...)
		if (trOffset < 0) {
			for (trOffset=0; trOffset<0x10; trOffset+=4) {
				auto address = esp + trOffset;
				if (d.isValidPointer(pid, address)) {
					std::uintptr_t tr = d.read(pid, address);
					if (tr != 0) {
						tid = setThread(d, pid, tr);
						if (tid != -1) {
							break;
						}
					}
				}
			}
		} else {
			std::uintptr_t tr = d.read(pid, esp + trOffset);
			if (tr != 0) {
				tid = setThread(d, pid, tr);
			}
		}
	}
	return tid;
}


JikesRvm::ThreadID JikesRvm::threadDeath(Debugger &d, Pid pid)
{
	ThreadID tid = -1;

	auto eip = d.readEip(pid) - 1;
	if (eip == _sysThreadTerminate) {
		tid = _pidToTid.at(pid);
		_objIdToAddr[tid-1] = 0;
		_threadInformations.erase(tid);
		_pidToTid.erase(pid);
	}

	return tid;
}

JikesRvm::ThreadID JikesRvm::setThread(Debugger &d, Pid pid, std::uintptr_t tr)
{
	ThreadID tid = -1;

	if (tr != 0) {
		auto address = tr + _threadOffsetFromThreadRVM;
		if (d.isValidPointer(pid, address)) {
			std::uintptr_t thread = d.read(pid, address);

			ThreadInformation t;
			if (thread != 0) {
				t.pid = pid;
				t.tr = tr;
				t.thread = thread;

				auto address = tr + _nameOffsetFromThreadRVM;
				if (d.isValidPointer(pid, address)) {
				std::uintptr_t name = d.read(pid, address);
					if (name != 0) {
						auto address = name + _countOffsetFromString;
						if (d.isValidPointer(pid, address)) {
							auto count = d.read(pid, address);
							auto address = name + _valueOffsetFromString;
							if (d.isValidPointer(pid, address)) {
								auto value = d.read(pid, address);
								for(std::size_t i=0; i<count; i++) {
									char16_t c = d.read(pid, value + i*2);
									t.name.push_back(c);
								}
							}
						}
					}
				}

				if (!t.name.empty()) {
					_objIdToAddr.push_back(thread);
					_pidToTid[pid] = tid = t.tid = _objIdToAddr.size();
					_threadInformations[tid] = t;
				}
			}
		}
	}

	return tid;
}

JikesRvm::ThreadGroupID JikesRvm::setThreadGroup(Debugger &d, Pid pid, ThreadID tid)
{
	ThreadGroupID gid = 0;

	if (tid != 0) {
		auto thread = _objIdToAddr[tid-1];
		if (thread != 0) {
			auto group = d.read(pid, thread + _groupOffsetFromThread);

			ThreadGroupInformation g;

			if (group != 0) {
				g.group = group;
				_objIdToAddr.push_back(group);
				g.gid = gid = _objIdToAddr.size();

				g.parent = d.read(pid, group + _parentOffsetFromGroup);

				auto name = d.read(pid, group + _nameOffsetFromGroup);
				if (name != 0) {
					auto count = d.read(pid, name + _countOffsetFromString);
					auto value = d.read(pid, name + _valueOffsetFromString);
					for(std::size_t i=0; i<count; i++) {
						char16_t c = d.read(pid, value + i*2);
						g.name.push_back(c);
					}
				}

				auto threads = d.read(pid, group + _threadsOffsetFromGroup);
				if (threads != 0) {
					auto count = d.read(pid, threads + _countOffsetFromVector);
					auto value = d.read(pid, threads + _valueOffsetFromVector);
					for(std::size_t i=0; i < count; i++) {
						auto p = value + i*4;
						auto tid = d.read(pid, p);
						g.threads.push_back(tid);
					}
				}

				auto groups = d.read(pid, group + _groupsOffsetFromGroup);
				if (groups != 0) {
					auto count = d.read(pid, groups + _countOffsetFromVector);
					auto value = d.read(pid, groups + _valueOffsetFromVector);
					for(std::size_t i=0; i < count; i++) {
						auto p = value + i*4;
						std::uintptr_t tid = d.read(pid, p);
						g.groups.push_back(tid);
					}
				}
				_threadGroupInformations[gid] = g;
			}
		}
	}

	return gid;
}

JikesRvm::ReferenceType JikesRvm::getObjectReferenceType(Debugger &d, ObjectID objectID)
{
	JikesRvm::ReferenceType type = NONE;

	auto pids = d.getPids();
	for (auto &pid : pids) {
		bool success = false;
		bool old = d.isSuspended(pid);
		bool stopped = d.suspend(pid);

		if (stopped == true) {
			try {
				auto object = _objIdToAddr[objectID-1];
				auto tib = d.read(pid, object+_discover._object.tibOffset);
				auto rvmClass = d.read(pid, tib + _discover._tib.typeOffset);
				auto tibRvmClass = d.read(pid, rvmClass+_discover._object.tibOffset);
				auto rvmRvmClass = d.read(pid, tibRvmClass+_discover._tib.typeOffset);
				if (rvmRvmClass == _RVMClass) {
					type = ReferenceType::CLASS;
				} else if (rvmRvmClass == _RVMArray) {
					type = ReferenceType::ARRAY;
				} else {
					type = ReferenceType::INTERFACE;
				}
				success = true;
			} catch (std::exception &e) {
			}
			if (!old) {
				d.resume(pid);
			}
		}
		if (success == true) {
			break;
		}
	}
	return type;
}

JikesRvm::ReferenceTypeID JikesRvm::getObjectReferenceID(Debugger &d, ObjectID objectID)
{
	ReferenceTypeID id = 0;

	auto pids = d.getPids();
	for (auto &pid : pids) {
		bool success = false;
		bool old = d.isSuspended(pid);
		bool stopped = d.suspend(pid);

		if (stopped == true) {
			try {
				auto object = _objIdToAddr[objectID-1];
				auto tib = d.read(pid, object +_discover._object.tibOffset);
				auto rvm = d.read(pid, tib + _discover._tib.typeOffset);
				auto address = rvm + _discover._rvmClass.typeRefOffset;
				if (d.isValidPointer(pid, address)) {
					auto typeRef = d.read(pid, address);
					auto idOffset = _discover.getOffset(pid, "Lorg/jikesrvm/classloader/TypeReference;", "id");
					auto address = typeRef + idOffset;
					if (idOffset != INTPTR_MIN && d.isValidPointer(pid, address)) {
						id = d.read(pid, address);
						_refIdToAddr[id] = rvm;
						success = true;
					}
					success = true;
				}
			} catch (std::exception &e) {
			}
			if (!old) {
				d.resume(pid);
			}
		}
		if (success == true) {
			break;
		}
	}
	return id;
}

JikesRvm::ClassID JikesRvm::getSuperclassID(Debugger &d, ClassID classID)
{
	ClassID id = 0;

	auto pids = d.getPids();
	for (auto &pid : pids) {
		bool success = false;
		bool old = d.isSuspended(pid);
		bool stopped = d.suspend(pid);

		if (stopped == true) {
			try {
				auto rvmClass = _refIdToAddr.at(classID);
				auto superclass = d.read(pid, rvmClass + _discover._rvmClass.superClassOffset);
				auto signature = _discover.getTypename(pid, 0, 0, superclass, 0);
				id = getClassReferenceID(d, signature);
				success = true;
			} catch (std::exception &e) {
			}
			if (!old) {
				d.resume(pid);
			}
		}
		if (success == true) {
			break;
		}
	}
	return id;
}

JikesRvm::FrameID JikesRvm::getFramePointer(Debugger &d, ThreadID threadID)
{
	auto t = getThreadInformation(threadID);
	auto pid = t.pid;
	FrameID frameID  = d.read(pid, t.tr+ _framepointerOffsetFromRVMThread);

	return frameID;
}

JikesRvm::MethodID JikesRvm::getMethodID(Debugger &d, ThreadID threadID, FrameID frameID)
{
	auto t = getThreadInformation(threadID);
	auto pid = t.pid;
	MethodID mid  = d.read(pid, frameID-0x04);
	return mid;
}

std::string JikesRvm::getTypename(Pid pid, std::uintptr_t object)
{
	return _discover.getTypename(pid, object);
}

std::unordered_set<std::string> JikesRvm::getClassnames(Pid pid)
{
	std::unordered_set<std::string> result;

	_discover.rescanJtoc(pid);
	for (auto &pair : _discover._types) {
		result.insert(pair.first);
	}

	return result;
}

JikesRvm::ReferenceTypeID JikesRvm::getClassReferenceID(Debugger &d, std::string signature)
{
	ReferenceTypeID id = 0;

	auto pids = d.getPids();
	for (auto &pid : pids) {
		bool success = false;
		bool old = d.isSuspended(pid);
		bool stopped = d.suspend(pid);

		if (stopped == true) {

			std::uintptr_t rvm = 0;
			try {
				rvm = _discover._types.at(signature);
			} catch (std::out_of_range &e) {

			}

			if (rvm != 0) {
				auto address = rvm + _discover._rvmClass.typeRefOffset;
				if (d.isValidPointer(pid, address)) {
					auto typeRef = d.read(pid, address);
					static auto idOffset = _discover.getOffset(pid, "Lorg/jikesrvm/classloader/TypeReference;", "id");
					auto address = typeRef + idOffset;
					if (idOffset != INTPTR_MIN && d.isValidPointer(pid, address)) {
						id = d.read(pid, address);
						_refIdToAddr[id] = rvm;
						success = true;
					}
				}
			}

			if (!old) {
				d.resume(pid);
			}
		}
		if (success == true) {
			break;
		}
	}
	return id;
}

JikesRvm::ClassStatus JikesRvm::getClassStatus(Debugger &d, std::string signature)
{
	ClassStatus status = ClassStatus::ERROR;

	auto pids = d.getPids();
	for (auto &pid : pids) {
		bool success = false;
		bool old = d.isSuspended(pid);
		bool stopped = d.suspend(pid);

		if (stopped == true) {
			std::uintptr_t rvm = 0;
			try {
				rvm = _discover._types.at(signature);
			} catch (std::out_of_range &e) {
			}
			if (rvm != 0) {
				status = ClassStatus::PREPARED;
				auto stateOffset = _discover.getOffset(pid, "Lorg/jikesrvm/classloader/RVMClass;", "state");
				if (stateOffset != INTPTR_MIN) {
					auto address = rvm + stateOffset;
					if (d.isValidPointer(pid, address)) {
						std::int8_t state = d.read(pid, rvm + stateOffset);

						int initializedAddr = _discover.getStaticField(pid, "Lorg/jikesrvm/classloader/ClassLoaderConstants;", "CLASS_INITIALIZED");
						if (d.isValidPointer(pid, initializedAddr)) {
							int initialized = d.read(pid, initializedAddr);
							if (state == initialized) {
								status = ClassStatus::INITIALIZED;
							}
							int failedAddr = _discover.getStaticField(pid, "Lorg/jikesrvm/classloader/ClassLoaderConstants;", "CLASS_INITIALIZER_FAILED");
							if (d.isValidPointer(pid, failedAddr)) {
								int failed = d.read(pid, failedAddr);
								success = true;
								if (state == failed) {
									status = ClassStatus::ERROR;
								}
							}
						}
					}

				}
			}

			if (!old) {
				d.resume(pid);
			}
		}
		if (success == true) {
			break;
		}
	}

	return status;
}

std::vector<std::string> JikesRvm::makeArgs()
{
	FILE *fd = popen("date +%Z", "r");
	char buf[256];
	int read = fscanf(fd, "%255s", buf);
	assert(read == 1);
	std::string timezone = buf;
	fclose(fd);

	fd = popen("echo $HOME", "r");
	read = fscanf(fd, "%255s", buf);
	assert(read == 1);
	std::string home = buf;
	fclose(fd);

	// std::string pwd = getcwd(buf, 256);

	fd = popen("whoami", "r");
	read = fscanf(fd, "%255s", buf);
	assert(read == 1);
	std::string whoami = buf;
	fclose(fd);

	fd = popen("uname -s", "r");
	read = fscanf(fd, "%255s", buf);
	assert(read == 1);
	std::string uname_s = buf;
	fclose(fd);

	fd = popen("uname -r", "r");
	read = fscanf(fd, "%255s", buf);
	assert(read == 1);
	std::string uname_r = buf;
	fclose(fd);

	fd = popen("uname -m | tr ' ' '_'", "r");
	read = fscanf(fd, "%255s", buf);
	assert(read == 1);
	std::string uname_m = buf;
	fclose(fd);

	auto vmPath = CommandLineArgs::getVmPath();
	auto rvmHome = vmPath.substr(0, vmPath.find_last_of('/'));

	std::string baseArgs = "-X:ic="+rvmHome+"/RVM.code.image\n"
			"-X:id="+rvmHome+"/RVM.data.image\n"
			"-X:ir="+rvmHome+"/RVM.rmap.image\n"
			"-X:vmClasses="+rvmHome+"/jksvm.jar:"+rvmHome+"/rvmrt.jar\n"
			"-Duser.timezone="+timezone+"\n"
			"-Djava.home="+rvmHome+"\n"
			"-Dgnu.classpath.home.url=file:"+rvmHome+"\n"
			"-Dgnu.classpath.vm.shortname=JikesRVM\n"
			"-Duser.home="+home+"\n"
			"-Duser.dir="+rvmHome+"\n"
			"-Duser.name="+whoami+"\n"
			"-Dos.name="+uname_s+" \n"
			"-Dos.version="+uname_r+"\n"
			"-Dos.arch="+uname_m;


	std::string ldLibraryPath;
	auto ldpath = getenv("LD_LIBRARY_PATH");
	if (ldpath == nullptr) {
	  ldLibraryPath = rvmHome;
	} else {
	  ldLibraryPath = rvmHome+":"+ldLibraryPath;
	}
	setenv("LD_LIBRARY_PATH", ldLibraryPath.c_str(), 1);

	std::vector<std::string> args;
	std::istringstream iss(baseArgs);
	std::string str;
	while (std::getline(iss, str)) {
		args.push_back(str);
	}

	auto vmArgs = CommandLineArgs::getVmArgs();
	args.insert(args.end(), vmArgs.cbegin(), vmArgs.cend());

	return args;
}

