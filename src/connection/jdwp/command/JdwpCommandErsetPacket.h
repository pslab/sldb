/*
 * JdwpCommandErsetPacket.h
 *
 *  Created on: 2016. 3. 9.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDERSETPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDERSETPACKET_H_

#include "../JdwpCommandPacket.h"

#include <vector>
#include <memory>

#include "../JdwpTypes.h"

/**
 * EventRequest Command Set (15)
 * Set Command (1)
 */
template <typename Vm>
class JdwpCommandErsetPacket : public JdwpCommandPacket {
public:
	// Modifiers
	class Modifier {
	public:
		Int getModKind() const { return _modKind; }

		virtual std::size_t size() const = 0;
		virtual void print(std::ostream &out) const = 0;

	protected:
		Modifier(Int modKind) : _modKind(modKind) {}
		~Modifier() {}

	private:
		Int _modKind = 0;
	};

	class Modifier1 final : public Modifier {
	public:
		Modifier1() : Modifier(1) {}
		~Modifier1() {}

		Int getCount() const { return _count; }

		virtual std::size_t size() const override { return sizeof(Int)+sizeof(_count); }
		virtual void print(std::ostream &out) const override;

		static std::shared_ptr<Modifier1> unpack(const std::vector<std::uint8_t> &vector) { return unpack(vector.cbegin()); }
		static std::shared_ptr<Modifier1> unpack(std::vector<std::uint8_t>::const_iterator iter);

	private:
		Int _count = 0;
	};

	class Modifier2 final : public Modifier {
	public:
		Modifier2() : Modifier(2) {}
		~Modifier2() {}

		Int getExprID() const { return _exprID; }

		virtual std::size_t size() const override { return sizeof(Int)+sizeof(_exprID);}
		virtual void print(std::ostream &out) const;

		static std::shared_ptr<Modifier2> unpack(const std::vector<std::uint8_t> &vector) { return unpack(vector.cbegin()); }
		static std::shared_ptr<Modifier2> unpack(std::vector<std::uint8_t>::const_iterator iter);

	private:
		Int _exprID = 0;
	};

	class Modifier3 final : public Modifier {
	public:
		Modifier3() : Modifier(3) {}
		~Modifier3() {}

		typename JdwpTypes<Vm>::ThreadID getThread() const { return _thread; }

		virtual std::size_t size() const override { return sizeof(Int)+sizeof(_thread);}
		virtual void print(std::ostream &out) const override;

		static std::shared_ptr<Modifier3> unpack(const std::vector<std::uint8_t> &vector) { return unpack(vector.cbegin()); }
		static std::shared_ptr<Modifier3> unpack(std::vector<std::uint8_t>::const_iterator iter);

	private:
		typename JdwpTypes<Vm>::ThreadID _thread = 0;
	};

	class Modifier4 final : public Modifier {
	public:
		Modifier4() : Modifier(4) {}
		~Modifier4() {}

		typename JdwpTypes<Vm>::ReferenceTypeID getClazz() const { return _clazz; }

		virtual std::size_t size() const override { return sizeof(Int)+sizeof(_clazz);}
		virtual void print(std::ostream &out) const override;

		static std::shared_ptr<Modifier4> unpack(const std::vector<std::uint8_t> &vector) { return unpack(vector.cbegin()); }
		static std::shared_ptr<Modifier4> unpack(std::vector<std::uint8_t>::const_iterator iter);

	private:
		typename JdwpTypes<Vm>::ReferenceTypeID _clazz = 0;
	};

	class Modifier5 final : public Modifier {
	public:
		Modifier5() : Modifier(5) {}
		~Modifier5() {}

		typename JdwpTypes<Vm>::String getClassPattern() const { return _classPattern; }

		virtual std::size_t size() const override { return sizeof(Int)+_classPattern.size();}
		virtual void print(std::ostream &out) const;

		static std::shared_ptr<Modifier5> unpack(const std::vector<std::uint8_t> &vector) { return unpack(vector.cbegin()); }
		static std::shared_ptr<Modifier5> unpack(std::vector<std::uint8_t>::const_iterator iter);

	private:
		typename JdwpTypes<Vm>::String _classPattern;
	};

	class Modifier6 final : public Modifier {
	public:
		Modifier6() : Modifier(6) {}
		~Modifier6() {}

		typename JdwpTypes<Vm>::String getClassPattern() const { return _classPattern; }

		virtual std::size_t size() const override { return sizeof(Int)+_classPattern.size();}
		virtual void print(std::ostream &out) const override;

		static std::shared_ptr<Modifier6> unpack(const std::vector<std::uint8_t> &vector) { return unpack(vector.cbegin()); }
		static std::shared_ptr<Modifier6> unpack(std::vector<std::uint8_t>::const_iterator iter);

	private:
		typename JdwpTypes<Vm>::String _classPattern;
	};

	class Modifier7 final : public Modifier {
	public:
		Modifier7() : Modifier(7) {}
		~Modifier7() {}

		typename JdwpTypes<Vm>::Location getLoc() const { return _loc; }

		virtual std::size_t size() const override { return sizeof(Int)+_loc.size();}
		virtual void print(std::ostream &out) const override;

		static std::shared_ptr<Modifier7> unpack(const std::vector<std::uint8_t> &vector) { return unpack(vector.cbegin()); }
		static std::shared_ptr<Modifier7> unpack(std::vector<std::uint8_t>::const_iterator iter);

