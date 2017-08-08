/*
 * JdwpReplyTrframesPacket.h
 *
 *  Created on: 2016. 3. 31.
 *      Author: Seunghun Han
 */

#ifndef JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYTRFRAMESPACKET_H_
#define JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYTRFRAMESPACKET_H_

#include "../JdwpReplyPacket.h"

#include <vector>

#include "../JdwpTypes.h"

/**
 * ThreadReference Command Set (11)
 * Frames Command (6)
 */
template <typename Vm>
class JdwpReplyTrframesPacket final : public JdwpReplyPacket {
public:
	class Frame final {
	public:
		void setFrameID(typename JdwpTypes<Vm>::FrameID frameID) { _frameID = frameID; }
		void setLocation(typename JdwpTypes<Vm>::Location location) { _location = location; }

		virtual std::size_t size() const { return sizeof(_frameID) + _location.size(); }
		virtual void print(std::ostream &out) const;

		virtual std::vector<std::uint8_t> pack() const { std::vector<std::uint8_t> buf(size()); pack(buf.begin()); return buf; }
		virtual void pack(std::vector<std::uint8_t>::iterator iter) const;

	private:
		typename JdwpTypes<Vm>::FrameID _frameID;
		typename JdwpTypes<Vm>::Location _location;
	};

	JdwpReplyTrframesPacket();
	virtual ~JdwpReplyTrframesPacket();

	void setFrames(const std::vector<Frame> &frames);
	void pushFrame(const Frame &frame);

	virtual void print(std::ostream &out) const override;

private:
	virtual void writeData() final override;

	std::vector<Frame> _frames;
};

template <typename Vm>
void JdwpReplyTrframesPacket<Vm>::Frame::print(std::ostream &out) const
{
	out << "(frameID : ";
	out << static_cast<std::int64_t>(_frameID);
	out << ", location : ";
	_location.print(out);
	out << ")";
}

template <typename Vm>
void JdwpReplyTrframesPacket<Vm>::Frame::pack(std::vector<std::uint8_t>::iterator iter) const
{
	::pack(_frameID, iter);
	iter += sizeof(_frameID);

	_location.pack(iter);
}

template <typename Vm>
JdwpReplyTrframesPacket<Vm>::JdwpReplyTrframesPacket()
{
	writeData();
}

template <typename Vm>
JdwpReplyTrframesPacket<Vm>::~JdwpReplyTrframesPacket()
{

}

template <typename Vm>
void JdwpReplyTrframesPacket<Vm>::setFrames(const std::vector<Frame> &frames)
{
	_frames = frames;
	writeData();
}

template <typename Vm>
void JdwpReplyTrframesPacket<Vm>::pushFrame(const Frame &frame)
{
	_frames.push_back(frame);

	Int frames = _frames.size();
	::pack(frames, _dataBuf.begin());

	auto f = frame.pack();
	_dataBuf.insert(_dataBuf.end(), f.cbegin(), f.cend());
}

template <typename Vm>
void JdwpReplyTrframesPacket<Vm>::print(std::ostream &out) const
{
	JdwpReplyPacket::print(out);
	out << "TR_FRAMES: ";
	out << "(";
	out << "frames : ";
	Int frames = _frames.size();
	out << frames;

	for(Int i=0; i<frames; i++) {
		out << ", frame" << i << " : ";
		_frames[i].print(out);
	}

	out << ")";
}

template <typename Vm>
void JdwpReplyTrframesPacket<Vm>::writeData()
{
	_dataBuf.clear();

	Int frames = _frames.size();
	auto a = ::pack(frames);
	_dataBuf.insert(_dataBuf.end(), a.cbegin(), a.cend());

	for(Int i = 0; i<frames; i++) {
		Frame frame = _frames[i];
		auto f = frame.pack();
		_dataBuf.insert(_dataBuf.end(), f.cbegin(), f.cend());
	}
}

#endif /* JIKESDBG_SRC_CONNECTION_JDWP_REPLY_JDWPREPLYTRFRAMESPACKET_H_ */
