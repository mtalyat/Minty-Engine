#include "pch.h"
#include "M_AudioEngine.h"

#include "M_Audio.h"
#include "M_AudioSourceComponent.h"
#include "M_AudioListenerComponent.h"

#include "M_TransformComponent.h"
#include "M_DirtyTag.h"

#include "M_File.h"
#include "M_Assets.h"
#include "M_Console.h"
#include "M_Scene.h"

#include <unordered_set>

using namespace minty;

minty::AudioEngine::AudioEngine()
	: _engine()
	, _clips()
	, _playing()
	, _scene()
	, _registry()
	, _listener(NULL_ENTITY)
{
	_engine.init(SoLoud::Soloud::CLIP_ROUNDOFF || SoLoud::Soloud::LEFT_HANDED_3D);
	// SoLoud::Soloud::CLIP_ROUNDOFF
		//SoLoud::Soloud::ENABLE_VISUALIZATION | 
		//SoLoud::Soloud::LEFT_HANDED_3D);
}

minty::AudioEngine::~AudioEngine()
{
	destroy_all_clips();

	_engine.deinit();
}

void minty::AudioEngine::update()
{
	TransformComponent* transComp;

	bool listenerDirty = _registry->any_of<Dirty>(_listener);

	// update listener data in engine
	if (_listener != NULL_ENTITY && listenerDirty)
	{
		transComp = _registry->try_get<TransformComponent>(_listener);

		if (transComp)
		{
			update_sound_data(_listenerData, _listener);
			Vector3 up = transComp->get_up();

			_engine.set3dListenerPosition(_listenerData.position.x, _listenerData.position.y, _listenerData.position.z);
			_engine.set3dListenerVelocity(_listenerData.velocity.x, _listenerData.velocity.y, _listenerData.velocity.z);
			_engine.set3dListenerUp(up.x, up.y, up.z);

			//console::log(std::format("Listener: {}", to_string(_listenerData.position)));
		}
	}

	// sounds that have completed playing
	std::unordered_set<ID> finished;

	// data for the source
	SoundData sourceData{};

	// check all playing sounds, if they have stopped, then remove from playing
	// if they have not stopped, then update in 3D space relative to the camera
	for (auto& pair : _playing)
	{
		if (_engine.isValidVoiceHandle(pair.second.handle) && pair.second.entity != NULL_ENTITY)
		{
			// still playing, update 3d location
			
			// if entity dirty or needs updated AND has position component
			if (_registry->all_of<Dirty>(pair.second.entity) && (transComp = _registry->try_get<TransformComponent>(pair.second.entity)))
			{
				update_sound_data(sourceData, pair.second.entity);

				_engine.set3dSourcePosition(pair.second.handle, sourceData.position.x, sourceData.position.y, sourceData.position.z);
				_engine.set3dSourceVelocity(pair.second.handle, sourceData.velocity.x, sourceData.velocity.y, sourceData.velocity.z);

				//console::log(std::format("Source: {}", to_string(sourceData.position)));
			}
		}
		else
		{
			// no longer playing
			finished.emplace(pair.first);
		}
	}

	// remove finished sounds
	for (auto const id : finished)
	{
		_playing.erase(id);
	}

	// finalize updates
	_engine.update3dAudio();
}

void minty::AudioEngine::play(ID const id, Entity const entity)
{
	if (id == ERROR_ID) return;

	// get clip to play
	AudioClip& clip = at(id);

	// start playing
	SoLoud::handle handle;
	if (entity == NULL_ENTITY)
	{
		// no entity tied to sound, so just play
		handle = _engine.play(clip._clip);
	}
	else
	{
		// entity tied to sound, so play in 3d space
		SoundData data{};
		update_sound_data(data, entity);
		handle = _engine.play3d(clip._clip, data.position.x, data.position.y, data.position.z, data.velocity.x, data.velocity.y, data.velocity.z);

		if (AudioSource* source = _registry->try_get<AudioSource>(entity))
		{
			_engine.set3dSourceAttenuation(handle, SoLoud::AudioSource::LINEAR_DISTANCE, source->attenuation);
			_engine.set3dSourceMinMaxDistance(handle, source->nearDistance, source->farDistance);
			_engine.update3dAudio();
		}		
	}

	// add to playing
	_playing[id] = SoundSource
	{
		.entity = entity,
		.handle = handle
	};
}

void minty::AudioEngine::play(std::string const& name, Entity const entity)
{
	play(_clips.get_id(name), entity);
}

void minty::AudioEngine::play(Entity const entity)
{
	// if no registry, do nothing
	if (!_registry)
	{
		return;
	}

	// play sound if entity is not null and has an audio source
	if (AudioSource* source = _registry->try_get<AudioSource>(entity))
	{
		play(source->clipId, entity);
	}
}

void minty::AudioEngine::play_background(ID const id)
{
	if (id == ERROR_ID) return;

	// get clip to play
	AudioClip& clip = at(id);

	// start playing
	SoLoud::handle handle = _engine.playBackground(clip._clip);

	// add to playing
	_playing[id] = SoundSource
	{
		.entity = NULL_ENTITY,
		.handle = handle
	};
}

void minty::AudioEngine::play_background(std::string const& name)
{
	play_background(_clips.get_id(name));
}

bool minty::AudioEngine::is_playing(ID const id) const
{
	return _playing.contains(id);
}

bool minty::AudioEngine::is_playing(std::string const& name) const
{
	return is_playing(_clips.get_id(name));
}

void minty::AudioEngine::stop(ID const id)
{
	auto const& found = _playing.find(id);

	if (found != _playing.end())
	{
		_engine.stop(found->second.handle);
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

std::string const& minty::AudioEngine::get_name(ID const id) const
{
	if (id == ERROR_ID) return "";

	return _clips.get_alias(id);
}

ID minty::AudioEngine::get_id(std::string const& name) const
{
	if (name.empty()) return ERROR_ID;

	return _clips.get_id(name);
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

void minty::AudioEngine::set_listener(Entity const entity)
{
	_listener = entity;
	update_sound_data(_listenerData, entity);
}

void minty::AudioEngine::set_scene(Scene const* const scene, Entity const listener)
{
	// update scene that is being used
	_scene = scene;

	// if scene is null, so are other things
	if (!_scene)
	{
		_registry = nullptr;
		_listener = NULL_ENTITY;
		return;
	}

	// set registry to scene registry
	_registry = _scene->get_entity_registry();

	// update camera we are rendering from
	if (listener == NULL_ENTITY)
	{
		// no listener provided, attempt to find first one in scene
		set_listener(_scene->get_entity_registry()->find_by_type<AudioListener>());
	}
	else
	{
		// camera provided, use that
		set_listener(listener);
	}
}

void minty::AudioEngine::update_sound_data(SoundData& data, Entity const entity)
{
	if (TransformComponent* transComp = _registry->try_get<TransformComponent>(entity))
	{
		data.position = transComp->get_global_position();
		data.velocity = Vector3(); // TEMP
	}
	else
	{
		data.position = Vector3();
		data.velocity = Vector3();
	}
}

std::string minty::to_string(AudioEngine const& value)
{
	return "AudioEngine()";
}
