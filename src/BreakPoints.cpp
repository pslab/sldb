/*
 * BreakPoints.cpp
 *
 *  Created on: 2016. 1. 20.
 *      Author: Seunghun Han
 */

#include "BreakPoints.h"

#include <iostream>
#include <vector>
#include <cassert>
#include <system_error>

bool BreakPoints::isBreakpoint(Ptrace::Pid pid, std::uintptr_t addr, bool *result)
{
	return isBreakpoint(pid, reinterpret_cast<void*>(addr), result);
}

bool BreakPoints::isBreakpoint(Ptrace::Pid pid, void *addr, bool *result)
{
	assert(result != nullptr);
	bool succeed = false;

	std::uintptr_t data;
	if(Ptrace::peekText(pid, addr, &data)) {
		if ((data & 0xFF) == INT3) {
			*result = true;
		} else {
			*result = false;
		}
		succeed = true;
	}

	return succeed;
}

bool BreakPoints::enable(Ptrace::Pid pid, std::uintptr_t addr)
{
	return enable(pid, reinterpret_cast<void*>(addr));
}

bool BreakPoints::enable(Ptrace::Pid pid, void *addr)
{
	bool succeed = false;

	if (_memory.find(addr) == _memory.end()) {
		succeed = inject(pid, addr);
	}

	return succeed;
}

bool BreakPoints::disable(Ptrace::Pid pid, std::uintptr_t addr)
{
	return disable(pid, reinterpret_cast<void*>(addr));
}

bool BreakPoints::disable(Ptrace::Pid pid, void *addr)
{
	return eject(pid, addr);
}

bool BreakPoints::disableAll(Ptrace::Pid pid)
{
	bool succeed = false;
	std::vector<void*> removed;
	for (auto &data : _memory) {
		bool r = disable(pid, data.first);
		if (r == false) {
			break;
		} else {
			removed.push_back(data.first);
		}
	}
	if (_memory.empty()) {
		succeed = true;
	} else {
		// failed... try to recover
		for (auto &addr : removed) {
			bool r = enable(pid, addr);
			if (r == false) {
				std::cerr << "error : " << errno << ", at : BreakPoints::disableAll()" << std::endl;
				throw std::system_error(std::error_code(errno, std::generic_category()));
			}
		}
	}
	return succeed;
}

bool BreakPoints::inject(Ptrace::Pid pid, std::uintptr_t addr)
{
	return inject(pid, reinterpret_cast<void*>(addr));
}

bool BreakPoints::inject(Ptrace::Pid pid, void *addr)
{
	bool succeed = false;

	std::uintptr_t data;
	if(Ptrace::peekText(pid, addr, &data)) {
		std::uintptr_t dataWithTrap = (data & ~(std::uintptr_t)0xFF) | INT3;
		if(Ptrace::pokeText(pid, addr, dataWithTrap)) {
		    _memory[addr] = static_cast<std::uint8_t>(data);
			succeed = true;
		}
	}
	return succeed;
}

bool BreakPoints::eject(Ptrace::Pid pid, std::uintptr_t addr)
{
	return eject(pid, reinterpret_cast<void*>(addr));
}

bool BreakPoints::eject(Ptrace::Pid pid, void *addr)
{
	bool succeed = false;

	auto iter = _memory.find(addr);
	if (iter != _memory.end()) {
		std::uintptr_t data;
		if(Ptrace::peekText(pid, addr, &data)) {
			std::uint8_t original = iter->second;
			std::uintptr_t dataWithoutTrap = (data & ~(std::uintptr_t)0xFF) | original;
			if(Ptrace::pokeText(pid, addr, dataWithoutTrap)) {
			    _memory.erase(addr);
				succeed = true;
			}
		}
	}
	return succeed;
}
