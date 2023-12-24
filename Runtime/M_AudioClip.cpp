#include "pch.h"
#include "M_AudioClip.h"

#include "M_Asset.h"
#include "M_Console.h"

minty::AudioClip::AudioClip()
	: _clip()
{

}

void minty::AudioClip::load(Path const& path)
{
	// load clip
	SoLoud::result result = _clip.load(Asset::absolute(path).string().c_str());

	if (result != SoLoud::SOLOUD_ERRORS::SO_NO_ERROR)
	{
		console::error(std::format("Failed to load AudioClip at path \"{}\". Error code {}.", Asset::absolute(path).string(), result));
		return;
	}

	// load meta
	Node meta = Asset::load_meta(path);

	set_volume(meta.get_float("volume", 1.0f));
	set_looping(meta.get_bool("looping", false));
	set_loop_point(meta.get_float("loopPoint", 0.0f));
	set_single_instance(meta.get_bool("singleInstance", false));
}

void minty::AudioClip::set_volume(float const value)
{
	_clip.setVolume(value);
}

void minty::AudioClip::set_looping(bool const looping)
{
	_clip.setLooping(looping);
}

void minty::AudioClip::set_loop_point(float const point)
{
	_clip.setLoopPoint(static_cast<SoLoud::time>(point));
}

void minty::AudioClip::set_single_instance(bool const value)
{
	_clip.setSingleInstance(value);
}
