#pragma once

#include "libraries/M_SoLoud.h"

namespace minty
{
	typedef SoLoud::handle AudioHandle;
	constexpr static AudioHandle ERROR_AUDIO_HANDLE = UINT_MAX;
}