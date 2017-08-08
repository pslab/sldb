/*
 * Vm.h
 *
 *  Created on: 2016. 3. 8.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_VM_VM_H_
#define JIKESDBG_SRC_VM_VM_H_

#include <string>
#include <vector>
#include <memory>

class Vm {
public:
	enum ReferenceType {
		NONE,
		CLASS,
		INTERFACE,
		ARRAY
	};

	enum ClassStatus {
		ERROR,
		VERIFIED,
		PREPARED,
		INITIALIZED
	};

	std::string getVmName(const std::string vmPath);

	Vm();
	virtual ~Vm();
};

#endif /* SRC_VM_VM_H_ */
