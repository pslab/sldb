/*
 * Ptrace.cpp
 *
 *  Created on: 2016. 1. 20.
 *      Author: Seunghun Han
 */

#include "Ptrace.h"

#include <cerrno>
#include <cassert>

bool Ptrace::traceMe()
{
	return ::ptrace(PTRACE_TRACEME, nullptr, nullptr, nullptr) != -1;
}

bool Ptrace::peekText(Pid pid, void *addr, std::uintptr_t *data)
{
	assert(data != nullptr);

	errno = 0;
	*data = ::ptrace(PTRACE_PEEKTEXT, pid, addr, nullptr);
	return errno == 0;
}

bool Ptrace::peekUser(Pid pid, std::uintptr_t offset, std::uintptr_t *data)
{
    assert(data != nullptr);

    errno = 0;
	*data = ::ptrace(PTRACE_PEEKUSER, pid, offset, nullptr);
	return errno == 0;
}

bool Ptrace::pokeText(Pid pid, void *addr, std::uintptr_t data)
{
	return ::ptrace(PTRACE_POKETEXT, pid, addr, data) != -1;
}

bool Ptrace::pokeUser(Pid pid, std::uintptr_t offset, std::uintptr_t data)
{
	return ::ptrace(PTRACE_POKEUSER, pid, offset, data) != -1;
}

bool Ptrace::getReges(Pid pid, struct user_regs_struct *regs)
{
	return ::ptrace(PTRACE_GETREGS, pid, nullptr, regs) != -1;
}

bool Ptrace::setReges(Pid pid, struct user_regs_struct regs)
{
	return ::ptrace(PTRACE_SETREGS, pid, nullptr, &regs) != -1;
}


bool Ptrace::setOptions(Pid pid, unsigned long int option)
{
	return ptrace(PTRACE_SETOPTIONS, pid, nullptr, option) != -1;
}

bool Ptrace::cont(Pid pid, std::uintptr_t sig)
{
	return ptrace(PTRACE_CONT, pid, nullptr, sig) != -1;
}

bool Ptrace::cont(Pid pid)
{
	return ptrace(PTRACE_CONT, pid, nullptr, nullptr) != -1;
}

bool Ptrace::singlestep(Pid pid)
{
	return ptrace(PTRACE_SINGLESTEP, pid, nullptr, nullptr) != -1;
}

bool Ptrace::syscall(Pid pid)
{
	return ptrace(PTRACE_SYSCALL, pid, nullptr, nullptr) != -1;
}

bool Ptrace::attach(Pid pid)
{
	return ::ptrace(PTRACE_ATTACH, pid, nullptr, nullptr) != -1;
}

bool Ptrace::detach(Pid pid)
{
	return ::ptrace(PTRACE_DETACH, pid, nullptr, nullptr) != -1;
}
