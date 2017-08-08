/*
 * Vm.cpp
 *
 *  Created on: 2016. 3. 15.
 *      Author: Seunghun Han
 */

#include "Vm.h"

#include "JikesRvm/JikesRvm.h"

std::string Vm::getVmName(const std::string vmPath)
{
	std::string vmName;

	const std::string jikesRvm = "rvm";
	if (vmPath.substr(vmPath.length()-jikesRvm.length()) == jikesRvm) {
		vmName = JikesRvm::NAME;
	}
	return vmName;
}
