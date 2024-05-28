#pragma once

#include "Minty/Libraries/M_SoLoud.h"

namespace Minty
{
	typedef SoLoud::handle AudioHandle;
	constexpr static AudioHandle ERROR_AUDIO_HANDLE = UINT_MAX;
}