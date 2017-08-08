/*
 * Ptrace.h
 *
 *  Created on: 2016. 1. 20.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_PTRACE_H_
#define JIKESDBG_SRC_PTRACE_H_

#include <cstdint>

#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>

class Ptrace {
public:
	typedef pid_t Pid;

	enum Option {
		EXITKILL = PTRACE_O_EXITKILL,
		TRACECLONE = PTRACE_O_TRACECLONE,
		TRACEEXEC = PTRACE_O_TRACEEXEC,
		TRACEEXIT = PTRACE_O_TRACEEXIT,
		TRACEFORK = PTRACE_O_TRACEFORK,
		TRACESYSGOOD = PTRACE_O_TRACESYSGOOD,
		TRACEVFORK = PTRACE_O_TRACEVFORK,
		TRACEVFORKDONE = PTRACE_O_TRACEVFORKDONE,
		TRACESECCOMP = PTRACE_O_TRACESECCOMP
	};
	static bool traceMe();
	static bool peekText(Pid pid, void *addr, std::uintptr_t *data);
	static bool peekUser(Pid pid, std::uintptr_t offset, std::uintptr_t *data);
	static bool pokeText(Pid pid, void *addr, std::uintptr_t data);
	static bool pokeUser(Pid pid, std::uintptr_t offset, std::uintptr_t data);
	static bool getReges(Pid pid, struct user_regs_struct *regs);
	static bool setReges(Pid pid, struct user_regs_struct regs);
	static bool setOptions(Pid pid, unsigned long int option);
	static bool cont(Pid pid, std::uintptr_t sig);
	static bool cont(Pid pid);
	static bool singlestep(Pid pid);
	static bool syscall(Pid pid);
	static bool attach(Pid pid);
	static bool detach(Pid pid);


private:
	Ptrace();
	~Ptrace();
};

#endif /* JIKESDBG_SRC_PTRACE_H_ */
