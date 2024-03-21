#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include "soloud.h"
#include "soloud_wav.h"

namespace minty
{
	typedef SoLoud::handle AudioHandle;
	constexpr static AudioHandle ERROR_AUDIO_HANDLE = UINT_MAX;
}