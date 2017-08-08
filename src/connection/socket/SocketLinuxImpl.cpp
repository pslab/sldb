/*
 * SocketLinuxImpl.cpp
 *
 *  Created on: 2016. 2. 23.
 *      Author: Seunghun Han
 */
#include "../config.h"
#if HAVE_UNISTD_H == 1

#include "SocketLinuxImpl.h"

#include <iostream>
#include <system_error>
#include <cstdlib>
#include <cassert>
#include <cerrno>
#include <cstring>

#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

SocketLinuxImpl::SocketLinuxImpl() : _socket(-1)
{
}

SocketLinuxImpl::~SocketLinuxImpl()
{
	if (_closed == false) {
		if (close() == false) {
			std::abort();
		}
	}
}

bool SocketLinuxImpl::open(const std::string &address)
{
	bool succeed = false;

	_socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (_socket != -1) {
		std::size_t pos_colon = address.find(':');
		if (pos_colon != std::string::npos) {
			std::string address_without_port = address.substr(0, pos_colon);
			in_addr_t ip;
			if(getIP(address_without_port, &ip) == true) {
				std::uint16_t port = std::stoi(address.substr(pos_colon+1));
				sockaddr_in _addr = {AF_INET, htons(port), ip, {0}};
				if (::connect(_socket, reinterpret_cast<struct sockaddr *>(&_addr), sizeof(_addr)) == 0) {
					_closed = false;
					succeed = true;
				}
			}
		}
		if (succeed == false) {
			close();
		}
	}
	return succeed;
}

bool SocketLinuxImpl::close() noexcept
{
	bool succeed = false;
	int ret = ::close(_socket);
	if (ret == 0) {
		succeed = true;
	} else {
		if (errno == EINTR) {
			TEMP_FAILURE_RETRY(::close(_socket));
			if (errno == EINTR) {
				succeed = true;
			}
		}
	}

	if(succeed) {
		_closed = true;
		_socket = -1;
	}
	return succeed;
}

std::vector<std::uint8_t> SocketLinuxImpl::read(std::size_t count) const
{
	std::vector<std::uint8_t> bytes(count);
	if (count > 0) {
		ssize_t size = ::read(_socket, bytes.data(), count);
		if (size == -1) {
			if (errno == EINTR) {
				TEMP_FAILURE_RETRY(::read(_socket, bytes.data(), count));
				if (errno != EINTR) {
					std::cerr << "error : " << errno << ", at : SocketLinuxImpl::read()" << std::endl;
					throw std::system_error(std::error_code(errno, std::generic_category()));
				}
			} else {
				std::cerr << "error : " << errno << ", at : SocketLinuxImpl::read()" << std::endl;
				throw std::system_error(std::error_code(errno, std::generic_category()));
			}
		}
		bytes.resize(size);
	}
	return bytes;
}

std::size_t SocketLinuxImpl::write(const std::vector<std::uint8_t> &bytes) const
{
	std::size_t count = bytes.size();
	const std::uint8_t* data = bytes.data();

	ssize_t length = 0;
	while (count > 0 && length != -1) {
		count -= length;
		data += length;
		length = ::write(_socket, data, count);
	}
	if (length == -1) {
		if (errno == EINTR) {
			TEMP_FAILURE_RETRY(::write(_socket, data, count));
			if (errno != EINTR) {
				std::cerr << "error : " << errno << ", at : SocketLinuxImpl::write()" << std::endl;
				throw std::system_error(std::error_code(errno, std::generic_category()));
			}
		} else {
			std::cerr << "error : " << errno << ", at : SocketLinuxImpl::write()" << std::endl;
			throw std::system_error(std::error_code(errno, std::generic_category()));
		}
	}
	return bytes.size()-count;
}

bool SocketLinuxImpl::getIP(std::string address_without_port, in_addr_t *ip)
{
	assert(ip != nullptr);
	bool succeed = false;

	in_addr_t ret = ::inet_addr(address_without_port.c_str());
	if (ret != INADDR_NONE) {
		*ip = ret;
		succeed = true;
	} else {
		hostent *ent = ::gethostbyname(address_without_port.c_str());
		if (ent != NULL) {
			in_addr **addr_list = reinterpret_cast<in_addr **>(ent->h_addr_list);
			if (addr_list[0] != NULL) {
				*ip = addr_list[0]->s_addr;
				succeed = true;
			}
		}
	}

	return succeed;
}

#endif
