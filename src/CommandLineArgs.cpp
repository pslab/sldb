/*
 * CommandLineArgs.cpp
 *
 *  Created on: 2016. 2. 22.
 *      Author: Seunghun Han
 */

#include "CommandLineArgs.h"

#include <iostream>
#include <sstream>

std::unordered_map<std::string, std::string> CommandLineArgs::_agentArgs;
std::vector<std::string> CommandLineArgs::_classPath;
std::string CommandLineArgs::_vmPath;
std::vector<std::string> CommandLineArgs::_vmArgs;

void CommandLineArgs::process(int argc, char *argv[])
{
	_agentArgs.clear();
	_classPath.clear();
	_vmArgs.clear();

	const std::string agentlib = "-agentlib:";
	const std::string agentpath = "-agentpath:";
	const std::string classpath = "-classpath";
	const std::string vmpath = "-vmpath";
	const std::string vmargs = "-vmargs";

	for (int i=0; i < argc; ++i) {
		std::string arg(argv[i]);

		if (arg.compare(0, agentlib.length(), agentlib) == 0 || arg.compare(0, agentpath.length(), agentpath) == 0) {
			std::size_t end = arg.find_first_of(":");
			do {
				std::size_t begin = end + 1;
				end = arg.find_first_of("=", begin);
				std::string key = arg.substr(begin, end-begin);
				if (end == std::string::npos) {
					break;
				}
				begin = end + 1;
				end = arg.find_first_of(",", begin);
				std::string value = arg.substr(begin, end-begin);
				_agentArgs[key]=value;
			} while (end != std::string::npos);
		}

		if (arg == classpath) {
			std::string arg(argv[++i]);
			std::size_t begin = 0;
			std::size_t end = -1;
			do {
				begin = end+1;
				end = arg.find_first_of(':', begin);
				std::string path = arg.substr(begin, end-begin);
				_classPath.push_back(path);
			} while (end != std::string::npos);
		}

		if (arg == vmpath) {
			std::string arg(argv[++i]);
			_vmPath = arg;
		}

		if (arg == vmargs) {
			std::string arg(argv[++i]);
			std::istringstream iss(arg);
			std::string str;
			while (std::getline(iss, str, ' ')) {
				_vmArgs.push_back(str);
			}
		}
	}
}

std::string CommandLineArgs::getAddress()
{
	return _agentArgs.at("address");
}

std::string CommandLineArgs::getVmPath()
{
	return _vmPath;
}

std::vector<std::string> CommandLineArgs::getVmArgs()
{
	return _vmArgs;
}

void CommandLineArgs::print(std::ostream &out)
{
	printAgent(out);
	out << std::endl;
	printClassPath(out);
	out << std::endl;
	printVmPath(out);
	out << std::endl;
	printVmArgs(out);
	out << std::endl;
}


void CommandLineArgs::printAgent(std::ostream &out)
{
	out << "agent : ";
	auto iter = _agentArgs.cbegin();
	while (iter != _agentArgs.cend()) {
		out << iter->first << "=" << iter->second;
		++iter;
		if(iter != _agentArgs.cend()) {
			out << ",";
		}
	}
}

void CommandLineArgs::printClassPath(std::ostream &out)
{
	out << "classpath : ";
	auto iter = _classPath.cbegin();
	while (iter != _classPath.cend()) {
		out << *iter;
		++iter;
		if(iter != _classPath.cend()) {
			out << ":";
		}
	}
}

void CommandLineArgs::printVmPath(std::ostream &out)
{
	out << "vmpath : ";
	out << _vmPath;
}

void CommandLineArgs::printVmArgs(std::ostream &out)
{
	out << "vmargs :";
	for (auto &arg : _vmArgs) {
		out << " ";
		out << arg;
	}
}
