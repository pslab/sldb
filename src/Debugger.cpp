/*
 * Debugger.cpp
 *
 *  Created on: 2016. 1. 20.
 *      Author: Seunghun Han
 */

#include "Debugger.h"

#include <iostream>
#include <system_error>
#include <cstdlib>
#include <cstring>
#include <cerrno>

#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/reg.h>
#include <sys/user.h>


void Debugger::setBreakpoint(Pid pid, void *addr)
{
	bool r = _breakpoint.enable(pid, addr);
	if (r == false) {
		std::cerr << "error : " << errno << ", at : Debugger::setBreakpoint()" << std::endl;
		throw std::system_error(std::error_code(errno, std::generic_category()));
	}
}

void Debugger::setBreakpoint(Pid pid, std::uintptr_t addr)
{
	setBreakpoint(pid, reinterpret_cast<void*>(addr));
}

void Debugger::removeBreakpoint(Pid pid, void *addr)
{
	bool r = _breakpoint.disable(pid, addr);
	if (r == false) {
		std::cerr << "error : " << errno << ", at : Debugger::removeBreakpoint()" << std::endl;
		throw std::system_error(std::error_code(errno, std::generic_category()));
	}
}

void Debugger::removeBreakpoint(Pid pid, std::uintptr_t addr)
{
	removeBreakpoint(pid, reinterpret_cast<void*>(addr));
}

void Debugger::removeAllBreakpoint(Pid pid)
{
	bool r = _breakpoint.disableAll(pid);
	if (r == false) {
		std::cerr << "error : " << errno << ", at : Debugger::removeAllBreakpoint()" << std::endl;
		throw std::system_error(std::error_code(errno, std::generic_category()));
	}
}

Pid Debugger::wait(int *status, bool hang)
{
	return waitpid(-1, status, hang);
}

Pid Debugger::waitpid(Pid pid, int *status, bool hang)
{
	int s;
	int option = __WALL;
	if (hang == false) {
		option |= WNOHANG;
	}
	Pid p = ::waitpid(pid, &s, option);
	if (p > 0) {
		if (WIFEXITED(s) || WIFSIGNALED(s)) {
			eraseProcessInfo(p);
			p = -p;
		} else if (WIFSTOPPED(s)){
			_pids.insert(p);
			_stopped.insert(p);
		} else {
			p = 0;
		}
	}
	if (status != nullptr) {
		*status = s;
	}
	return p;
}

bool Debugger::cont(Pid pid)
{
	bool notTerminated = true;

	if (_suspended.find(pid) == _suspended.cend()) {
		waitpid(pid, nullptr, false);
		if (_stopped.find(pid) != _stopped.cend()) {
			auto addr = readEip(pid) - 1;
			bool isBreakpoint = false;
			bool success = BreakPoints::isBreakpoint(pid, addr, &isBreakpoint);
			if (success && isBreakpoint) {
				if(_breakpoint.eject(pid, addr)) {
					writeEip(pid, addr);
					singlestep(pid);
					int status;
					pid = waitpid(pid, &status);
					if (WIFSTOPPED(status)) {
						_breakpoint.inject(pid, addr);
					}
				}
			}
			errno = 0;
			bool r = Ptrace::cont(pid);
			if (r == true) {
				_stopped.erase(pid);
			} else {
				if (errno == ESRCH) {
					eraseProcessInfo(pid);
					notTerminated = false;
				}
			}
		} else {
			if (_pids.find(pid) == _pids.cend()) {
				notTerminated = false;
			}
		}
	}

	return notTerminated;
}

void Debugger::singlestep(Pid pid)
{
	bool r = Ptrace::singlestep(pid);
	if (r == false) {
		std::cerr << "error : " << errno << ", at : Debugger::singlestep()" << std::endl;
		throw std::system_error(std::error_code(errno, std::generic_category()));
	}
}

void Debugger::syscall(Pid pid)
{
    bool r = Ptrace::syscall(pid);
	if (r == false) {
		std::cerr << "error : " << errno << ", at : Debugger::syscall()" << std::endl;
		throw std::system_error(std::error_code(errno, std::generic_category()));
	}
}

bool Debugger::isStopped(Pid pid)
{
	bool stopped = false;

	if (_pids.find(pid) != _pids.cend() ) {
		waitpid(pid, nullptr, false);
		if (_stopped.find(pid) != _stopped.end()) {
			stopped = true;
		}
	}
	return stopped;
}

