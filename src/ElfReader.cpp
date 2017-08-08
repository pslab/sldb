/*
 * ElfReader.cpp
 *
 *  Created on: 2016. 5. 19.
 *      Author: Seunghun Han
 */
#include "ElfReader.h"

#include <iostream>

#include <elf.h>
#include <fstream>
#include <stdexcept>

std::uintptr_t ElfReader::getAddress(const std::string &symbolname)
{
	std::uintptr_t ret = 0;

	if (_read == false) {
		read();
	}

	if(_read == true) {
		try {
			ret = _symbols.at(symbolname);
		} catch (std::out_of_range &e) {

		}
	}

	return ret;
}

std::uintptr_t ElfReader::getGotAddress(const std::string &symbolname)
{
	std::uintptr_t ret = 0;

	if (_read == false) {
		read();
	}

	if(_read == true) {
		try {
			ret = _dynSymbols.at(symbolname);
		} catch (std::out_of_range &e) {

		}
	}

	return ret;
}

void ElfReader::read(void)
{
	_read = false;
	_symbols.clear();

	std::ifstream ifs(_filename, std::ios_base::in | std::ios_base::binary);

	char* buffer = nullptr;
	if (!ifs.fail()) {
		std::filebuf * pbuf = ifs.rdbuf();
		std::size_t size = pbuf->pubseekoff (0,ifs.end,ifs.in);
		pbuf->pubseekpos (0,ifs.in);
		buffer=new char[size];
		pbuf->sgetn (buffer,size);
		ifs.close();
	}
	if (buffer != nullptr) {
		Elf32_Ehdr *ehdr = reinterpret_cast<Elf32_Ehdr*>(buffer);
		if (ehdr->e_shstrndx != 0) {
			Elf32_Shdr *shdr_shstr = reinterpret_cast<Elf32_Shdr*>(buffer + ehdr->e_shoff + ehdr->e_shentsize*ehdr->e_shstrndx);
			char *shstrtab = buffer + shdr_shstr->sh_offset;
			int num = 0;
			char *strtab = nullptr;
			while (num < ehdr->e_shnum) {
				Elf32_Shdr *shdr = reinterpret_cast<Elf32_Shdr*>(buffer + ehdr->e_shoff + ehdr->e_shentsize*num++);
				if (shdr->sh_type == SHT_STRTAB && std::string(&shstrtab[shdr->sh_name]) == ".strtab") {
					strtab = buffer + shdr->sh_offset;
					break;
				}
			}
			if (strtab != nullptr) {
				int num = 0;
				while (num < ehdr->e_shnum) {
					Elf32_Shdr *shdr = reinterpret_cast<Elf32_Shdr*>(buffer + ehdr->e_shoff + ehdr->e_shentsize*num++);
					if (shdr->sh_type == SHT_SYMTAB) {
						Elf32_Sym *sym = reinterpret_cast<Elf32_Sym*>(buffer + shdr->sh_offset);
						std::size_t size = shdr->sh_size;
						std::size_t entsize = shdr->sh_entsize;
						setSymbols(reinterpret_cast<std::uintptr_t>(sym), size, entsize, strtab);
					}
				}

			}

			num = 0;
			char *dynstr = nullptr;
			while (num < ehdr->e_shnum) {
				Elf32_Shdr *shdr = reinterpret_cast<Elf32_Shdr*>(buffer + ehdr->e_shoff + ehdr->e_shentsize*num++);
				if (shdr->sh_type == SHT_STRTAB && std::string(&shstrtab[shdr->sh_name]) == ".dynstr") {
					dynstr = buffer + shdr->sh_offset;
					break;
				}
			}

			if (dynstr != nullptr) {
				int num = 0;
				char *dynsym = nullptr;
				std::size_t dynEntsize = 0;
				while (num < ehdr->e_shnum) {
					Elf32_Shdr *shdr = reinterpret_cast<Elf32_Shdr*>(buffer + ehdr->e_shoff + ehdr->e_shentsize*num++);
					if (shdr->sh_type == SHT_DYNSYM && std::string(&shstrtab[shdr->sh_name]) == ".dynsym") {
						dynsym = buffer + shdr->sh_offset;
						dynEntsize = shdr->sh_entsize;
						break;
					}
				}
				if (dynsym != nullptr) {
					int num = 0;
					while (num < ehdr->e_shnum) {
						Elf32_Shdr *shdr = reinterpret_cast<Elf32_Shdr*>(buffer + ehdr->e_shoff + ehdr->e_shentsize*num++);
						if (shdr->sh_type == SHT_REL) {
							Elf32_Rel *rel = reinterpret_cast<Elf32_Rel*>(buffer + shdr->sh_offset);
							std::size_t size = shdr->sh_size;
							std::size_t entsize = shdr->sh_entsize;
							setDynSymbols(reinterpret_cast<std::uintptr_t>(rel), size, entsize,
									reinterpret_cast<std::uintptr_t>(dynsym), dynEntsize, dynstr);
						}
					}
				}
			}
		}
		delete[] buffer;
		buffer = nullptr;
	}

	if(!_symbols.empty()) {
		_read = true;
	}
}

void ElfReader::setSymbols(std::uintptr_t sym, std::size_t size, std::size_t entsize, char* str)
{

	int i = 0;
	Elf32_Sym *s = reinterpret_cast<Elf32_Sym*>(sym);
	while (reinterpret_cast<std::uintptr_t>(s) < sym + size) {
		std::string name(&str[s->st_name]);
		if (!name.empty()) {
			_symbols[name] = s->st_value;
		}
		s = reinterpret_cast<Elf32_Sym*>(sym+entsize*++i);
	}
}

void ElfReader::setDynSymbols(std::uintptr_t rel, std::size_t size, std::size_t entsize, std::uintptr_t dynsym, std::size_t dynEntsize, char* str)
{
	int i = 0;
	Elf32_Rel *r = reinterpret_cast<Elf32_Rel*>(rel);
	while (reinterpret_cast<std::uintptr_t>(r) < rel + size) {
		auto index = ELF32_R_SYM(r->r_info);
		Elf32_Sym *d = reinterpret_cast<Elf32_Sym*>(dynsym+dynEntsize*index);
		std::string name(&str[d->st_name]);
		if (!name.empty()) {
			_dynSymbols[name] = r->r_offset;
		}
		r = reinterpret_cast<Elf32_Rel*>(rel+entsize*++i);
	}
}
