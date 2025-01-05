#include "pch.h"
#include "AudioSystem.h"

#include "Minty/Asset/AssetManager.h"
#include "Minty/Audio/AudioEngine.h"
#include "Minty/Component/AllComponents.h"
#include "Minty/Entity/EntityRegistry.h"
#include "Minty/Scene/Scene.h"

using namespace Minty;

Minty::AudioSystem::AudioSystem(Scene& scene)
	: System::System("Audio", scene)
	, m_clips()
	, m_playing()
	, m_listener(NULL_ENTITY)
	, m_listenerData()
{}

void Minty::AudioSystem::update(Time const& time)
{
	EntityRegistry& entityRegistry = get_entity_registry();

	TransformComponent* transComp;
	Bool listenerDirty = entityRegistry.all_of<DirtyComponent>(m_listener);

	// update listener data in engine
	if (m_listener != NULL_ENTITY && listenerDirty)
	{
		transComp = entityRegistry.try_get<TransformComponent>(m_listener);

		if (transComp)
		{
			// get data for listener
			initialize_audio_listener(m_listener, m_listenerData);
			Float3 up = transComp->get_up();
			Float3 forward = transComp->get_forward();

			// update that data in the audio engine
			AudioEngine::set_audio_listener(m_listenerData);
		}
	}

	// sounds that have completed playing
	std::unordered_set<Handle> finished;

	// data for the source
	AudioSource sourceData{};

	// if there are any disabled entities with an audiosource, stop playing the sound
	for (auto [entity, source] : entityRegistry.view<AudioSourceComponent>(entt::exclude<EnabledComponent>).each())
	{
		stop(source.handle);
		source.handle = INVALID_HANDLE;
	}

	// check all playing sounds, if they have stopped, then remove from playing
	// if they have not stopped, then update in 3D space relative to the camera
	for (auto& pair : m_playing)
	{
		// check if sound is still playing
		if (AudioEngine::is_valid_handle(pair.first))
		{
			// still playing, update 3d location if needed
			if (pair.second != NULL_ENTITY && entityRegistry.all_of<DirtyComponent>(pair.second) && (transComp = entityRegistry.try_get<TransformComponent>(pair.second)))
			{
				initialize_audio_source(pair.second, sourceData);
				AudioEngine::set_audio_source(pair.first, sourceData);
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
		Entity entity = m_playing.at(id);
		if (entity != NULL_ENTITY)
		{
			// if there was an entity, reset the audio source
			if (AudioSourceComponent* sourceComponent = entityRegistry.try_get<AudioSourceComponent>(entity))
			{
				sourceComponent->handle = INVALID_HANDLE;
			}
		}
		m_playing.erase(id);
	}

	// finalize updates
	AudioEngine::apply();
}

void Minty::AudioSystem::set_listener(Entity const entity)
{
	MINTY_ASSERT_MESSAGE(get_entity_registry().all_of<AudioListenerComponent>(entity), "Listener Entity must have an AudioListenerComponent component.");

	m_listener = entity;
	initialize_audio_listener(entity, m_listenerData);
}

Handle Minty::AudioSystem::play(Ref<AudioClip> const clip, Float const volume, Float const pan, Bool const paused, UInt const bus)
{
	// start playing
	Handle handle = AudioEngine::play(clip, volume, pan, paused);

	// mark as playing
	m_playing[handle] = NULL_ENTITY;

	return handle;
}

Handle Minty::AudioSystem::play(String const& name, Float const volume, Float const pan, Bool const paused, UInt const bus)
{
	return play(m_clips.at(name), volume, pan, paused, bus);
}

Handle Minty::AudioSystem::play_object(Entity const entity, Float const volume, Bool const paused, UInt const bus)
{
	EntityRegistry& entityRegistry = get_entity_registry();

	MINTY_ASSERT_MESSAGE(entityRegistry.all_of<AudioSourceComponent>(entity), "The entity for play_object must have an AudioSourceComponent component.");

	AudioSourceComponent& source = entityRegistry.get<AudioSourceComponent>(entity);

	// source must have a clip
	if (!source.clip) return INVALID_HANDLE;

	return play_object(source.clip, entity, volume, paused, bus);
}

Handle Minty::AudioSystem::play_object(Ref<AudioClip> const clip, Entity const entity, Float const volume, Bool const paused, UInt const bus)
{
	EntityRegistry& entityRegistry = get_entity_registry();

	MINTY_ASSERT_MESSAGE(entityRegistry.all_of<AudioSourceComponent>(entity), "The entity for play_object must have an AudioSourceComponent component.");

	AudioSourceComponent& source = entityRegistry.get<AudioSourceComponent>(entity);

	// entity tied to sound, so play in 3d space
	AudioSource data{};
	initialize_audio_source(entity, data);
	Handle handle = AudioEngine::play_object(clip, data, volume, paused, bus);
	AudioEngine::apply();

	m_playing[handle] = entity;

	return handle;
}

Handle Minty::AudioSystem::play_object(String const& name, Entity const entity, Float const volume, Bool const paused, UInt const bus)
{
	return play_object(m_clips.at(name), entity, volume, paused, bus);
}

Handle Minty::AudioSystem::play_background(Ref<AudioClip> const clip, Float const volume, Bool const paused, UInt const bus)
{
	// start playing
	Handle handle = AudioEngine::play_background(clip, volume, paused, bus);

	// add to playing
	m_playing[handle] = NULL_ENTITY;

	return handle;
}

Handle Minty::AudioSystem::play_background(String const& name, Float const volume, Bool const paused, UInt const bus)
{
	return play_background(m_clips.at(name), volume, paused, bus);
}

Bool Minty::AudioSystem::is_playing(Handle const handle)
{
	return AudioEngine::is_valid_handle(handle);
}

void Minty::AudioSystem::stop(Handle const handle)
{
	auto found = m_playing.find(handle);

	if (found != m_playing.end())
	{
		AudioEngine::stop(handle);
		m_playing.erase(handle);
	}
}

void Minty::AudioSystem::stop_all()
{
	AudioEngine::stop_all();
	m_playing.clear();
}

void Minty::AudioSystem::initialize_audio_listener(Entity const entity, AudioListener& listener)
{
	EntityRegistry& entityRegistry = get_entity_registry();
	if (TransformComponent* transComp = entityRegistry.try_get<TransformComponent>(entity))
	{
		listener.position = transComp->get_global_position();
		listener.forward = forward(transComp->globalMatrix);
	}
	else
	{
		listener.position = Float3(0.0f, 0.0f, 0.0f);
		listener.forward = Float3(0.0f, 0.0f, 1.0f);
	}
}

void Minty::AudioSystem::initialize_audio_source(Entity const entity, AudioSource& source)
{
	EntityRegistry& entityRegistry = get_entity_registry();
	if (TransformComponent* transComp = entityRegistry.try_get<TransformComponent>(entity))
	{
		source.position = transComp->get_global_position();
	}
	else
	{
		source.position = Float3();
	}
}
