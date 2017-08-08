/*
 * Debuggee.h
 *
 *  Created on: 2016. 1. 20.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_DEBUGGEE_H_
#define JIKESDBG_SRC_DEBUGGEE_H_

#include <string>
#include <vector>

#include "Ptrace.h"

class Debuggee {
public:
	Debuggee(const std::string &vm, const std::vector<std::string> &args) : _vm(vm), _args(args) {}
	virtual ~Debuggee() {}

	Ptrace::Pid run() const;

private:
	std::string _vm;
	const std::vector<std::string> _args;
};

#endif /* JIKESDBG_SRC_DEBUGGEE_H_ */
