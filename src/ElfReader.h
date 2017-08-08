/*
 * ElfReader.h
 *
 *  Created on: 2016. 5. 19.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_ELFREADER_H_
#define JIKESDBG_SRC_ELFREADER_H_

#include <cstdint>
#include <string>
#include <unordered_map>

class ElfReader {
public:
	ElfReader(const std::string &filename) : _filename(filename) {
	}
	virtual ~ElfReader() {
	}

	std::uintptr_t getAddress(const std::string &symbolname);
	std::uintptr_t getGotAddress(const std::string &symbolname);

private:
	virtual void read(void);
	void setSymbols(std::uintptr_t sym, std::size_t size, std::size_t entsize, char* str);
	void setDynSymbols(std::uintptr_t rel, std::size_t size, std::size_t entsize, std::uintptr_t dynsym, std::size_t dynEntsize, char* str);


	const std::string _filename;
	bool _read = false;
	std::unordered_map<std::string, std::uintptr_t> _symbols;
	std::unordered_map<std::string, std::uintptr_t> _dynSymbols;
};

#endif /* JIKESDBG_SRC_ELFREADER_H_ */
