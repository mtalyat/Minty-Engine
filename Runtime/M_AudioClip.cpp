#include "pch.h"
#include "M_AudioClip.h"

#include "M_Runtime.h"
#include "M_AssetEngine.h"
#include "M_Asset.h"
#include "M_Console.h"
#include "M_Reader.h"
#include "M_Writer.h"

minty::AudioClip::AudioClip()
	: _clip()
{}

minty::AudioClip::AudioClip(AudioClipBuilder const& builder, Runtime& runtime)
	: Asset(builder.id, builder.path, runtime)
	, _clip()
{
	load(builder.path);
}

void minty::AudioClip::load(Path const& path)
{
	AssetEngine& assets = get_runtime().get_asset_engine();
	std::vector<Byte> fileData = assets.read_file_bytes(path);

	// load clip
	SoLoud::result result = _clip.loadMem(fileData.data(), static_cast<unsigned int>(fileData.size()), true, false);

	if (result != SoLoud::SOLOUD_ERRORS::SO_NO_ERROR)
	{
		MINTY_ERROR_FORMAT("Failed to load AudioClip at path \"{}\". Error code {}.", path.string(), result);
		return;
	}
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
