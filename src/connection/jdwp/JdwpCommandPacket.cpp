/*
 * JdwpCommandPacket.cpp
 *
 *  Created on: 2016. 3. 2.
 *      Author: Seunghun Han
 */

#include "JdwpCommandPacket.h"

std::atomic<std::uint32_t> JdwpCommandPacket::_nextId(1);

JdwpCommandPacket::JdwpCommandPacket(std::uint32_t command) : _command(command)
{
	setId(getNextId());
}

JdwpCommandPacket::JdwpCommandPacket(const JdwpCommandPacket &packet) : JdwpPacket(packet), _command(packet._command)
{
}

JdwpCommandPacket::~JdwpCommandPacket()
{
}

void JdwpCommandPacket::print(std::ostream &out) const
{
	out << '[';
	out << getId();
	out << "] ";
	switch (getCommand()) {

	// Commands VirtualMachine.
	case VM_VERSION:
		out << "VM_VERSION";
		break;
	case VM_CLASSES_BY_SIGNATURE:
		out << "VM_CLASSES_BY_SIGNATURE";
		break;
	case VM_ALL_CLASSES:
		out << "VM_ALL_CLASSES";
		break;
	case VM_ALL_THREADS:
		out << "VM_ALL_THREADS";
		break;
	case VM_TOP_LEVEL_THREAD_GROUPS:
		out << "VM_TOP_LEVEL_THREAD_GROUPS";
		break;
	case VM_DISPOSE:
		out << "VM_DISPOSE";
		break;
	case VM_ID_SIZES:
		out << "VM_ID_SIZES";
		break;
	case VM_SUSPEND:
		out << "VM_SUSPEND";
		break;
	case VM_RESUME:
		out << "VM_RESUME";
		break;
	case VM_EXIT:
		out << "VM_EXIT";
		break;
	case VM_CREATE_STRING:
		out << "VM_CREATE_STRING";
		break;
	case VM_CAPABILITIES:
		out << "VM_CAPABILITIES";
		break;
	case VM_CLASS_PATHS:
		out << "VM_CLASS_PATHS";
		break;
	case VM_DISPOSE_OBJECTS:
		out << "VM_DISPOSE_OBJECTS";
		break;
	case VM_HOLD_EVENTS:
		out << "VM_HOLD_EVENTS";
		break;
	case VM_RELEASE_EVENTS:
		out << "VM_RELEASE_EVENTS";
		break;
	case VM_CAPABILITIES_NEW:
		out << "VM_CAPABILITIES_NEW";
		break;
	case VM_REDEFINE_CLASSES:
		out << "VM_REDEFINE_CLASSES";
		break;
	case VM_SET_DEFAULT_STRATUM:
		out << "VM_SET_DEFAULT_STRATUM";
		break;
	case VM_ALL_CLASSES_WITH_GENERIC:
		out << "VM_ALL_CLASSES_WITH_GENERIC";
		break;

	// Commands ReferenceType.
	case RT_SIGNATURE:
		out << "RT_SIGNATURE";
		break;
	case RT_CLASS_LOADER:
		out << "RT_CLASS_LOADER";
		break;
	case RT_MODIFIERS:
		out << "RT_MODIFIERS";
		break;
	case RT_FIELDS:
		out << "RT_FIELDS";
		break;
	case RT_METHODS:
		out << "RT_METHODS";
		break;
	case RT_GET_VALUES:
		out << "RT_GET_VALUES";
		break;
	case RT_SOURCE_FILE:
		out << "RT_SOURCE_FILE";
		break;
	case RT_NESTED_TYPES:
		out << "RT_NESTED_TYPES";
		break;
	case RT_STATUS:
		out << "RT_STATUS";
		break;
	case RT_INTERFACES:
		out << "RT_INTERFACES";
		break;
	case RT_CLASS_OBJECT:
		out << "RT_CLASS_OBJECT";
		break;
	case RT_SOURCE_DEBUG_EXTENSION:
		out << "RT_SOURCE_DEBUG_EXTENSION";
		break;
	case RT_SIGNATURE_WITH_GENERIC:
		out << "RT_SIGNATURE_WITH_GENERIC";
		break;
	case RT_FIELDS_WITH_GENERIC:
		out << "RT_FIELDS_WITH_GENERIC";
		break;
	case RT_METHODS_WITH_GENERIC:
		out << "RT_METHODS_WITH_GENERIC";
		break;

	// Commands ClassType.
	case CT_SUPERCLASS:
		out << "CT_SUPERCLASS";
		break;
	case CT_SET_VALUES:
		out << "CT_SET_VALUES";
		break;
	case CT_INVOKE_METHOD:
		out << "CT_INVOKE_METHOD";
		break;
	case CT_NEW_INSTANCE:
		out << "CT_NEW_INSTANCE";
		break;

	// Commands ArrayType.
	case AT_NEW_INSTANCE:
		out << "AT_NEW_INSTANCE";
		break;

	// Commands Method.
	case M_LINE_TABLE:
		out << "M_LINE_TABLE";
		break;
	case M_VARIABLE_TABLE:
		out << "M_VARIABLE_TABLE";
		break;
	case M_BYTECODES:
		out << "M_BYTECODES";
		break;
	case M_OBSOLETE:
		out << "M_OBSOLETE";
		break;
	case M_VARIABLE_TABLE_WITH_GENERIC:
		out << "M_VARIABLE_TABLE_WITH_GENERIC";
		break;

	// Commands ObjectReference.
	case OR_REFERENCE_TYPE:
		out << "OR_REFERENCE_TYPE";
		break;
	case OR_GET_VALUES:
		out << "OR_GET_VALUES";
		break;
	case OR_SET_VALUES:
		out << "OR_SET_VALUES";
		break;
	case OR_MONITOR_INFO:
		out << "OR_MONITOR_INFO";
		break;
	case OR_INVOKE_METHOD:
		out << "OR_INVOKE_METHOD";
		break;
	case OR_DISABLE_COLLECTION:
		out << "OR_DISABLE_COLLECTION";
		break;
	case OR_ENABLE_COLLECTION:
		out << "OR_ENABLE_COLLECTION";
		break;
	case OR_IS_COLLECTED:
		out << "OR_IS_COLLECTED";
		break;

	// Commands StringReference.
	case SR_VALUE:
		out << "SR_VALUE";
		break;

	// Commands ThreadReference.
	case TR_NAME:
		out << "TR_NAME";
		break;
	case TR_SUSPEND:
		out << "TR_SUSPEND";
		break;
	case TR_RESUME:
		out << "TR_RESUME";
		break;
	case TR_STATUS:
		out << "TR_STATUS";
		break;
	case TR_THREAD_GROUP:
		out << "TR_THREAD_GROUP";
		break;
	case TR_FRAMES:
		out << "TR_FRAMES";
		break;
	case TR_FRAME_COUNT:
		out << "TR_FRAME_COUNT";
		break;
	case TR_OWNED_MONITORS:
		out << "TR_OWNED_MONITORS";
		break;
	case TR_CURRENT_CONTENDED_MONITOR:
		out << "TR_CURRENT_CONTENDED_MONITOR";
		break;
	case TR_STOP:
		out << "TR_STOP";
		break;
	case TR_INTERRUPT:
		out << "TR_INTERRUPT";
		break;
	case TR_SUSPEND_COUNT:
		out << "TR_SUSPEND_COUNT";
		break;

	// Commands ThreadGroupReference.
	case TGR_NAME:
		out << "TGR_NAME";
		break;
	case TGR_PARENT:
		out << "TGR_PARENT";
		break;
	case TGR_CHILDREN:
		out << "TGR_CHILDREN";
		break;

	// Commands ArrayReference.
	case AR_LENGTH:
		out << "AR_LENGTH";
		break;
	case AR_GET_VALUES:
		out << "AR_GET_VALUES";
		break;
	case AR_SET_VALUES:
		out << "AR_SET_VALUES";
		break;

	// Commands ClassLoaderReference.
	case CLR_VISIBLE_CLASSES:
		out << "CLR_VISIBLE_CLASSES";
		break;

	// Commands EventRequest.
	case ER_SET:
		out << "ER_SET";
		break;
	case ER_CLEAR:
		out << "ER_CLEAR";
		break;
	case ER_CLEAR_ALL_BREAKPOINTS:
		out << "ER_CLEAR_ALL_BREAKPOINTS";
		break;

	// Commands StackFrame.
	case SF_GET_VALUES:
		out << "SF_GET_VALUES";
		break;
	case SF_SET_VALUES:
		out << "SF_SET_VALUES";
		break;
	case SF_THIS_OBJECT:
		out << "SF_THIS_OBJECT";
		break;
	case SF_POP_FRAME:
		out << "SF_POP_FRAME";
		break;

	// Commands ClassObjectReference.
	case COR_REFLECTED_TYPE:
		out << "COR_REFLECTED_TYPE";
		break;

	// Commands Event.
	case E_COMPOSITE:
		out << "E_COMPOSITE";
		break;

	default:
		out << "UNKNOWN COMMAND: ";
		out << getCommand();
		break;
	}
}

void JdwpCommandPacket::readSpecificHeaderFields(const std::vector<std::uint8_t> &bytes)
{
	_command = getCommandFromHeader(bytes);
}

void JdwpCommandPacket::writeSpecificHeaderFields(std::vector<std::uint8_t> &bytes) const
{
	bytes[INDEX_COMMAND_SET] = getCommandSet();
	bytes[INDEX_COMMAND] = static_cast<std::uint8_t>(_command);
}

std::uint32_t JdwpCommandPacket::getCommandFromHeader(const std::vector<std::uint8_t> &bytes)
{
	std::uint8_t commandSet = bytes[INDEX_COMMAND_SET];
	return bytes[INDEX_COMMAND] + (static_cast<std::uint32_t>(commandSet) << 8);
}
