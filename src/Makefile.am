AM_CXXFLAGS = -pthread -std=c++11 -Wextra -Wall -pedantic -m32 -DNDEBUG

bin_PROGRAMS=../bin/dbg

___bin_dbg_SOURCES=\
	main.cpp\
	BreakPoints.cpp\
	CommandLineArgs.cpp\
	Debugger.cpp\
	Debuggee.cpp\
	Ptrace.cpp\
	ElfReader.cpp\
	connection/Connector.cpp\
	connection/jdwp/JdwpPacket.cpp\
	connection/jdwp/JdwpCommandPacket.cpp\
	connection/jdwp/JdwpReplyPacket.cpp\
	connection/jdwp/JdwpEventKind.cpp\
	connection/jdwp/JdwpSuspendPolicy.cpp\
	connection/jdwp/JdwpStatus.cpp\
	connection/jdwp/JdwpTag.cpp\
	connection/jdwp/JdwpTypeTag.cpp\
	connection/jdwp/JdwpThreadStatus.cpp\
	connection/jdwp/JdwpSuspendStatus.cpp\
	connection/socket/Socket.cpp\
	connection/socket/SocketLinuxImpl.cpp\
	vm/Vm.cpp\
	vm/JikesRvm/JikesRvm.cpp\
	vm/JikesRvm/Discover.cpp