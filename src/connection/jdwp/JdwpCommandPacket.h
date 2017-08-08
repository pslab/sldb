/*
 * JdwpCommandPacket.h
 *
 *  Created on: 2016. 3. 2.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_JDWPCOMMANDPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_JDWPCOMMANDPACKET_H_

#include "JdwpPacket.h"

#include <atomic>

class JdwpCommandPacket: public JdwpPacket {
public:
	// Command Sets.
	static const std::uint32_t CSET_VIRTUAL_MACHINE = 1;
	static const std::uint32_t CSET_REFERENCE_TYPE = 2;
	static const std::uint32_t CSET_CLASS_TYPE = 3;
	static const std::uint32_t CSET_ARRAY_TYPE = 4;
	static const std::uint32_t CSET_INTERFACE_TYPE = 5;
	static const std::uint32_t CSET_METHOD = 6;
	static const std::uint32_t CSET_FIELD = 8;
	static const std::uint32_t CSET_OBJECT_REFERENCE = 9;
	static const std::uint32_t CSET_STRING_REFERENCE = 10;
	static const std::uint32_t CSET_THREAD_REFERENCE = 11;
	static const std::uint32_t CSET_THREAD_GROUP_REFERENCE = 12;
	static const std::uint32_t CSET_ARRAY_REFERENCE = 13;
	static const std::uint32_t CSET_CLASS_LOADER_REFERENCE = 14;
	static const std::uint32_t CSET_EVENT_REQUEST = 15;
	static const std::uint32_t CSET_STACK_FRAME = 16;
	static const std::uint32_t CSET_CLASS_OBJECT_REFERENCE = 17;
	static const std::uint32_t CSET_EVENT = 64;
	static const std::uint32_t CSET_HOT_CODE_REPLACEMENT = 128;

	// Commands VirtualMachine.
	static const std::uint32_t VM_VERSION = 1 + (CSET_VIRTUAL_MACHINE << 8);
	static const std::uint32_t VM_CLASSES_BY_SIGNATURE = 2 + (CSET_VIRTUAL_MACHINE << 8);
	static const std::uint32_t VM_ALL_CLASSES = 3 + (CSET_VIRTUAL_MACHINE << 8);
	static const std::uint32_t VM_ALL_THREADS = 4 + (CSET_VIRTUAL_MACHINE << 8);
	static const std::uint32_t VM_TOP_LEVEL_THREAD_GROUPS = 5 + (CSET_VIRTUAL_MACHINE << 8);
	static const std::uint32_t VM_DISPOSE = 6 + (CSET_VIRTUAL_MACHINE << 8);
	static const std::uint32_t VM_ID_SIZES = 7 + (CSET_VIRTUAL_MACHINE << 8);
	static const std::uint32_t VM_SUSPEND = 8 + (CSET_VIRTUAL_MACHINE << 8);
	static const std::uint32_t VM_RESUME = 9 + (CSET_VIRTUAL_MACHINE << 8);
	static const std::uint32_t VM_EXIT = 10 + (CSET_VIRTUAL_MACHINE << 8);
	static const std::uint32_t VM_CREATE_STRING = 11 + (CSET_VIRTUAL_MACHINE << 8);
	static const std::uint32_t VM_CAPABILITIES = 12 + (CSET_VIRTUAL_MACHINE << 8);
	static const std::uint32_t VM_CLASS_PATHS = 13 + (CSET_VIRTUAL_MACHINE << 8);
	static const std::uint32_t VM_DISPOSE_OBJECTS = 14 + (CSET_VIRTUAL_MACHINE << 8);
	static const std::uint32_t VM_HOLD_EVENTS = 15 + (CSET_VIRTUAL_MACHINE << 8);
	static const std::uint32_t VM_RELEASE_EVENTS = 16 + (CSET_VIRTUAL_MACHINE << 8);
	static const std::uint32_t VM_CAPABILITIES_NEW = 17 + (CSET_VIRTUAL_MACHINE << 8);
	static const std::uint32_t VM_REDEFINE_CLASSES = 18 + (CSET_VIRTUAL_MACHINE << 8);
	static const std::uint32_t VM_SET_DEFAULT_STRATUM = 19 + (CSET_VIRTUAL_MACHINE << 8);
	static const std::uint32_t VM_ALL_CLASSES_WITH_GENERIC = 20 + (CSET_VIRTUAL_MACHINE << 8);
	static const std::uint32_t VM_INSTANCE_COUNTS = 21 + (CSET_VIRTUAL_MACHINE << 8);

	// Commands ReferenceType.
	static const std::uint32_t RT_SIGNATURE = 1 + (CSET_REFERENCE_TYPE << 8);
	static const std::uint32_t RT_CLASS_LOADER = 2 + (CSET_REFERENCE_TYPE << 8);
	static const std::uint32_t RT_MODIFIERS = 3 + (CSET_REFERENCE_TYPE << 8);
	static const std::uint32_t RT_FIELDS = 4 + (CSET_REFERENCE_TYPE << 8);
	static const std::uint32_t RT_METHODS = 5 + (CSET_REFERENCE_TYPE << 8);
	static const std::uint32_t RT_GET_VALUES = 6 + (CSET_REFERENCE_TYPE << 8);
	static const std::uint32_t RT_SOURCE_FILE = 7 + (CSET_REFERENCE_TYPE << 8);
	static const std::uint32_t RT_NESTED_TYPES = 8 + (CSET_REFERENCE_TYPE << 8);
	static const std::uint32_t RT_STATUS = 9 + (CSET_REFERENCE_TYPE << 8);
	static const std::uint32_t RT_INTERFACES = 10 + (CSET_REFERENCE_TYPE << 8);
	static const std::uint32_t RT_CLASS_OBJECT = 11 + (CSET_REFERENCE_TYPE << 8);
	static const std::uint32_t RT_SOURCE_DEBUG_EXTENSION = 12 + (CSET_REFERENCE_TYPE << 8);
	static const std::uint32_t RT_SIGNATURE_WITH_GENERIC = 13 + (CSET_REFERENCE_TYPE << 8);
	static const std::uint32_t RT_FIELDS_WITH_GENERIC = 14 + (CSET_REFERENCE_TYPE << 8);
	static const std::uint32_t RT_METHODS_WITH_GENERIC = 15 + (CSET_REFERENCE_TYPE << 8);
	static const std::uint32_t RT_INSTANCES = 16 + (CSET_REFERENCE_TYPE << 8);
	static const std::uint32_t RT_CLASS_VERSION = 17 + (CSET_REFERENCE_TYPE << 8);
	static const std::uint32_t RT_CONSTANT_POOL = 18 + (CSET_REFERENCE_TYPE << 8);

	// Commands ClassType.
	static const std::uint32_t CT_SUPERCLASS = 1 + (CSET_CLASS_TYPE << 8);
	static const std::uint32_t CT_SET_VALUES = 2 + (CSET_CLASS_TYPE << 8);
	static const std::uint32_t CT_INVOKE_METHOD = 3 + (CSET_CLASS_TYPE << 8);
	static const std::uint32_t CT_NEW_INSTANCE = 4 + (CSET_CLASS_TYPE << 8);

	// Commands ArrayType.
	static const std::uint32_t AT_NEW_INSTANCE = 1 + (CSET_ARRAY_TYPE << 8);

	// Commands Method.
	static const std::uint32_t M_LINE_TABLE = 1 + (CSET_METHOD << 8);
	static const std::uint32_t M_VARIABLE_TABLE = 2 + (CSET_METHOD << 8);
	static const std::uint32_t M_BYTECODES = 3 + (CSET_METHOD << 8);
	static const std::uint32_t M_OBSOLETE = 4 + (CSET_METHOD << 8);
	static const std::uint32_t M_VARIABLE_TABLE_WITH_GENERIC = 5 + (CSET_METHOD << 8);

	// Commands ObjectReference.
	static const std::uint32_t OR_REFERENCE_TYPE = 1 + (CSET_OBJECT_REFERENCE << 8);
	static const std::uint32_t OR_GET_VALUES = 2 + (CSET_OBJECT_REFERENCE << 8);
	static const std::uint32_t OR_SET_VALUES = 3 + (CSET_OBJECT_REFERENCE << 8);
	static const std::uint32_t OR_MONITOR_INFO = 5 + (CSET_OBJECT_REFERENCE << 8);
	static const std::uint32_t OR_INVOKE_METHOD = 6 + (CSET_OBJECT_REFERENCE << 8);
	static const std::uint32_t OR_DISABLE_COLLECTION = 7 + (CSET_OBJECT_REFERENCE << 8);
	static const std::uint32_t OR_ENABLE_COLLECTION = 8 + (CSET_OBJECT_REFERENCE << 8);
	static const std::uint32_t OR_IS_COLLECTED = 9 + (CSET_OBJECT_REFERENCE << 8);
	static const std::uint32_t OR_REFERRING_OBJECTS = 10 + (CSET_OBJECT_REFERENCE << 8);

	// Commands StringReference.
	static const std::uint32_t SR_VALUE = 1 + (CSET_STRING_REFERENCE << 8);

	// Commands ThreadReference.
	static const std::uint32_t TR_NAME = 1 + (CSET_THREAD_REFERENCE << 8);
	static const std::uint32_t TR_SUSPEND = 2 + (CSET_THREAD_REFERENCE << 8);
	static const std::uint32_t TR_RESUME = 3 + (CSET_THREAD_REFERENCE << 8);
	static const std::uint32_t TR_STATUS = 4 + (CSET_THREAD_REFERENCE << 8);
	static const std::uint32_t TR_THREAD_GROUP = 5 + (CSET_THREAD_REFERENCE << 8);
	static const std::uint32_t TR_FRAMES = 6 + (CSET_THREAD_REFERENCE << 8);
	static const std::uint32_t TR_FRAME_COUNT = 7 + (CSET_THREAD_REFERENCE << 8);
	static const std::uint32_t TR_OWNED_MONITORS = 8 + (CSET_THREAD_REFERENCE << 8);
	static const std::uint32_t TR_CURRENT_CONTENDED_MONITOR = 9 + (CSET_THREAD_REFERENCE << 8);
	static const std::uint32_t TR_STOP = 10 + (CSET_THREAD_REFERENCE << 8);
	static const std::uint32_t TR_INTERRUPT = 11 + (CSET_THREAD_REFERENCE << 8);
	static const std::uint32_t TR_SUSPEND_COUNT = 12 + (CSET_THREAD_REFERENCE << 8);
	static const std::uint32_t TR_OWNED_MONITOR_STACK_DEPTH = 13 + (CSET_THREAD_REFERENCE << 8);
	static const std::uint32_t TR_FORCE_EARLY_RETURN = 14 + (CSET_THREAD_REFERENCE << 8);

	// Commands ThreadGroupReference.
	static const std::uint32_t TGR_NAME = 1 + (CSET_THREAD_GROUP_REFERENCE << 8);
	static const std::uint32_t TGR_PARENT = 2 + (CSET_THREAD_GROUP_REFERENCE << 8);
	static const std::uint32_t TGR_CHILDREN = 3 + (CSET_THREAD_GROUP_REFERENCE << 8);

	// Commands ArrayReference.
	static const std::uint32_t AR_LENGTH = 1 + (CSET_ARRAY_REFERENCE << 8);
	static const std::uint32_t AR_GET_VALUES = 2 + (CSET_ARRAY_REFERENCE << 8);
	static const std::uint32_t AR_SET_VALUES = 3 + (CSET_ARRAY_REFERENCE << 8);

	// Commands ClassLoaderReference.
	static const std::uint32_t CLR_VISIBLE_CLASSES = 1 + (CSET_CLASS_LOADER_REFERENCE << 8);

	// Commands EventRequest.
	static const std::uint32_t ER_SET = 1 + (CSET_EVENT_REQUEST << 8);
	static const std::uint32_t ER_CLEAR = 2 + (CSET_EVENT_REQUEST << 8);
	static const std::uint32_t ER_CLEAR_ALL_BREAKPOINTS = 3 + (CSET_EVENT_REQUEST << 8);

	// Commands StackFrame.
	static const std::uint32_t SF_GET_VALUES = 1 + (CSET_STACK_FRAME << 8);
	static const std::uint32_t SF_SET_VALUES = 2 + (CSET_STACK_FRAME << 8);
	static const std::uint32_t SF_THIS_OBJECT = 3 + (CSET_STACK_FRAME << 8);
	static const std::uint32_t SF_POP_FRAME = 4 + (CSET_STACK_FRAME << 8);

	// Commands ClassObjectReference.
	static const std::uint32_t COR_REFLECTED_TYPE = 1 + (CSET_CLASS_OBJECT_REFERENCE << 8);

	// Commands Event.
	static const std::uint32_t E_COMPOSITE = 100 + (CSET_EVENT << 8);

	// Commands Hot Code Replacement (OTI specific).
	static const std::uint32_t HCR_CLASSES_HAVE_CHANGED = 1 + (CSET_HOT_CODE_REPLACEMENT << 8);
	static const std::uint32_t HCR_GET_CLASS_VERSION = 2 + (CSET_HOT_CODE_REPLACEMENT << 8);
	static const std::uint32_t HCR_DO_RETURN = 3 + (CSET_HOT_CODE_REPLACEMENT << 8);
	static const std::uint32_t HCR_REENTER_ON_EXIT = 4 + (CSET_HOT_CODE_REPLACEMENT << 8);
	static const std::uint32_t HCR_CAPABILITIES = 5 + (CSET_HOT_CODE_REPLACEMENT << 8);

	explicit JdwpCommandPacket(std::uint32_t command);
	JdwpCommandPacket(const JdwpCommandPacket &packet);
	virtual ~JdwpCommandPacket();

	std::uint32_t getNextId() { return _nextId++; }
	std::uint8_t getCommandSet() const { return static_cast<std::uint8_t>(_command) >> 8; }
	std::uint32_t getCommand() const { return _command; }
	void setCommand(std::uint32_t command) { _command = command; }

	static std::uint32_t getCommandFromHeader(const std::vector<std::uint8_t> &bytes);

	virtual void print(std::ostream &out) const override;

protected:
	virtual void readSpecificHeaderFields(const std::vector<std::uint8_t> &bytes) final override;
	virtual void writeSpecificHeaderFields(std::vector<std::uint8_t> &bytes) const final override;
	virtual void readData() override {}
	virtual void writeData() override {}

private:
	static const std::size_t INDEX_COMMAND_SET = 9;
	static const std::size_t INDEX_COMMAND = INDEX_COMMAND_SET + 1;
	static std::atomic<std::uint32_t> _nextId;

	std::uint32_t _command = 0;
};

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_JDWPCOMMANDPACKET_H_ */
