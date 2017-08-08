/*
 * Socket.h
 *
 *  Created on: 2016. 2. 23.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION__SOCKET_SOCKET_H_
#define JIKESDBG_SRC_CONNECTION__SOCKET_SOCKET_H_

#include <string>
#include <vector>
#include <memory>

class Socket {
public:
	Socket() {}
	Socket(const Socket&) = delete;
	virtual ~Socket() {}

	virtual bool open(const std::string &address) = 0;
	virtual bool close() noexcept = 0;
	virtual std::vector<unsigned char> read(const std::size_t count) const = 0;
	virtual std::size_t write(const std::vector<unsigned char> &bytes) const = 0;

	Socket& operator =(const Socket&) = delete;

	static std::unique_ptr<Socket> create();

protected:
	bool _closed = true;
};

#endif /* JIKESDBG_SRC_CONNECTION__SOCKET_SOCKET_H_ */
