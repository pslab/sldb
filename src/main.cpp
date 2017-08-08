/*
 ============================================================================
 Name        : main.cpp
 Author      : Seunghun Han
 Version     :
 Copyright   : Eclipse Public License - v 1.0
 Description : Jikes RVM Debugger,
 ============================================================================
 */

#include <iostream>
#include <stdexcept>

#include "CommandLineArgs.h"
#include "DebugProcess.h"

#include "vm/JikesRvm/JikesRvm.h"



int main(int argc, char *argv[])
{

	try {
		CommandLineArgs::process(argc, argv);
		CommandLineArgs::print(std::cout);
	} catch (std::exception &e) {
		std::cerr << "please execute from eclipse plugin." << std::endl;
	}

	DebugProcess<JikesRvm>::getInstance()->start();

	return 0;
}
