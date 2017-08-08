/*
 * DebugProcess.h
 *
 *  Created on: 2016. 3. 21.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_DEBUGPROCESS_H_
#define JIKESDBG_SRC_DEBUGPROCESS_H_

#include <queue>
#include <unordered_map>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <stdexcept>
#include <cassert>

#include "connection/Connector.h"
#include "Debugger.h"

template <typename Vm>
class DebugProcess {
public:

	static DebugProcess* getInstance() { return &instance; }
	virtual void start();

protected:
	DebugProcess() :_exited(true) {}
	virtual ~DebugProcess() {}

	virtual void debug();
	virtual void connect();

private:
	class MessageQueue {
	public:
		enum Id {
			NONE,
			VM_START,
			VM_EXIT,
			VM_CLASSES,
			ALL_THREADS,
			SUPERCLASS,
			FILES_WITH_GENERIC,
			INTERFACES,
			REFERENCE_TYPE,
			THREAD_START,
			THREAD_DEATH,
			THREAD_NAME,
			THREAD_SUSPEND,
			THREAD_RESUME,
			THREAD_STATUS,
			THREAD_GROUP,
			THREAD_FRAMES,
			THREAD_GROUP_NAME,
			THREAD_GROUP_PARENT,
			THREAD_GROUP_CHILDREN
		};

		MessageQueue() {}

		bool empty() {
			std::lock_guard<std::mutex> lock(_lock);
			return _queue.empty();
		}

		void pushMessage(Id id) {
			auto d = std::shared_ptr<void>();
			Message msg = {id, d, false};
			std::lock_guard<std::mutex> lock(_lock);
			_queue.push(msg);
		}

		template <typename T>
		void pushMessage(Id id, const T &data) {
			auto d = std::make_shared<T>(data);
			Message msg = {id, d, true};
			std::lock_guard<std::mutex> lock(_lock);
			_queue.push(msg);
		}

		Id getId() {
			std::lock_guard<std::mutex> lock(_lock);
			Message msg = _queue.front();
			return msg.id;
		}

		template <typename T>
		void getData(T *data) {
			std::lock_guard<std::mutex> lock(_lock);
			Message msg = _queue.front();
			if (msg.dataExist == true) {
				*data = *std::static_pointer_cast<T>(msg.data);
			} else {
				throw std::out_of_range("DebugProcess::MessageQueue::getData(T *data)");
			}
		}

		void pop() {
			std::lock_guard<std::mutex> lock(_lock);
			_queue.pop();
		}

	private:
		struct Message {Id id; std::shared_ptr<void> data; bool dataExist; };
		std::queue<Message> _queue;
		std::mutex _lock;
	};

	static DebugProcess instance;

	std::atomic_bool _exited;

	Connector _connector;
	Debugger _debugger;

	MessageQueue _toConnector;
	MessageQueue _toDebugger;
	std::unordered_map<Byte, Int> _requestIdMap;
	std::unordered_map<Byte, Byte> _suspendPolicyMap;

	virtual void debuggerMessageLoop();
	virtual void connectionMessageLoop(typename MessageQueue::Id messageToProcess, Int idOfCommandPacket);
};

#include "DebugProcess.hpp"

#endif /* JIKESDBG_SRC_DEBUGPROCESS_H_ */
