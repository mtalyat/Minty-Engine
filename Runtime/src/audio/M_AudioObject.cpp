#include "pch.h"
#include "audio/M_AudioObject.h"

#include "runtime/M_Runtime.h"
#include "audio/M_AudioEngine.h"

using namespace minty;

minty::AudioObject::AudioObject()
	: RuntimeObject()
{}

minty::AudioObject::AudioObject(Runtime& engine)
	: RuntimeObject(engine)
{}

minty::AudioObject::~AudioObject()
{}

AudioEngine& minty::AudioObject::get_audio_engine() const
{
	return get_runtime().get_audio_engine();
}
