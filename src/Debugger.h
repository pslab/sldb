/*
 * Debugger.h
 *
 *  Created on: 2016. 1. 20.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_DEBUGGER_H_
#define JIKESDBG_SRC_DEBUGGER_H_

#include <set>

#include "BreakPoints.h"

#include "Ptrace.h"

typedef Ptrace::Pid Pid;

class Debugger {
public:
	Debugger() {}
	virtual ~Debugger() {}

	std::set<Pid> getPids() const { return _pids; }
	std::set<Pid> getStopped() const { return _stopped; }
	bool isDebuggeeDead() const { return _pids.size() == 0; }

	void setBreakpoint(Pid pid, void *addr);
	void setBreakpoint(Pid pid, std::uintptr_t addr);
	void removeBreakpoint(Pid pid, void *addr);
	void removeBreakpoint(Pid pid, std::uintptr_t addr);
	void removeAllBreakpoint(Pid pid);
	Pid wait(int *status = nullptr, bool hang = true);
	Pid waitpid(Pid pid, int *status = nullptr, bool hang = true);
	bool cont(Pid pid);
	void singlestep(Pid pid);
	void syscall(Pid pid);
	bool isStopped(Pid pid);
	bool isSuspended(Pid pid);
	bool stop(Pid pid);
	bool suspend(Pid pid);
	bool resume(Pid pid);

	Pid start(int *status = nullptr);
	void debug(Pid pid);

	void write(Pid pid, void *p, std::uintptr_t data);
	void write(Pid pid, std::uintptr_t p, std::uintptr_t data);
	std::uintptr_t read(Pid pid, void *p);
	std::uintptr_t read(Pid pid, std::uintptr_t p);
	std::uintptr_t readEip(Pid pid);
	std::uintptr_t readEbp(Pid pid);
	std::uintptr_t readEsp(Pid pid);
	std::uintptr_t readEsi(Pid pid);
	std::uintptr_t readSysCallNumber(Pid pid);
	std::uintptr_t readSysCallArg1(Pid pid);
	void writeEip(Pid pid, std::uintptr_t eip);
	void writeEsp(Pid pid, std::uintptr_t esp);
	void writeEsi(Pid pid, std::uintptr_t esi);
	void writeSysCallArg1(Pid pid, std::uintptr_t data);

	bool isValidPointer(Pid pid, std::uintptr_t pointer);

private:
	static void printError();

	void eraseProcessInfo(Pid pid);

	std::set<Pid> _pids;
	std::set<Pid> _stopped;
	std::set<Pid> _suspended;
	BreakPoints _breakpoint;
};

#endif /* JIKESDBG_SRC_DEBUGGER_H_ */