bool Debugger::isSuspended(Pid pid)
{
	bool suspended = false;

	if (_pids.find(pid) != _pids.cend() ) {
		if (_suspended.find(pid) != _suspended.end()) {
			suspended = true;
		}
	}
	return suspended;
}

bool Debugger::stop(Pid pid)
{
	bool stopped = false;

	waitpid(pid, nullptr, false);

	if (_stopped.find(pid) == _stopped.end()) {
		::kill(pid, SIGSTOP);
		int status;
		while (stopped == false) {
			waitpid(pid, &status);
			if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGSTOP) {
				bool r = Ptrace::cont(pid, SIGSTOP);
				if (r == true) {
					waitpid(pid, &status);
					if (WIFSTOPPED(status)) {
						_stopped.insert(pid);
						stopped = true;
					}
				}
			}
		}
	} else {
		if(_pids.find(pid) != _pids.cend()) {
			stopped = true;
		}
	}
	return stopped;
}

bool Debugger::suspend(Pid pid)
{
	bool succeed = stop(pid);
	if (succeed) {
		_suspended.insert(pid);
	}
	return succeed;
}

bool Debugger::resume(Pid pid)
{
	_suspended.erase(pid);
	return cont(pid);
}

Pid Debugger::start(int *status)
{
	Pid pid = wait(status);
	Ptrace::setOptions(pid, Ptrace::TRACECLONE|Ptrace::EXITKILL|Ptrace::TRACESYSGOOD);
	return pid;
}

void Debugger::debug(Pid pid)
{
	if (pid > 0) {
		/* test code start
		struct user_regs_struct regs;
		Ptrace::getReges(pid, &regs);
		std::uintptr_t tid;
		Ptrace::peekText(pid, reinterpret_cast<void*>(regs.esi+0x14), &tid);
		std::cout << "\t pid : " << pid << ", tid : " << tid << ", eip : " << std::hex << regs.eip << std::dec << std::endl;
		 test code end */
	}
}

void Debugger::write(Pid pid, void *p, std::uintptr_t data)
{
	bool r = Ptrace::pokeText(pid, p, data);
	if (r == false) {
		std::cerr << "error : " << errno << ", at : Debugger::write()" << std::endl;
		throw std::system_error(std::error_code(errno, std::generic_category()));
	}
}

void Debugger::write(Pid pid, std::uintptr_t p, std::uintptr_t data)
{
	write(pid, reinterpret_cast<void*>(p), data);
}

std::uintptr_t Debugger::read(Pid pid, void *p)
{
	std::uintptr_t data;
	bool r = Ptrace::peekText(pid, p, &data);
	if (r == false) {
		std::cerr << "error : " << errno << ", at : Debugger::read()" << std::endl;
		throw std::system_error(std::error_code(errno, std::generic_category()));
	}
	return data;
}

std::uintptr_t Debugger::read(Pid pid, std::uintptr_t p)
{
	return read(pid, reinterpret_cast<void*>(p));
}

std::uintptr_t Debugger::readEip(Pid pid)
{
	struct user_regs_struct regs;
	bool r = Ptrace::getReges(pid, &regs);
	if (r == false) {
		std::cerr << "error : " << errno << ", at : Debugger::readEip()"  << std::endl;
		throw std::system_error(std::error_code(errno, std::generic_category()));
	}
#if __x86_64__
	return regs.rip;
#else
	return regs.eip;
#endif
}

std::uintptr_t Debugger::readEbp(Pid pid)
{
	struct user_regs_struct regs;
	bool r = Ptrace::getReges(pid, &regs);
	if (r == false) {
		std::cerr << "error : " << errno << ", at : Debugger::readEsp()"  << std::endl;
		throw std::system_error(std::error_code(errno, std::generic_category()));
	}
#if __x86_64__
	return regs.rbp;
#else
	return regs.ebp;
#endif
}

std::uintptr_t Debugger::readEsp(Pid pid)
{
	struct user_regs_struct regs;
	bool r = Ptrace::getReges(pid, &regs);
	if (r == false) {
		std::cerr << "error : " << errno << ", at : Debugger::readEsp()"  << std::endl;
		throw std::system_error(std::error_code(errno, std::generic_category()));
	}
#if __x86_64__
	return regs.rsp;
#else
	return regs.esp;
#endif
}

