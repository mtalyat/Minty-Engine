#include "pch.h"
#include "M_AudioEngine.h"

#include "M_Audio.h"
#include "M_File.h"
#include "M_Assets.h"
#include "M_Console.h"

using namespace minty;

minty::AudioEngine::AudioEngine()
	: _engine()
	, _clips()
	, _playing()
{
	_engine.init();
}

minty::AudioEngine::~AudioEngine()
{
	destroy_all_clips();

	_engine.deinit();
}

void minty::AudioEngine::play(ID const id)
{
	// get clip to play
	AudioClip& clip = at(id);

	// start playing
	SoLoud::handle handle = _engine.play(clip._clip);

	// add to playing
	_playing[id] = handle;
}

void minty::AudioEngine::play(std::string const& name)
{
	play(_clips.get_id(name));
}

void minty::AudioEngine::stop(ID const id)
{
	auto const& found = _playing.find(id);

	if (found != _playing.end())
	{
		_engine.stop(found->second);
		_playing.erase(id);
	}
}

void minty::AudioEngine::stop_all()
{
	_engine.stopAll();
	_playing.clear();
}

ID minty::AudioEngine::load_clip(std::string const& name, std::string const& path)
{
	// add to clips
	ID id = _clips.emplace(name, AudioClip());

	// load into clip
	_clips.at(id).load(path);

	return id;
}

ID minty::AudioEngine::load_clip(std::string const& path)
{
	return load_clip(file::name(path), path);
}

AudioClip& minty::AudioEngine::at(ID const id)
{
	return _clips.at(id);
}

AudioClip const& minty::AudioEngine::at(ID const id) const
{
	return _clips.at(id);
}

AudioClip& minty::AudioEngine::at(std::string const& name)
{
	return _clips.at(name);
}

AudioClip const& minty::AudioEngine::at(std::string const& name) const
{
	return _clips.at(name);
}

void minty::AudioEngine::destroy_clip(ID const id)
{
	// stop clip
	stop(id);

	// remove
	_clips.erase(id);
}

void minty::AudioEngine::destroy_all_clips()
{
	// stop all clips
	stop_all();

	// remove all
	_clips.clear();
}

std::string minty::to_string(AudioEngine const& value)
{
	return "AudioEngine()";
}
