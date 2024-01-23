#include "pch.h"
#include "M_AudioSystem.h"

#include "M_Scene.h"
#include "M_Engine.h"

#include "M_AudioListenerComponent.h"
#include "M_AudioSourceComponent.h"
#include "M_DirtyComponent.h"

using namespace minty;

minty::AudioSystem::AudioSystem(Engine& engine, ID const sceneId)
	: System::System(engine, sceneId)
{}

void minty::AudioSystem::update()
{
	TransformComponent* transComp;

	EntityRegistry& entityRegistry = get_entity_registry();
	AudioEngine& audioEngine = get_audio_engine();

	bool listenerDirty = entityRegistry.all_of<DirtyComponent>(_listener);

	// update listener data in engine
	if (_listener != NULL_ENTITY && listenerDirty)
	{
		transComp = entityRegistry.try_get<TransformComponent>(_listener);

		if (transComp)
		{
			// get data for listener
			update_sound_data(_listenerData, _listener);
			Vector3 up = transComp->get_up();
			Vector3 forward = transComp->get_forward();

			// update that data in the audio engine
			audioEngine.set_listener_parameters(_listenerData.position, forward, up, _listenerData.velocity);
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
		// check if sound is still playing
		if (audioEngine.is_valid_handle(pair.first))
		{
			// still playing, update 3d location if needed
			if (pair.second != NULL_ENTITY && entityRegistry.all_of<DirtyComponent>(pair.second) && (transComp = entityRegistry.try_get<TransformComponent>(pair.second)))
			{
				update_sound_data(sourceData, pair.second);

				audioEngine.set_source_parameters(pair.first, sourceData.position, sourceData.velocity);
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
	audioEngine.apply();
}


AudioHandle minty::AudioSystem::play(ID const id, float const volume, float const pan, bool const paused, unsigned int const bus)
{
	if (id == ERROR_ID) return ERROR_AUDIO_HANDLE;

	// get clip to play
	AudioClip& clip = at(id);

	// start playing
	AudioHandle handle = get_audio_engine().play(clip, volume, pan, paused);

	// mark as playing
	_playing[handle] = NULL_ENTITY;

	return handle;
}

AudioHandle minty::AudioSystem::play(String const& name, float const volume, float const pan, bool const paused, unsigned int const bus)
{
	return play(get_id(name), volume, pan, paused, bus);
}

AudioHandle minty::AudioSystem::play_spatial(Entity const entity, float const volume, bool const paused, unsigned int const bus)
{
	EntityRegistry& entityRegistry = get_entity_registry();

	MINTY_ASSERT(entityRegistry.all_of<AudioSourceComponent>(entity), "The entity for play_spatial must have an AudioSourceComponent component.");

	AudioSourceComponent& source = entityRegistry.get<AudioSourceComponent>(entity);

	if (source.clipId == ERROR_ID) return ERROR_AUDIO_HANDLE;

	return play_spatial(source.clipId, entity, volume, paused, bus);
}

AudioHandle minty::AudioSystem::play_spatial(ID const id, Entity const entity, float const volume, bool const paused, unsigned int const bus)
{
	if (id == ERROR_ID) return ERROR_AUDIO_HANDLE;

	EntityRegistry& entityRegistry = get_entity_registry();

	MINTY_ASSERT(entityRegistry.all_of<AudioSourceComponent>(entity), "The entity for play_spatial must have an AudioSourceComponent component.");

	AudioSourceComponent& source = entityRegistry.get<AudioSourceComponent>(entity);
	AudioEngine& audioEngine = get_audio_engine();

	// entity tied to sound, so play in 3d space
	SoundData data{};
	update_sound_data(data, entity);
	AudioHandle handle = audioEngine.play_spatial(_clips.at(id), data.position, data.velocity, volume, paused, bus);

	audioEngine.set_source_attenuation(handle, SoLoud::AudioSource::LINEAR_DISTANCE, source.attenuation);
	audioEngine.set_source_min_max_distance(handle, source.nearDistance, source.farDistance);
	audioEngine.apply();

	_playing[handle] = entity;

	return handle;
}

AudioHandle minty::AudioSystem::play_spatial(String const& name, Entity const entity, float const volume, bool const paused, unsigned int const bus)
{
	return play_spatial(get_id(name), entity, volume, paused, bus);
}

AudioHandle minty::AudioSystem::play_background(ID const id, float const volume, bool const paused, unsigned int const bus)
{
	if (id == ERROR_ID) return ERROR_AUDIO_HANDLE;

	// get clip to play
	AudioClip& clip = at(id);

	// start playing
	SoLoud::handle handle = get_audio_engine().play_background(clip, volume, paused, bus);

	// add to playing
	_playing[handle] = NULL_ENTITY;

	return handle;
}

AudioHandle minty::AudioSystem::play_background(String const& name, float const volume, bool const paused, unsigned int const bus)
{
	return play_background(_clips.get_id(name), volume, paused, bus);
}

bool minty::AudioSystem::is_playing(AudioHandle const handle)
{
	return get_audio_engine().is_valid_handle(handle);
}

void minty::AudioSystem::set_pause(AudioHandle const handle, bool const paused)
{
	get_audio_engine().set_pause(handle, paused);
}

bool minty::AudioSystem::get_pause(AudioHandle const handle)
{
	return get_audio_engine().get_pause(handle);
}

void minty::AudioSystem::stop(AudioHandle const handle)
{
	auto const& found = _playing.find(handle);

	if (found != _playing.end())
	{
		get_audio_engine().stop(handle);
		_playing.erase(handle);
	}
}

void minty::AudioSystem::stop_all()
{
	get_audio_engine().stop_all();
}

ID minty::AudioSystem::load_clip(String const& name, Path const& path)
{
	// add to clips
	ID id = _clips.emplace(name, AudioClip());

	// load into clip
	_clips.at(id).load(path);

	return id;
}

ID minty::AudioSystem::load_clip(Path const& path)
{
	return load_clip(Path(path).stem().string(), path);
}

bool minty::AudioSystem::contains(ID const id) const
{
	return _clips.contains(id);
}

bool minty::AudioSystem::contains(String const& name) const
{
	return _clips.contains(name);
}

AudioClip& minty::AudioSystem::at(ID const id)
{
	return _clips.at(id);
}

AudioClip const& minty::AudioSystem::at(ID const id) const
{
	return _clips.at(id);
}

AudioClip& minty::AudioSystem::at(String const& name)
{
	return _clips.at(name);
}

AudioClip const& minty::AudioSystem::at(String const& name) const
{
	return _clips.at(name);
}

String minty::AudioSystem::get_name(ID const id) const
{
	return _clips.get_name(id);
}

ID minty::AudioSystem::get_id(String const& name) const
{
	if (name.empty()) return ERROR_ID;

	return _clips.get_id(name);
}

void minty::AudioSystem::destroy_clip(ID const id)
{
	// stop clip
	stop(id);

	// remove
	_clips.erase(id);
}

void minty::AudioSystem::destroy_all_clips()
{
	// stop all clips
	stop_all();

	// remove all
	_clips.clear();
}

AudioEngine& minty::AudioSystem::get_audio_engine() const
{
	return get_engine().get_audio_engine();
}

void minty::AudioSystem::set_listener(Entity const entity)
{
	MINTY_ASSERT(get_entity_registry().all_of<AudioListenerComponent>(entity), "Listener Entity must have an AudioListenerComponent component.");

	_listener = entity;
	update_sound_data(_listenerData, entity);
}

void minty::AudioSystem::update_sound_data(SoundData& data, Entity const entity)
{
	if (TransformComponent* transComp = get_entity_registry().try_get<TransformComponent>(entity))
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