	private:
		typename JdwpTypes<Vm>::Location _loc;
	};

	class Modifier8 final : public Modifier {
	public:
		Modifier8() : Modifier(8) {}
		~Modifier8() {}

		typename JdwpTypes<Vm>::ReferenceTypeID getExceptionOrNull() const { return _exceptionOrNull; }
		Boolean getCaught() const { return _caught; }
		Boolean getUncaught() const { return _uncaught; }

		virtual std::size_t size() const override { return sizeof(Int)+sizeof(_exceptionOrNull)+sizeof(_caught)+sizeof(_uncaught);}
		virtual void print(std::ostream &out) const override;

		static std::shared_ptr<Modifier8> unpack(const std::vector<std::uint8_t> &vector) { return unpack(vector.cbegin()); }
		static std::shared_ptr<Modifier8> unpack(std::vector<std::uint8_t>::const_iterator iter);

	private:
		typename JdwpTypes<Vm>::ReferenceTypeID _exceptionOrNull = 0;
		Boolean _caught = 0;
		Boolean _uncaught = 0;
	};

	class Modifier9 final : public Modifier {
	public:
		Modifier9() : Modifier(9) {}
		~Modifier9() {}

		typename JdwpTypes<Vm>::ReferenceTypeID getDeclaring() const { return _declaring; }
		typename JdwpTypes<Vm>::FieldID getFieldID() const { return _fieldID; }

		virtual std::size_t size() const override { return sizeof(Int)+sizeof(_declaring)+sizeof(_fieldID);}
		virtual void print(std::ostream &out) const override;

		static std::shared_ptr<Modifier9> unpack(const std::vector<std::uint8_t> &vector) { return unpack(vector.cbegin()); }
		static std::shared_ptr<Modifier9> unpack(std::vector<std::uint8_t>::const_iterator iter);

	private:
		typename JdwpTypes<Vm>::ReferenceTypeID _declaring = 0;
		typename JdwpTypes<Vm>::FieldID _fieldID = 0;
	};

	class Modifier10 final : public Modifier {
	public:
		Modifier10() : Modifier(10) {}
		~Modifier10() {}

		typename JdwpTypes<Vm>::ThreadID getThread() const { return _thread; }
		Int getSize() const { return _size; }
		Int getDepth() const { return _depth; }

		virtual std::size_t size() const override { return sizeof(Int)+sizeof(_thread)+sizeof(_size)+sizeof(_depth);}
		virtual void print(std::ostream &out) const override;

		static std::shared_ptr<Modifier10> unpack(const std::vector<std::uint8_t> &vector) { return unpack(vector.cbegin()); }
		static std::shared_ptr<Modifier10> unpack(std::vector<std::uint8_t>::const_iterator iter);

	private:
		typename JdwpTypes<Vm>::ThreadID _thread = 0;
		Int _size = 0;
		Int _depth = 0;
	};

	class Modifier11 final : public Modifier {
	public:
		Modifier11() : Modifier(11) {}
		~Modifier11() {}
		typename JdwpTypes<Vm>::ObjectID getInstance() const { return _instance; }

		virtual std::size_t size() const override { return sizeof(Int)+sizeof(_instance);}
		virtual void print(std::ostream &out) const override;

		static std::shared_ptr<Modifier11> unpack(const std::vector<std::uint8_t> &vector) { return unpack(vector.cbegin()); }
		static std::shared_ptr<Modifier11> unpack(std::vector<std::uint8_t>::const_iterator iter);

	private:
		typename JdwpTypes<Vm>::ObjectID _instance = 0;
	};

	class Modifier12 final : public Modifier {
		public:
			Modifier12() : Modifier(12) {}
			~Modifier12() {}
			typename JdwpTypes<Vm>::String getSourceNamePattern() const { return _sourceNamePattern; }

			virtual std::size_t size() const override { return sizeof(Int)+_sourceNamePattern.size();}
			virtual void print(std::ostream &out) const override;

			static std::shared_ptr<Modifier12> unpack(const std::vector<std::uint8_t> &vector) { return unpack(vector.cbegin()); }
			static std::shared_ptr<Modifier12> unpack(std::vector<std::uint8_t>::const_iterator iter);

		private:
			typename JdwpTypes<Vm>::String _sourceNamePattern;
		};

	JdwpCommandErsetPacket(const JdwpCommandPacket &packet);
	virtual ~JdwpCommandErsetPacket();

	JdwpEventKind getEventKind() { return _eventKind; }
	Byte getSuspendPolicy() { return _suspendPolicy; }
	Int getModifiers() { return _modifiers; }
	std::shared_ptr<Modifier> getModifier(Int i) { return _modifierBuf.at(i); }
	std::vector<std::shared_ptr<Modifier>> getModifierList() { return _modifierBuf; }

	virtual void print(std::ostream &out) const override;

private:
	virtual void readData() final override;

	JdwpEventKind _eventKind;
	Byte _suspendPolicy = 0;
	Int _modifiers = 0;
	std::vector<std::shared_ptr<Modifier>> _modifierBuf;
};

#include "JdwpCommandErsetPacket.hpp"

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_COMMAND_JDWPCOMMANDERSETPACKET_H_ */
