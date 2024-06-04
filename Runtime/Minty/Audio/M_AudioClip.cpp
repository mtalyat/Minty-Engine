#include "pch.h"
#include "Minty/Audio/M_AudioClip.h"

#include "Minty/Assets/M_AssetEngine.h"
#include "Minty/Assets/M_Asset.h"
#include "Minty/Tools/M_Console.h"
#include "Minty/Serialization/M_Reader.h"
#include "Minty/Serialization/M_Writer.h"

Minty::AudioClip::AudioClip()
	: _clip()
{}

Minty::AudioClip::AudioClip(AudioClipBuilder const& builder)
	: Asset(builder.id, builder.path)
	, _clip()
{
	load(builder.path);
}

void Minty::AudioClip::load(Path const& path)
{
	AssetEngine& assets = AssetEngine::instance();
	std::vector<Byte> fileData = assets.read_file_bytes(path);

	// load clip
	SoLoud::result result = _clip.loadMem(fileData.data(), static_cast<unsigned int>(fileData.size()), true, false);

	if (result != SoLoud::SOLOUD_ERRORS::SO_NO_ERROR)
	{
		MINTY_ERROR_FORMAT("Failed to load AudioClip at path \"{}\". Error code {}.", path.string(), result);
		return;
	}
}

void Minty::AudioClip::set_volume(float const value)
{
	_clip.setVolume(value);
}

void Minty::AudioClip::set_looping(bool const looping)
{
	_clip.setLooping(looping);
}

void Minty::AudioClip::set_loop_point(float const point)
{
	_clip.setLoopPoint(static_cast<SoLoud::time>(point));
}

void Minty::AudioClip::set_single_instance(bool const value)
{
	_clip.setSingleInstance(value);
}
