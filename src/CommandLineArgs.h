/*
 * CommandLineArgs.h
 *
 *  Created on: 2016. 2. 22.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_COMMANDLINEARGS_H_
#define JIKESDBG_SRC_COMMANDLINEARGS_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <ostream>

class CommandLineArgs final {
public:
	static void process(int argc, char *argv[]);
	static std::string getAddress();
	static std::string getVmPath();
	static std::vector<std::string> getVmArgs();

	static void print(std::ostream &out);
	static void printAgent(std::ostream &out);
	static void printClassPath(std::ostream &out);
	static void printVmPath(std::ostream &out);
	static void printVmArgs(std::ostream &out);

private:
	static std::unordered_map<std::string, std::string> _agentArgs;
	static std::vector<std::string> _classPath;
	static std::string _vmPath;
	static std::vector<std::string> _vmArgs;

	CommandLineArgs();
	~CommandLineArgs();
};

#endif /* JIKESDBG_SRC_COMMANDLINEARGS_H_ */
