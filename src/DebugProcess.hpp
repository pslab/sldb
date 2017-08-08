/*
 * DebugProcess.hpp
 *
 *  Created on: 2016. 3. 21.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_DEBUGPROCESS_HPP_
#define JIKESDBG_SRC_DEBUGPROCESS_HPP_

#include "DebugProcess.h"

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <stdexcept>


template <typename Vm>
DebugProcess<Vm> DebugProcess<Vm>::instance;

template <typename Vm>
void DebugProcess<Vm>::start()
{
	if (_exited == true) {
		std::thread thread1 = std::thread(&DebugProcess<Vm>::debug, &instance);
		_exited = false;
		while (_toConnector.empty()) {
			std::this_thread::sleep_for (std::chrono::milliseconds(500));
		}
		std::thread thread2 = std::thread(&DebugProcess<Vm>::connect, &instance);
		thread1.join();
		thread2.join();
	}
}

template <typename Vm>
void DebugProcess<Vm>::debug()
{
	auto firstThreadID = Vm::start(_debugger);
	if (firstThreadID == -1) {
		_toConnector.pushMessage(MessageQueue::VM_EXIT);
	} else if (_debugger.isDebuggeeDead() == false) {
		_toConnector.pushMessage(MessageQueue::VM_START, firstThreadID);

		while (_debugger.isDebuggeeDead() == false && _exited == false) {
			Pid pid = 0;
			pid = _debugger.wait(nullptr, false);

			if (pid > 0) {
				typename Vm::ThreadID tid = -1;
				if ((tid = Vm::threadStart(_debugger, pid)) > 0 ) {
					try {
						static auto startevent = JdwpEventKind::THREAD_START;
						if (_suspendPolicyMap.at(startevent) == JdwpSuspendPolicy::EVENT_THREAD) {
							_debugger.suspend(pid);
						}
					} catch( std::out_of_range &e) {
					}
					_toConnector.pushMessage(MessageQueue::THREAD_START, tid);
				}else if ((tid = Vm::threadDeath(_debugger, pid)) > 0 ) {
					_toConnector.pushMessage(MessageQueue::THREAD_DEATH, tid);
				} else {
					_debugger.debug(pid);
				}

				bool notTerminated = _debugger.cont(pid);
				if (notTerminated == false) {
					Vm::threadDeath(_debugger, pid);
				}
			}

			auto stopped = _debugger.getStopped();
			for (auto &pid : stopped) {
				bool notTerminated = _debugger.cont(pid);
				if (notTerminated == false) {
					Vm::threadDeath(_debugger, pid);
				}
			}

			debuggerMessageLoop();
		}
		_toConnector.pushMessage(MessageQueue::VM_EXIT);
	}
}

template <typename Vm>
void DebugProcess<Vm>::debuggerMessageLoop()
{
	while(_toDebugger.empty() == false && _exited == false) {
		auto id = _toDebugger.getId();
		switch (id) {
		case MessageQueue::VM_EXIT: {
			_exited = true;
			break;
		}

		case MessageQueue::ALL_THREADS: {
			auto threadIDs = Vm::getAllThreadID();
			_toConnector.pushMessage(id, threadIDs);
			break;
		}

		case MessageQueue::VM_CLASSES: {
			std::string signature;
			_toDebugger.getData(&signature);
			auto s = Vm::getClassStatus(_debugger, signature);

			Int status = 0;
			switch (s) {
			case Vm::ClassStatus::ERROR:
				status = JdwpStatus::ERROR;
				break;
			case Vm::ClassStatus::VERIFIED:
				status = JdwpStatus::VERIFIED;
				break;
			case Vm::ClassStatus::PREPARED:
				status = JdwpStatus::VERIFIED|JdwpStatus::PREPARED;
				break;
			case Vm::ClassStatus::INITIALIZED:
				status = JdwpStatus::VERIFIED|JdwpStatus::PREPARED|JdwpStatus::INITIALIZED;
				break;
			default:
				status = s;
				break;
			}
			auto refId = Vm::getClassReferenceID(_debugger, signature);
			std::vector<uint64_t> data;
			data.push_back(status);
			data.push_back(refId);
			_toConnector.pushMessage(id, data);
			break;
		}

		case MessageQueue::SUPERCLASS: {
			typename Vm::ClassID clazz;
			_toDebugger.getData(&clazz);
			auto superclass = Vm::getSuperclassID(_debugger, clazz);
			_toConnector.pushMessage(id, superclass);
			break;
		}

		case MessageQueue::REFERENCE_TYPE: {
			typename Vm::ObjectID object;
			_toDebugger.getData(&object);
			auto type = Vm::getObjectReferenceType(_debugger, object);
			Byte typeTag = 0;
			switch (type) {
			case Vm::ReferenceType::CLASS:
				typeTag = JdwpTypeTag::CLASS;
				break;
			case Vm::ReferenceType::ARRAY:
				typeTag = JdwpTypeTag::ARRAY;
				break;
			case Vm::ReferenceType::INTERFACE:
				typeTag = JdwpTypeTag::INTERFACE;
				break;
			default:
				typeTag = type;
				break;
			}

			auto refId = Vm::getObjectReferenceID(_debugger, object);
			std::vector<uint64_t> data;
			data.push_back(typeTag);
			data.push_back(refId);
			_toConnector.pushMessage(id, data);
			break;
		}

		case MessageQueue::THREAD_NAME: {
			typename Vm::ThreadID threadID = -1;
			_toDebugger.getData(&threadID);
			try {

				auto threadInformation = Vm::getThreadInformation(threadID);
				auto pid = threadInformation.pid;
				bool stopped = _debugger.stop(pid);
				if (stopped) {
					// TBI
					// Vm::renewThreadInformation(_debugger, pid);
					_debugger.cont(pid);
				}
				threadInformation = Vm::getThreadInformation(threadID);
				auto name = threadInformation.name;
				_toConnector.pushMessage(id, name);
			} catch (std::out_of_range &e) {
				_toConnector.pushMessage(id);
			}
			break;
		}

		case MessageQueue::THREAD_SUSPEND: {
			typename Vm::ThreadID threadID = -1;
			_toDebugger.getData(&threadID);
			try {

				auto threadInformation = Vm::getThreadInformation(threadID);
				auto pid = threadInformation.pid;
				bool stopped = _debugger.suspend(pid);
				if (stopped) {
					// TBI
					// Vm::renewThreadInformation(_debugger, pid);
				}

				_toConnector.pushMessage(id, true);
			} catch (std::out_of_range &e) {
				_toConnector.pushMessage(id);
			}
			break;
		}

		case MessageQueue::THREAD_RESUME: {
			typename Vm::ThreadID threadID = -1;
			_toDebugger.getData(&threadID);
			try {

				auto threadInformation = Vm::getThreadInformation(threadID);
				auto pid = threadInformation.pid;
				bool notTerminated = _debugger.resume(pid);
				if (notTerminated == false) {
					Vm::threadDeath(_debugger, pid);
				}
				_toConnector.pushMessage(id, true);
			} catch (std::out_of_range &e) {
				_toConnector.pushMessage(id);
			}
			break;
		}

		case MessageQueue::THREAD_STATUS: {
			typename Vm::ThreadID threadID = -1;
			_toDebugger.getData(&threadID);
			try {
				auto threadInformation = Vm::getThreadInformation(threadID);
				auto pid = threadInformation.pid;
				bool old = _debugger.isStopped(pid);
				bool stopped = _debugger.stop(pid);
				if (stopped) {
					// TBI
					// Vm::getThreadStatus(_debugger, pid, threadID);
					if (old == false) {
						_debugger.cont(pid);
					}
				}
				Int threadStatus = JdwpThreadStatus::RUNNING;
				if (_debugger.isSuspended(pid)) {
					threadStatus = -threadStatus;
				}
				_toConnector.pushMessage(id, threadStatus);
			} catch (std::out_of_range &e) {
				_toConnector.pushMessage(id);
			}
			break;
		}

		case MessageQueue::THREAD_GROUP: {
			typename Vm::ThreadID threadID = -1;
			_toDebugger.getData(&threadID);
			try {
				auto threadInformation = Vm::getThreadInformation(threadID);
				auto pid = threadInformation.pid;
				typename Vm::ThreadGroupID groupID = -1;
				bool old = _debugger.isStopped(pid);
				bool stopped = _debugger.stop(pid);
				if (stopped) {
					groupID = Vm::setThreadGroup(_debugger, pid, threadID);
					if (old == false) {
						_debugger.cont(pid);
					}
				}
				_toConnector.pushMessage(id, groupID);
			} catch (std::out_of_range &e) {
				_toConnector.pushMessage(id);
			}
			break;
		}

		case MessageQueue::THREAD_FRAMES: {
			typename Vm::ThreadID threadID = -1;
			_toDebugger.getData(&threadID);
			try {
				auto thread = Vm::getThreadInformation(threadID);
				auto pid = thread.pid;
				std::uintptr_t frame = Vm::getFramePointer(_debugger, threadID);
				// For Test
				std::cout << " ==test start==" << std::endl;
				while (_debugger.isValidPointer(pid, frame)) {
					std::cout << "frame pointer : " << std::hex << frame << std::dec << std::endl;
					auto cmid = Vm::getMethodID(_debugger, threadID, frame);
					std::cout << "methodID : " << cmid << std::endl;
					std::uintptr_t old = frame;
					frame = _debugger.read(pid, frame);
					if (cmid != -1) {
						while(old < frame) {
							if (_debugger.isValidPointer(pid, old)) {
								auto test = _debugger.read(pid, old);
								auto str = Vm::getTypename(pid, test);
								if (!str.empty()) {
									std::cout << std::hex << old << std::dec << " : " << str << std::endl;
								}
							}
							old += sizeof(std::uintptr_t);
						}
					}
				}
				std::cout << " ==test end==" << std::endl;
				// TBI
				auto some = 0;
				_toConnector.pushMessage(id, some);
			} catch (std::out_of_range &e) {
				_toConnector.pushMessage(id);
			}
			break;
		}

		case MessageQueue::THREAD_GROUP_NAME: {
			typename Vm::ThreadGroupID threadGroupID = -1;
			_toDebugger.getData(&threadGroupID);
			try {
				auto threadGroupInformation = Vm::getThreadGroupInformation(threadGroupID);
				std::u16string name = threadGroupInformation.name;
				_toConnector.pushMessage(id, name);
			} catch (std::out_of_range &e) {
				_toConnector.pushMessage(id);
			}
			break;
		}

		case MessageQueue::THREAD_GROUP_PARENT: {
			typename Vm::ThreadGroupID threadGroupID = -1;
			_toDebugger.getData(&threadGroupID);
			try {
				auto g = Vm::getThreadGroupInformation(threadGroupID);
				auto parent = g.parent;
				_toConnector.pushMessage(id, parent);
			} catch (std::out_of_range &e) {
				_toConnector.pushMessage(id);
			}
			break;
		}

		case MessageQueue::THREAD_GROUP_CHILDREN: {
			typename Vm::ThreadGroupID threadGroupID = -1;
			_toDebugger.getData(&threadGroupID);
			try {
				auto threadGroupInformation = Vm::getThreadGroupInformation(threadGroupID);
				_toConnector.pushMessage(id, threadGroupInformation);
			} catch (std::out_of_range &e) {
				_toConnector.pushMessage(id);
			}
			break;
		}

		default:
			break;
		}
		_toDebugger.pop();
	}
}

template <typename Vm>
void DebugProcess<Vm>::connect()
{
	bool connected = _connector.connect(CommandLineArgs::getAddress());
	_connector.printOn();
	if (connected == true) {

		connectionMessageLoop(MessageQueue::VM_START, 0);

		while (_exited == false) {
			typename MessageQueue::Id messageToProcess = MessageQueue::NONE;
			Int idOfCommandPacket = 0;

			auto packet = _connector.recvPacket<Vm>();

			if (packet->getFlags() != JdwpPacket::FLAG_REPLY_PACKET) {
				auto command = std::dynamic_pointer_cast<JdwpCommandPacket>(packet);

				switch(command->getCommand()) {
				case JdwpCommandPacket::VM_VERSION: {
					auto reply = std::make_shared<JdwpReplyVmversionPacket<Vm>>();
					reply->setId(command->getId());
					_connector.sendPacket(reply);
					break;
				}

				case JdwpCommandPacket::VM_CLASSES_BY_SIGNATURE: {
					messageToProcess = MessageQueue::VM_CLASSES;
					idOfCommandPacket = command->getId();
					auto ref = std::dynamic_pointer_cast<JdwpCommandVmclassesbysignaturePacket<Vm>>(command);
					auto signature = ref->getSignature();
					_toDebugger.pushMessage(messageToProcess, signature);
					break;
				}

				case JdwpCommandPacket::VM_ALL_THREADS: {
					messageToProcess = MessageQueue::ALL_THREADS;
					idOfCommandPacket = command->getId();
					_toDebugger.pushMessage(messageToProcess);
					break;
				}

				case JdwpCommandPacket::VM_ID_SIZES: {
					auto reply = std::make_shared<JdwpReplyVmidsizesPacket<Vm>>();
					reply->setId(command->getId());
					_connector.sendPacket(reply);
					break;
				}

				case JdwpCommandPacket::VM_CAPABILITIES_NEW: {
					auto reply = std::make_shared<JdwpReplyVmcapabilitiesnewPacket<Vm>>();
					reply->setId(command->getId());
					_connector.sendPacket(reply);
					break;
				}

				case JdwpCommandPacket::CT_SUPERCLASS: {
					messageToProcess = MessageQueue::SUPERCLASS;
					idOfCommandPacket = command->getId();
					auto ct = std::dynamic_pointer_cast<JdwpCommandCtsuperclassPacket<Vm>>(command);
					auto clazz = ct->getClazz();
					_toDebugger.pushMessage(messageToProcess, clazz);
					break;
				}

				case JdwpCommandPacket::RT_INTERFACES: {
					auto reply = std::make_shared<JdwpReplyRtinterfacesPacket<Vm>>();
					reply->setId(command->getId());
					_connector.sendPacket(reply);
					break;
				}

				case JdwpCommandPacket::RT_FIELDS_WITH_GENERIC: {
					auto reply = std::make_shared<JdwpReplyRtfieldswithgenericPacket<Vm>>();
					reply->setId(command->getId());
					_connector.sendPacket(reply);
					break;
				}

				case JdwpCommandPacket::OR_REFERENCE_TYPE: {
					messageToProcess = MessageQueue::REFERENCE_TYPE;
					idOfCommandPacket = command->getId();
					auto ref = std::dynamic_pointer_cast<JdwpCommandOrreferencetypePacket<Vm>>(command);
					auto object = ref->getObject();
					_toDebugger.pushMessage(messageToProcess, object);
					break;
				}

				case JdwpCommandPacket::ER_SET: {
					static Int eventId = 2;
					auto erset = std::dynamic_pointer_cast<JdwpCommandErsetPacket<Vm>>(command);
					switch (erset->getEventKind()) {
					case JdwpEventKind::THREAD_START: {
						auto event = JdwpEventKind::THREAD_START;
						_requestIdMap[event] = eventId++;
						_suspendPolicyMap[event] = erset->getSuspendPolicy();
						break;
					}
					case JdwpEventKind::THREAD_DEATH: {
						auto event = JdwpEventKind::THREAD_DEATH;
						_requestIdMap[event] = eventId++;
						_suspendPolicyMap[event] = erset->getSuspendPolicy();
						break;
					}
					case JdwpEventKind::CLASS_UNLOAD: {
						auto event = JdwpEventKind::CLASS_UNLOAD;
						_requestIdMap[event] = eventId++;
						_suspendPolicyMap[event] = erset->getSuspendPolicy();
						break;
					}
					case JdwpEventKind::CLASS_PREPARE: {
						auto event = JdwpEventKind::CLASS_PREPARE;
						_requestIdMap[event] = eventId++;
						_suspendPolicyMap[event] = erset->getSuspendPolicy();
						break;
					}
					default:
						break;
					}

					auto reply = std::make_shared<JdwpReplyErsetPacket<Vm>>();
					reply->setId(command->getId());
					reply->setRequestID(eventId);
					_connector.sendPacket(reply);

					break;
				}

				case JdwpCommandPacket::TR_NAME: {
					messageToProcess = MessageQueue::THREAD_NAME;
					idOfCommandPacket = command->getId();
					auto tr = std::dynamic_pointer_cast<JdwpCommandTrnamePacket<Vm>>(command);
					auto threadID = tr->getThread();
					_toDebugger.pushMessage(messageToProcess, threadID);
					break;
				}

				case JdwpCommandPacket::TR_SUSPEND: {
					messageToProcess = MessageQueue::THREAD_SUSPEND;
					idOfCommandPacket = command->getId();
					auto tr = std::dynamic_pointer_cast<JdwpCommandTrsuspendPacket<Vm>>(command);
					auto threadID = tr->getThread();
					_toDebugger.pushMessage(messageToProcess, threadID);
					break;
				}

				case JdwpCommandPacket::TR_RESUME: {
					messageToProcess = MessageQueue::THREAD_RESUME;
					idOfCommandPacket = command->getId();
					auto tr = std::dynamic_pointer_cast<JdwpCommandTrresumePacket<Vm>>(command);
					auto threadID = tr->getThread();
					_toDebugger.pushMessage(messageToProcess, threadID);
					break;
				}

				case JdwpCommandPacket::TR_STATUS: {
					messageToProcess = MessageQueue::THREAD_STATUS;
					idOfCommandPacket = command->getId();
					auto tr = std::dynamic_pointer_cast<JdwpCommandTrstatusPacket<Vm>>(command);
					auto threadID = tr->getThread();
					_toDebugger.pushMessage(messageToProcess, threadID);
					break;
				}

				case JdwpCommandPacket::TR_THREAD_GROUP: {
					messageToProcess = MessageQueue::THREAD_GROUP;
					idOfCommandPacket = command->getId();
					auto tr = std::dynamic_pointer_cast<JdwpCommandTrthreadgroupPacket<Vm>>(command);
					auto threadID = tr->getThread();
					_toDebugger.pushMessage(messageToProcess, threadID);
					break;
				}

				case JdwpCommandPacket::TR_FRAMES: {
					messageToProcess = MessageQueue::THREAD_FRAMES;
					idOfCommandPacket = command->getId();
					auto tr = std::dynamic_pointer_cast<JdwpCommandTrframesPacket<Vm>>(command);
					auto threadID = tr->getThread();
					_toDebugger.pushMessage(messageToProcess, threadID);
					break;
				}

				case JdwpCommandPacket::TGR_NAME: {
					messageToProcess = MessageQueue::THREAD_GROUP_NAME;
					idOfCommandPacket = command->getId();
					auto tgr = std::dynamic_pointer_cast<JdwpCommandTgrnamePacket<Vm>>(command);
					auto groupID = tgr->getGroup();
					_toDebugger.pushMessage(messageToProcess, groupID);
					break;
				}

				case JdwpCommandPacket::TGR_PARENT: {
					messageToProcess = MessageQueue::THREAD_GROUP_PARENT;
					idOfCommandPacket = command->getId();
					auto tgr = std::dynamic_pointer_cast<JdwpCommandTgrparentPacket<Vm>>(command);
					auto groupID = tgr->getGroup();
					_toDebugger.pushMessage(messageToProcess, groupID);
					break;
				}

				case JdwpCommandPacket::TGR_CHILDREN: {
					messageToProcess = MessageQueue::THREAD_GROUP_CHILDREN;
					idOfCommandPacket = command->getId();
					auto tgr = std::dynamic_pointer_cast<JdwpCommandTgrchildrenPacket<Vm>>(command);
					auto groupID = tgr->getGroup();
					_toDebugger.pushMessage(messageToProcess, groupID);
					break;
				}

				case JdwpCommandPacket::VM_EXIT: {
					_exited = true;
					break;
				}
				default:
					break;
				}
			}

			connectionMessageLoop(messageToProcess, idOfCommandPacket);
		}
		_connector.disconnect();
		_toDebugger.pushMessage(MessageQueue::VM_EXIT);
	}
}

template <typename Vm>
void DebugProcess<Vm>::connectionMessageLoop(typename MessageQueue::Id messageToProcess, Int idOfCommandPacket)
{
	bool empty = false;
	bool proceed = (messageToProcess == MessageQueue::NONE)? true : false;
	while((empty == false || proceed == false) && _exited == false) {
		empty = _toConnector.empty();
		if (!empty) {
			typename MessageQueue::Id id = _toConnector.getId();
			switch (id) {
			case MessageQueue::VM_START: {
				auto events = std::make_shared<JdwpCommandEcompositePacket<Vm>>();
				auto start = std::make_shared<typename JdwpCommandEcompositePacket<Vm>::VmStart>();
				typename Vm::ThreadID tid = -1;
				_toConnector.getData(&tid);
				start->setThread(tid);
				events->setEvent(start);
				_connector.sendPacket(events);
				break;
			}

			case MessageQueue::VM_EXIT: {
				auto events = std::make_shared<JdwpCommandEcompositePacket<Vm>>();
				auto death = std::make_shared<typename JdwpCommandEcompositePacket<Vm>::VmDeath>();
				events->setEvent(death);
				_connector.sendPacket(events);
				_exited = true;
				break;
			}

			case MessageQueue::ALL_THREADS: {
				auto reply = std::make_shared<JdwpReplyVmallthreadsPacket<Vm>>();
				std::vector<typename Vm::ThreadID> threadIDs;
				_toConnector.getData(&threadIDs);
				reply->setId(idOfCommandPacket);
				reply->setThread(threadIDs);
				_connector.sendPacket(reply);
				break;
			}

			case MessageQueue::VM_CLASSES: {
				auto reply = std::make_shared<JdwpReplyVmclassesbysignaturePacket<Vm>>();
				std::vector<uint64_t> data;
				_toConnector.getData(&data);

				Int status = data[0];
				typename Vm::ReferenceTypeID id = data[1];
				typename JdwpReplyVmclassesbysignaturePacket<Vm>::Class a;
				a.setRefTypeTag(JdwpTypeTag::CLASS);
				a.setTypeID(id);
				a.setStatus(status);

				std::vector<typename JdwpReplyVmclassesbysignaturePacket<Vm>::Class> vec;
				vec.push_back(a);

				reply->setClasses(vec);
				reply->setId(idOfCommandPacket);
				_connector.sendPacket(reply);
				break;
			}

			case MessageQueue::SUPERCLASS: {
				auto reply = std::make_shared<JdwpReplyCtsuperclassPacket<Vm>>();
				typename Vm::ClassID superclass;
				_toConnector.getData(&superclass);
				reply->setSuperclass(superclass);
				reply->setId(idOfCommandPacket);
				_connector.sendPacket(reply);
				break;
			}

			case MessageQueue::REFERENCE_TYPE: {
				auto reply = std::make_shared<JdwpReplyOrreferencetypePacket<Vm>>();
				std::vector<uint64_t> data;
				_toConnector.getData(&data);

				Byte typetag = data[0];
				typename Vm::ReferenceTypeID id = data[1];
				reply->setRefTypeTag(typetag);
				reply->setTypeID(id);
				reply->setId(idOfCommandPacket);
				_connector.sendPacket(reply);
				break;
			}

			case MessageQueue::THREAD_START: {
				try {
					auto event = JdwpEventKind::THREAD_START;
					auto requestId = _requestIdMap.at(event);

					auto events = std::make_shared<JdwpCommandEcompositePacket<Vm>>();
					auto start = std::make_shared<typename JdwpCommandEcompositePacket<Vm>::ThreadStart>();
					typename Vm::ThreadID tid = -1;
					_toConnector.getData(&tid);
					start->setThread(tid);
					start->setRequestID(requestId);
					events->setEvent(start);
					_connector.sendPacket(events);
				} catch (std::out_of_range &e) {
				}
				break;
			}

			case MessageQueue::THREAD_DEATH: {
				try {
					auto event = JdwpEventKind::THREAD_DEATH;
					auto requestId = _requestIdMap.at(event);

					auto events = std::make_shared<JdwpCommandEcompositePacket<Vm>>();
					auto death = std::make_shared<typename JdwpCommandEcompositePacket<Vm>::ThreadDeath>();
					typename Vm::ThreadID tid = -1;
					_toConnector.getData(&tid);
					death->setThread(tid);
					death->setRequestID(requestId);
					events->setEvent(death);
					_connector.sendPacket(events);
				} catch (std::out_of_range &e) {
				}
				break;
			}

			case MessageQueue::THREAD_NAME: {
				auto reply = std::make_shared<JdwpReplyTrnamePacket<Vm>>();
				try {
					std::u16string name;
					_toConnector.getData(&name);
					 std::wstring_convert<std::codecvt_utf8<char16_t>,char16_t> cv;
					reply->setThreadName(cv.to_bytes(name));
				} catch (std::exception &e) {
					reply->setErrorCode(JdwpReplyPacket::INVALID_THREAD);
				}
				reply->setId(idOfCommandPacket);
				_connector.sendPacket(reply);
				break;
			}

			case MessageQueue::THREAD_SUSPEND: {
				auto reply = std::make_shared<JdwpReplyPacket>();
				try {
					bool result;
					_toConnector.getData(&result);
				} catch (std::exception &e) {
					reply->setErrorCode(JdwpReplyPacket::INVALID_THREAD);
				}
				reply->setId(idOfCommandPacket);
				_connector.sendPacket(reply);
				break;
			}

			case MessageQueue::THREAD_RESUME: {
				auto reply = std::make_shared<JdwpReplyPacket>();
				try {
					bool result;
					_toConnector.getData(&result);
				} catch (std::exception &e) {
					reply->setErrorCode(JdwpReplyPacket::INVALID_THREAD);
				}
				reply->setId(idOfCommandPacket);
				_connector.sendPacket(reply);
				break;
			}

			case MessageQueue::THREAD_STATUS: {
				auto reply = std::make_shared<JdwpReplyTrstatusPacket<Vm>>();
				try {
					Int threadStatus;
					_toConnector.getData(&threadStatus);
					if (threadStatus < 0) {
						threadStatus = -threadStatus;
						reply->setSuspendStatus(JdwpSuspendStatus::SUSPEND_STATUS_SUSPENDED);
					}
					reply->setThreadStatus(threadStatus);
				} catch (std::exception &e) {
					reply->setErrorCode(JdwpReplyPacket::INVALID_THREAD);
				}
				reply->setId(idOfCommandPacket);
				_connector.sendPacket(reply);
				break;
			}

			case MessageQueue::THREAD_GROUP: {
				auto reply = std::make_shared<JdwpReplyTrthreadgroupPacket<Vm>>();
				try {
					typename Vm::ThreadGroupID gid;

					_toConnector.getData(&gid);
					reply->setGroup(gid);
				} catch (std::exception &e) {
					reply->setErrorCode(JdwpReplyPacket::INVALID_THREAD);
				}
				reply->setId(idOfCommandPacket);
				_connector.sendPacket(reply);
				break;
			}

			case MessageQueue::THREAD_FRAMES: {
				auto reply = std::make_shared<JdwpReplyTrframesPacket<Vm>>();
				/*
				try {
					auto some
					_toConnector.getData(&some);
				} catch (std::exception &e) {
					reply->setErrorCode(JdwpReplyPacket::INVALID_THREAD);
				}
				*/
				reply->setId(idOfCommandPacket);
				_connector.sendPacket(reply);
				break;
			}

			case MessageQueue::THREAD_GROUP_NAME: {
				auto reply = std::make_shared<JdwpReplyTgrnamePacket<Vm>>();
				try {
					std::u16string name;
					_toConnector.getData(&name);
					std::wstring_convert<std::codecvt_utf8<char16_t>,char16_t> cv;
					reply->setGroupName(cv.to_bytes(name));
				} catch (std::exception &e) {
					reply->setErrorCode(JdwpReplyPacket::INVALID_THREAD_GROUP);
				}
				reply->setId(idOfCommandPacket);
				_connector.sendPacket(reply);
				break;
			}

			case MessageQueue::THREAD_GROUP_PARENT: {
				auto reply = std::make_shared<JdwpReplyTgrparentPacket<Vm>>();
				try {
					typename Vm::ThreadGroupID parent;
					_toConnector.getData(&parent);
					reply->setParentGroup(parent);
				} catch (std::exception &e) {
					reply->setErrorCode(JdwpReplyPacket::INVALID_THREAD_GROUP);
				}
				reply->setId(idOfCommandPacket);
				_connector.sendPacket(reply);
				break;
			}

			case MessageQueue::THREAD_GROUP_CHILDREN: {
				auto reply = std::make_shared<JdwpReplyTgrchildrenPacket<Vm>>();
				try {
					typename Vm::ThreadGroupInformation g;
					_toConnector.getData(&g);
					reply->setChildThreads(g.threads);
					reply->setChildGroups(g.groups);
				} catch (std::exception &e) {
					reply->setErrorCode(JdwpReplyPacket::INVALID_THREAD_GROUP);
				}
				reply->setId(idOfCommandPacket);
				_connector.sendPacket(reply);
				break;
			}

			default:
				break;
			}
			if (id == messageToProcess) {
				proceed = true;
			}
			_toConnector.pop();
		} else if (proceed == false) {
			std::this_thread::sleep_for (std::chrono::milliseconds(100));
		}
	}
}

#endif /* JIKESDBG_SRC_DEBUGPROCESS_HPP_ */