std::uintptr_t Debugger::readEsi(Pid pid)
{
	struct user_regs_struct regs;
	bool r = Ptrace::getReges(pid, &regs);
	if (r == false) {
		std::cerr << "error : " << errno << ", at : Debugger::readEsi()"  << std::endl;
		throw std::system_error(std::error_code(errno, std::generic_category()));
	}
#if __x86_64__
	return regs.rsi;
#else
	return regs.esi;
#endif
}

#if __x86_64__
#define ORIG_EAX ORIG_RAX
#define EBX RBX
typedef unsigned long long Reg;
#else
typedef long int Reg;
#endif

std::uintptr_t Debugger::readSysCallNumber(Pid pid)
{


    std::uintptr_t syscall_num;
	bool r = Ptrace::peekUser(pid, sizeof(Reg) * ORIG_EAX, &syscall_num);
	if (r == false) {
		std::cerr << "error : " << errno << ", at : Debugger::readSysCallNumber()"  << std::endl;
		throw std::system_error(std::error_code(errno, std::generic_category()));
	}
	return syscall_num;
}

std::uintptr_t Debugger::readSysCallArg1(Pid pid)
{
    std::uintptr_t syscall_arg1;
	bool r = Ptrace::peekUser(pid, sizeof(Reg) * EBX, &syscall_arg1);
	if (r == false) {
		std::cerr << "error : " << errno << ", at : Debugger::readSysCallArg1()"  << std::endl;
		throw std::system_error(std::error_code(errno, std::generic_category()));
	}
	return syscall_arg1;
}

void Debugger::writeEip(Pid pid, std::uintptr_t eip)
{
	struct user_regs_struct regs;
	bool r = Ptrace::getReges(pid, &regs);
	if (r == false) {
		std::cerr << "error : " << errno << ", at : Debugger::writeEip()"  << std::endl;
		throw std::system_error(std::error_code(errno, std::generic_category()));
	}
#if __x86_64__
	regs.rip = eip;
#else
	regs.eip = eip;
#endif
	Ptrace::setReges(pid, regs);
	if (r == false) {
		std::cerr << "error : " << errno << ", at : Debugger::writeEip()"  << std::endl;
		throw std::system_error(std::error_code(errno, std::generic_category()));
	}
}

void Debugger::writeEsp(Pid pid, std::uintptr_t esp)
{
	struct user_regs_struct regs;
	bool r = Ptrace::getReges(pid, &regs);
	if (r == false) {
		std::cerr << "error : " << errno << ", at : Debugger::writeEsp()"  << std::endl;
		throw std::system_error(std::error_code(errno, std::generic_category()));
	}
#if __x86_64__
	regs.rsp = esp;
#else
	regs.esp = esp;
#endif
	Ptrace::setReges(pid, regs);
	if (r == false) {
		std::cerr << "error : " << errno << ", at : Debugger::writeEsp()"  << std::endl;
		throw std::system_error(std::error_code(errno, std::generic_category()));
	}
}

void Debugger::writeEsi(Pid pid, std::uintptr_t esi)
{
	struct user_regs_struct regs;
	bool r = Ptrace::getReges(pid, &regs);
	if (r == false) {
		std::cerr << "error : " << errno << ", at : Debugger::writeEsi()"  << std::endl;
		throw std::system_error(std::error_code(errno, std::generic_category()));
	}
#if __x86_64__
	regs.rsi = esi;
#else
	regs.esi = esi;
#endif
	Ptrace::setReges(pid, regs);
	if (r == false) {
		std::cerr << "error : " << errno << ", at : Debugger::writeEsi()"  << std::endl;
		throw std::system_error(std::error_code(errno, std::generic_category()));
	}
}

void Debugger::writeSysCallArg1(Pid pid, std::uintptr_t data)
{
	bool r = Ptrace::pokeUser(pid, sizeof(Reg) * EBX, data);
	if (r == false) {
		std::cerr << "error : " << errno << ", at : Debugger::readSysCallArg1()"  << std::endl;
		throw std::system_error(std::error_code(errno, std::generic_category()));
	}
}

bool Debugger::isValidPointer(Pid pid, std::uintptr_t pointer)
{
	bool validPointer;
	try {
		std::uintptr_t data;
		bool r = Ptrace::peekText(pid, reinterpret_cast<void*>(pointer), &data);
		validPointer = r;
	} catch (std::system_error &e) {
		validPointer = false;
	}

	return validPointer;
}

void Debugger::printError()
{
	std::cerr << std::strerror(errno);
}


void Debugger::eraseProcessInfo(Pid pid)
{
	_stopped.erase(pid);
	_pids.erase(pid);
	_suspended.erase(pid);
}
