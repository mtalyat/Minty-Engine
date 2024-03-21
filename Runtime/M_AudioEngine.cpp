#include "pch.h"
#include "M_AudioEngine.h"

#include "M_Audio.h"
#include "M_AudioSourceComponent.h"
#include "M_AudioListenerComponent.h"
#include "M_AudioClip.h"

#include "M_TransformComponent.h"
#include "M_DirtyComponent.h"

#include "M_File.h"
#include "M_Asset.h"
#include "M_Console.h"
#include "M_Scene.h"

#include <unordered_set>

using namespace minty;

minty::AudioEngine::AudioEngine(Runtime& runtime)
	: Engine(runtime)
	, _engine()
{
	_engine.init(SoLoud::Soloud::CLIP_ROUNDOFF || SoLoud::Soloud::LEFT_HANDED_3D);
	// SoLoud::Soloud::CLIP_ROUNDOFF
		//SoLoud::Soloud::ENABLE_VISUALIZATION | 
		//SoLoud::Soloud::LEFT_HANDED_3D);
}

minty::AudioEngine::~AudioEngine()
{
	_engine.deinit();
}

void minty::AudioEngine::set_listener_parameters(Vector3 const position, Vector3 const forward, Vector3 const up, Vector3 const velocity)
{
	_engine.set3dListenerParameters(position.x, position.y, position.z, forward.x, forward.y, forward.z, up.x, up.y, up.z, velocity.x, velocity.y, velocity.z);
}

void minty::AudioEngine::set_listener_position(Vector3 const position)
{
	_engine.set3dListenerPosition(position.x, position.y, position.z);
}

void minty::AudioEngine::set_listener_forward(Vector3 const forward)
{
	_engine.set3dListenerAt(forward.x, forward.y, forward.z);
}

void minty::AudioEngine::set_listener_up(Vector3 const up)
{
	_engine.set3dListenerUp(up.x, up.y, up.z);
}

void minty::AudioEngine::set_listener_velocity(Vector3 const velocity)
{
	_engine.set3dListenerVelocity(velocity.x, velocity.y, velocity.z);
}

bool minty::AudioEngine::is_valid_handle(AudioHandle const handle)
{
	return _engine.isValidVoiceHandle(handle);
}

void minty::AudioEngine::set_source_parameters(AudioHandle const handle, Vector3 const position, Vector3 const velocity)
{
	_engine.set3dSourceParameters(handle, position.x, position.y, position.z, velocity.x, velocity.y, velocity.z);
}

void minty::AudioEngine::set_source_position(AudioHandle const handle, Vector3 const position)
{
	_engine.set3dSourcePosition(handle, position.x, position.y, position.z);
}

void minty::AudioEngine::set_source_velocity(AudioHandle const handle, Vector3 const velocity)
{
	_engine.set3dSourceVelocity(handle, velocity.x, velocity.y, velocity.z);
}

void minty::AudioEngine::set_source_attenuation(AudioHandle const handle, unsigned int const model, float const attenuation)
{
	_engine.set3dSourceAttenuation(handle, model, attenuation);
}

void minty::AudioEngine::set_source_min_max_distance(AudioHandle const handle, float const nearDistance, float const farDistance)
{
	_engine.set3dSourceMinMaxDistance(handle, nearDistance, farDistance);
}

void minty::AudioEngine::apply()
{
	_engine.update3dAudio();
}

AudioHandle minty::AudioEngine::play(AudioClip& clip, float const volume, float const pan, bool const paused, unsigned int const bus)
{
	return _engine.play(clip._clip, volume, pan, paused, bus);
}

AudioHandle minty::AudioEngine::play_spatial(AudioClip& clip, Vector3 const position, Vector3 const velocity, float const volume, bool const paused, unsigned int const bus)
{
	return _engine.play3d(clip._clip, position.x, position.y, position.z, velocity.x, velocity.y, velocity.z, volume, paused, bus);
}

AudioHandle minty::AudioEngine::play_background(AudioClip& clip, float const volume, float const paused, unsigned int const bus)
{
	return _engine.playBackground(clip._clip, volume, paused, bus);
}

void minty::AudioEngine::set_pause(AudioHandle const handle, bool const paused)
{
	_engine.setPause(handle, paused);
}

void minty::AudioEngine::set_pause_all(bool const paused)
{
	_engine.setPauseAll(paused);
}

bool minty::AudioEngine::get_pause(AudioHandle const handle)
{
	return _engine.getPause(handle);
}

void minty::AudioEngine::stop(AudioHandle const handle)
{
	_engine.stop(handle);
}

void minty::AudioEngine::stop_all()
{
	_engine.stopAll();
}

String minty::to_string(AudioEngine const& value)
{
	return "AudioEngine()";
}
