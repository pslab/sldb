/*
 * BreakPoints.h
 *
 *  Created on: 2016. 1. 20.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_BREAKPOINTS_H_
#define JIKESDBG_SRC_BREAKPOINTS_H_

#include <unordered_map>
#include <cstdint>

#include <Ptrace.h>

class BreakPoints {
public:
	BreakPoints() {}
	virtual ~BreakPoints() {}

	static bool isBreakpoint(Ptrace::Pid pid, std::uintptr_t addr, bool *result);
	static bool isBreakpoint(Ptrace::Pid pid, void *addr, bool *result);

	bool enable(Ptrace::Pid pid, std::uintptr_t addr);
	bool enable(Ptrace::Pid pid, void *addr);

	bool disable(Ptrace::Pid pid, std::uintptr_t addr);
	bool disable(Ptrace::Pid pid, void *addr);

	bool disableAll(Ptrace::Pid pid);

	bool inject(Ptrace::Pid pid, std::uintptr_t addr);
	bool inject(Ptrace::Pid pid, void *addr);

	bool eject(Ptrace::Pid pid, std::uintptr_t addr);
	bool eject(Ptrace::Pid pid, void *addr);

private:
	static const std::uint8_t INT3 = 0xCCu;

	std::unordered_map<void*, std::uint8_t> _memory;
};

#endif /* JIKESDBG_SRC_BREAKPOINTS_H_ */
