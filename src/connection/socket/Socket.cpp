/*
 * SocketLinuxImpl.cpp
 *
 *  Created on: 2016. 3. 7.
 *      Author: Seunghun Han
 */
#include "Socket.h"

#include "../config.h"

#if HAVE_UNISTD_H == 1
#include "SocketLinuxImpl.h"
#endif

std::unique_ptr<Socket> Socket::create()
{
#if HAVE_UNISTD_H == 1
	return std::unique_ptr<Socket>(new SocketLinuxImpl);
#endif
}
