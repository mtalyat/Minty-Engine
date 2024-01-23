#include "pch.h"
#include "M_AudioObject.h"

#include "M_Engine.h"
#include "M_AudioEngine.h"

using namespace minty;

minty::AudioObject::AudioObject()
	: SceneObject()
{}

minty::AudioObject::AudioObject(Engine& engine, ID const sceneId)
	: SceneObject(engine, sceneId)
{}

minty::AudioObject::~AudioObject()
{}

AudioEngine& minty::AudioObject::get_audio_engine() const
{
	return get_engine().get_audio_engine();
}
