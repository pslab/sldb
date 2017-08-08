/*
 * Connector.cpp
 *
 *  Created on: 2016. 2. 25.
 *      Author: Seunghun Han
 */

#include "Connector.h"

#include <cerrno>

#include "jdwp/JdwpPacket.h"

bool Connector::connect(std::string address)
{
	bool succeed = false;

	_socket = Socket::create();

	if (_socket->open(address) == true) {
		auto buf = _socket->read(16);
		std::string message(buf.begin(), buf.end());
		if (message == "JDWP-Handshake") {
			auto count = _socket->write(buf);
			if (count == buf.size()) {
				succeed = true;
			}
		}
	}

	return succeed;
}

void Connector::sendPacket(std::shared_ptr<JdwpPacket> packet) const
{
	if (print == true) {
		std::cout << "send : " << *packet << std::endl;
	}

	write(packet->pack());
}

void Connector::disconnect() noexcept
{
	_socket.reset();
}

std::vector<std::uint8_t> Connector::read(std::size_t count) const
{
	std::vector<std::uint8_t> bytes;
	std::size_t size = 0;
	while (size != count){
		auto buf = _socket->read(count-size);
		bytes.insert(bytes.end(), buf.cbegin(), buf.cend());
		size += buf.size();
	}

	return bytes;
}

void Connector::write(const std::vector<std::uint8_t> &bytes) const
{
	_socket->write(bytes);
}
