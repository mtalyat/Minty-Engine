#include "pch.h"
#include "M_Animation.h"

#include "M_SerializationData.h"
#include "M_RenderEngine.h"

using namespace minty;

minty::Animation::Animation()
	: _frameTime(0.25f)
	, _frames()
{}

minty::Animation::Animation(AnimationBuilder const& builder)
	: _frameTime(builder.frameTime)
	, _frames(builder.frames)
{}

float minty::Animation::get_frame_time() const
{
	return _frameTime;
}

size_t minty::Animation::get_frame_count() const
{
	return _frames.size();
}

ID minty::Animation::get_frame(size_t const index) const
{
	return _frames.at(index);
}

void minty::Animation::serialize(Writer& writer) const
{
	SerializationData* data = static_cast<SerializationData*>(writer.get_data());
	RenderEngine& renderer = data->scene->get_engine()->get_render_engine();

	// write basic types
	writer.write("frameTime", _frameTime);
	
	// convert all sprite IDs to names, then write the names
	std::vector<String> names(_frames.size());
	for (size_t i = 0; i < _frames.size(); i++)
	{
		names[i] = renderer.get_sprite_name(_frames.at(i));
	}

	writer.write("frames", names);
}

void minty::Animation::deserialize(Reader const& reader)
{
	SerializationData* data = static_cast<SerializationData*>(reader.get_data());
	RenderEngine& renderer = data->scene->get_engine()->get_render_engine();

	// get basic types
	_frameTime = reader.read_float("frameTime");

	// convert all names to IDs
	std::vector<String> names;
	reader.read_vector("frames", names);
	_frames.clear();
	_frames.resize(names.size());
	for (size_t i = 0; i < names.size(); i++)
	{
		_frames[i] = renderer.find_sprite(names.at(i));
	}
}
