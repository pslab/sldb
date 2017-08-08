/*
 * SocketLinuxImpl.h
 *
 *  Created on: 2016. 2. 23.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION__SOCKET_SOCKETLINUXIMPL_H_
#define JIKESDBG_SRC_CONNECTION__SOCKET_SOCKETLINUXIMPL_H_

#include "Socket.h"

#include <memory>
#include <cstdint>

#include <netinet/in.h>

class SocketLinuxImpl final : public Socket{
public:
	SocketLinuxImpl();
	SocketLinuxImpl(const SocketLinuxImpl&) = delete;
	virtual ~SocketLinuxImpl();

	virtual bool open(const std::string &address) override;
	virtual bool close() noexcept override;
	virtual std::vector<std::uint8_t> read(const std::size_t count) const override;
	virtual std::size_t write(const std::vector<std::uint8_t> &bytes) const override;

	SocketLinuxImpl& operator =(const SocketLinuxImpl&) = delete;

private:
	int _socket;

	bool getIP(std::string address_without_port, in_addr_t *ip);
};

#endif /* JIKESDBG_SRC_CONNECTION__SOCKET_SOCKETLINUXIMPL_H_ */
