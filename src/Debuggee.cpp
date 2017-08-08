/*
 * Debuggee.cpp
 *
 *  Created on: 2016. 1. 20.
 *      Author: Seunghun Han
 */

#include "Debuggee.h"

#include <iostream>
#include <system_error>
#include <cstdlib>
#include <cstring>

#include <unistd.h>

#include "Ptrace.h"
#include "CommandLineArgs.h"

Ptrace::Pid Debuggee::run() const
{
	Ptrace::Pid pid = fork();
	switch (pid)
	{
	case -1:
		std::cerr << "error : " << errno << ", at : Debuggee::run()" << std::endl;
		throw std::system_error(std::error_code(errno, std::generic_category()));
		break;
	case 0: {
		char **argv = new char*[_args.size()+2];
		argv[0] = new char[_vm.length()+1];
		std::strcpy(argv[0],_vm.c_str());
		for(std::size_t i=0; i<_args.size(); i++) {
			argv[i+1] = new char[_args[i].length()+1];
			std::strcpy(argv[i+1], _args[i].c_str());
		}
		argv[_args.size()+1] = nullptr;

		auto vmPath = CommandLineArgs::getVmPath();
		auto home = vmPath.substr(0, vmPath.find_last_of('/'));
		if (chdir(home.c_str()) == 0) {

			bool result = Ptrace::traceMe();
			if (result == true) {
				::execvp(argv[0], argv);
			}
		}

		std::cerr << "error : " << errno << ", at : Debuggee::run()" << std::endl;
		throw std::system_error(std::error_code(errno, std::generic_category()));
		::exit(0);
		break;
	}
	default:
		break;
	}
	return pid;
}
